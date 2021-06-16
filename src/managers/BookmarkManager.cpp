#include "BookmarkManager.h"

#include <QUrl>
#include <QDir>
#include <QUuid>
#include <QTimer>
#include <QStyle>
#include <QtDebug>
#include <QApplication>
#include <QJsonObject>
#include <QMutexLocker>
#include <QJsonArray>
#include <QJsonParseError>
#include <QElapsedTimer>
#include <QStandardItem>
#include <QStandardItemModel>

#include "utils/util_qt.h"
#include "FaviconManager.h"

namespace bookmark {
int BookmarkNode::count = 0;
int BookmarkUrl::url_count = 0;
int BookmarkFolder::folder_count = 0;
}

BookmarkMgr* BookmarkMgr::gInst = nullptr;
QMutex BookmarkMgr::gMutex;
QStandardItemModel *BookmarkMgr::gBookmarkModel_ = nullptr;

BookmarkMgr::BookmarkMgr(QObject *parent)
    : QObject(parent)
{
    if(!gBookmarkModel_){
        gBookmarkModel_ = new QStandardItemModel(this);
    }
    connect(&FaviconMgr::Instance(), &FaviconMgr::iconUpdated, this, &BookmarkMgr::onFaviconUpdated);

    worker_ = new BookmarkWorker;
    worker_->moveToThread(&worker_thread_);
    connect(&worker_thread_, &QThread::finished, this, [this](){
        qInfo()<<"woker thread finished!";
        worker_->deleteLater();
    });
    connect(this, &BookmarkMgr::load, worker_, &BookmarkWorker::loadFromFile);
    connect(this, &BookmarkMgr::save, worker_, &BookmarkWorker::saveToFile);
    connect(worker_, &BookmarkWorker::loadFinished, this, &BookmarkMgr::onWokerLoadFinished);
    connect(worker_, &BookmarkWorker::saveFinished, this, &BookmarkMgr::onWokerSaveFinished);
    worker_thread_.start();

    QTimer::singleShot(1, this, &BookmarkMgr::load);
}

void BookmarkMgr::onWokerLoadFinished()
{
    updateModel();
    emit bookmarksChanged();
}

void BookmarkMgr::onWokerSaveFinished()
{
    qInfo()<<__FUNCTION__;
}

void BookmarkMgr::onFaviconUpdated(const QString &urlDomain)
{
    return;
    qInfo()<<__FUNCTION__;
    auto rows = gBookmarkModel_->rowCount();
    auto columns = gBookmarkModel_->columnCount();
    for(int i = 0; i < rows; i++){
        for(int j = 0; j < columns; j++){
            auto item = gBookmarkModel_->item(i, j);
            if(item){
                auto data = item->data(Qt::UserRole + 1).toBool();
                if(data){
                    auto url = item->data(Qt::UserRole + 2).toString();
                    if(QUrl(url).host() == urlDomain){
                        item->setIcon(QIcon(FaviconMgr::Instance().iconFilePath(url)));
                    }
                }
            }
        }
    }
}

void BookmarkMgr::updateModel()
{
    QElapsedTimer timer;
    timer.start();

    gBookmarkModel_->clear();

    auto node = bkmk_toolbar_bkmks_;
    if(!node){
        return;
    }
    QStandardItem *barItem = new QStandardItem(qApp->style()->standardIcon(QStyle::SP_DirIcon), node->name_);
    barItem->setData(0, Qt::UserRole + 1);
    if(auto barNode = dynamic_cast<const bookmark::BookmarkFolder*>(node)){
        for(auto node : barNode->children_){
            parseNode2Item(barItem, node);
        }
    }
    gBookmarkModel_->appendRow(barItem);
    qInfo()<<__FUNCTION__<<":" << timer.elapsed() << "milliseconds";
}

void BookmarkMgr::parseNode2Item(QStandardItem *parent, const bookmark::BookmarkNode *node)
{
    static const QIcon fileIcon = qApp->style()->standardIcon(QStyle::SP_FileIcon);
    static const QIcon dirIcon = qApp->style()->standardIcon(QStyle::SP_DirIcon);

    QStandardItem *item = new QStandardItem(dirIcon, node->name_);
    if(node->type_ == "folder"){
        item->setData(0, Qt::UserRole + 1);
        if(auto folder = dynamic_cast<const bookmark::BookmarkFolder*>(node)){
            for(auto child : folder->children_){
                parseNode2Item(item, child);
            }
        }
    }else if(node->type_ == "url"){
        if(auto url = dynamic_cast<const bookmark::BookmarkUrl*>(node)){
            auto iconPath = FaviconMgr::Instance().iconFilePath(url->url_);
            QIcon icon(iconPath);
            if(icon.isNull()){
                icon = fileIcon;
            }
            item->setIcon(icon);

            item->setData(1, Qt::UserRole + 1);
            item->setData(url->url_, Qt::UserRole + 2);
        }
    }

    parent->appendRow(item);
}

BookmarkMgr::~BookmarkMgr()
{
    qInfo()<<__FUNCTION__;
    worker_thread_.quit();
    worker_thread_.wait();

    /*delete 一个nullptr是合法的*/
    delete bkmk_toolbar_bkmks_;
    delete other_bkmks_;
    delete synced_bkmks_;
}

BookmarkMgr *BookmarkMgr::Instance()
{
    if(gInst == nullptr){
        QMutexLocker locker(&gMutex);
        if(gInst == nullptr){
            gInst = new BookmarkMgr;
        }
    }
    static BookmarkMgr::Gc gc;
    return gInst;
}

BookmarkWorker::BookmarkWorker(QObject *parent)
    :QObject(parent)
{
    auto loc = UtilQt::appDataPath();
    file_path_ = QDir(loc).filePath("Bookmarks");
    if(!QFileInfo::exists(file_path_)){
        createFileIfNotExist();
    }
}

void BookmarkWorker::createFileIfNotExist()
{
    QFile file(file_path_);
    if( !file.open(QIODevice::WriteOnly) )
    {
        qInfo()<<__FUNCTION__<<"Can't create bookmark file:"<<file_path_;
        return;
    }

    QJsonDocument jsonDoc;
    // 根节点三个 key
    QJsonObject root;
    root.insert("version", "1");

    QJsonObject obj_roots, obj_bookmark_bar, obj_other, obj_synced;

    // obj_bookmark_bar
    obj_bookmark_bar.insert("children",QJsonArray());
    obj_bookmark_bar.insert("date_added", makeEpochStr());
    obj_bookmark_bar.insert("date_modified", makeEpochStr());
    obj_bookmark_bar.insert("guid", makeUUidStr());
    obj_bookmark_bar.insert("id","1");
    obj_bookmark_bar.insert("name", QJsonValue(QStringLiteral("书签栏")));
    obj_bookmark_bar.insert("type","folder");

    obj_roots.insert("bookmark_bar", QJsonValue(obj_bookmark_bar));

    // obj_other
    obj_other.insert("children",QJsonArray());
    obj_other.insert("date_added", makeEpochStr());
    obj_other.insert("date_modified", makeEpochStr());
    obj_other.insert("guid", makeUUidStr());
    obj_other.insert("id","2");
    obj_other.insert("name", QJsonValue(QStringLiteral("其他书签")));
    obj_other.insert("type","folder");

    obj_roots.insert("other", QJsonValue(obj_other));

    //obj_synced
    obj_synced.insert("children",QJsonArray());
    obj_synced.insert("date_added", makeEpochStr());
    obj_synced.insert("date_modified", makeEpochStr());
    obj_synced.insert("guid", makeUUidStr());
    obj_synced.insert("id","3");
    obj_synced.insert("name", QJsonValue(QStringLiteral("移动设备书签")));
    obj_synced.insert("type","folder");

    obj_roots.insert("synced", QJsonValue(obj_synced));

    root.insert("roots", QJsonValue(obj_roots));

    // TODO:校验和暂时不知道该如何做
    // quint16 qChecksum(const char *data, uint len)
    root.insert("checksum", "");

    jsonDoc.setObject(root);

    // 写入新的内容到文件，覆盖写入
    file.write(jsonDoc.toJson());
    file.close();
}

QString BookmarkWorker::makeEpochStr(bool msecond)
{
    if(!msecond){
        return QString::number(QDateTime::currentSecsSinceEpoch());
    }else{
        return QString::number(QDateTime::currentMSecsSinceEpoch());
    }
}

QString BookmarkWorker::makeUUidStr()
{
    return QUuid::createUuid().toString().remove("{").remove("}");
}

void BookmarkWorker::loadFromFile()
{
    QElapsedTimer timer;
    timer.start();

    QFile file(file_path_);
    // 解析 Json 获取 Record
    if(!file.open(QIODevice::ReadOnly)){
        qInfo()<<__FUNCTION__<<"Can't open bookmark file:"<<file_path_;
        return;
    }
    QByteArray content = file.readAll();
    file.close();

    QJsonParseError json_error;
    QJsonDocument jsonDoc(QJsonDocument::fromJson(content, &json_error));

    if(json_error.error != QJsonParseError::NoError)
    {
        qDebug() << __FUNCTION__ <<"json error!";
        return;
    }
    QString checksum, version;
    QJsonObject root = jsonDoc.object();

    if(root.contains("checksum")){
        checksum = root.value("checksum").toString();
    }
    if(root.contains("roots"))
    {
        /*写数据时加锁*/
        QMutexLocker locker(&mutext_);

        QJsonObject obj_roots = root.value("roots").toObject();
        delete BookmarkMgr::Instance()->bkmk_toolbar_bkmks_;
        BookmarkMgr::Instance()->bkmk_toolbar_bkmks_ =
                parseNode(obj_roots.value("bookmark_bar").toObject());

        delete BookmarkMgr::Instance()->other_bkmks_;
        BookmarkMgr::Instance()->other_bkmks_ =
                parseNode(obj_roots.value("other").toObject());

        delete BookmarkMgr::Instance()->synced_bkmks_;
        BookmarkMgr::Instance()->synced_bkmks_ =
                parseNode(obj_roots.value("synced").toObject());
    }
    if(root.contains("version")){
        version = root.value("version").toString();
    }
    emit loadFinished();
    qInfo()<<__FUNCTION__<<":" << timer.elapsed() << "milliseconds";
}

bookmark::BookmarkNode *BookmarkWorker::parseNode(const QJsonObject &obj)
{
    bookmark::BookmarkNode *ret = nullptr;
    auto type = obj.value("type").toString();
    if(type == "folder"){
        auto folder = new bookmark::BookmarkFolder;
        folder->guid_ = obj.value("guid").toString();
        folder->id_ = obj.value("id").toString();
        folder->date_added_ = obj.value("date_added").toString();
        folder->name_ = obj.value("name").toString();
        folder->type_ = obj.value("type").toString();
        foreach(auto item , obj.value("children").toArray()){
            folder->children_.append(parseNode(item.toObject()));
        }
        ret = folder;
    }else if(type == "url"){
        auto url = new bookmark::BookmarkUrl;
        url->guid_ = obj.value("guid").toString();
        url->id_ = obj.value("id").toString();
        url->date_added_ = obj.value("date_added").toString();
        url->name_ = obj.value("name").toString();
        url->type_ = obj.value("type").toString();
        url->url_ = obj.value("url").toString();
        ret = url;
    }
    return ret;
}

QJsonObject BookmarkWorker::createObject(bookmark::BookmarkNode *node)
{
    QJsonObject obj;
    obj.insert("guid",       node->guid_);
    obj.insert("id",         node->id_);
    obj.insert("date_added", node->date_added_);
    obj.insert("name",       node->name_);
    obj.insert("type",       node->type_);
    if(node->type_ == "folder"){
        if(auto folder = dynamic_cast<bookmark::BookmarkFolder*>(node))
        {
            obj.insert("date_modified", folder->date_modified_);
            QJsonArray array;
            foreach(auto child , folder->children_){
                array.append(createObject(child));
            }
            obj.insert("children", array);
        }
    }else if(node->type_ == "url"){
        if(auto url = dynamic_cast<bookmark::BookmarkUrl*>(node))
            obj.insert("url", url->url_);
    }
    return obj;
}

void BookmarkWorker::saveToFile()
{
    QElapsedTimer timer;
    timer.start();

    QJsonObject docRoot, obj_roots, obj_bookmark_bar, obj_other, obj_synced;

    auto barNode = BookmarkMgr::Instance()->bkmk_toolbar_bkmks_;
    if(barNode){
        obj_bookmark_bar = createObject(barNode);
    }else{
        obj_bookmark_bar.insert("children",QJsonArray());
        obj_bookmark_bar.insert("date_added", makeEpochStr());
        obj_bookmark_bar.insert("date_modified", makeEpochStr());
        obj_bookmark_bar.insert("guid", makeUUidStr());
        obj_bookmark_bar.insert("id","1");
        obj_bookmark_bar.insert("name", QJsonValue(QStringLiteral("书签栏")));
        obj_bookmark_bar.insert("type","folder");
    }

    auto otherNode = BookmarkMgr::Instance()->other_bkmks_;
    if(otherNode){
        obj_other = createObject(otherNode);
    }else{
        obj_other.insert("children",QJsonArray());
        obj_other.insert("date_added", makeEpochStr());
        obj_other.insert("date_modified", makeEpochStr());
        obj_other.insert("guid", makeUUidStr());
        obj_other.insert("id","2");
        obj_other.insert("name", QJsonValue(QStringLiteral("其他书签")));
        obj_other.insert("type","folder");
    }
    auto syncedNode = BookmarkMgr::Instance()->synced_bkmks_;
    if(syncedNode){
        obj_synced = createObject(syncedNode);
    }else{
        obj_synced.insert("children",QJsonArray());
        obj_synced.insert("date_added", makeEpochStr());
        obj_synced.insert("date_modified", makeEpochStr());
        obj_synced.insert("guid", makeUUidStr());
        obj_synced.insert("id","3");
        obj_synced.insert("name", QJsonValue(QStringLiteral("移动设备书签")));
        obj_synced.insert("type","folder");
    }

    obj_roots.insert("bookmark_bar", obj_bookmark_bar);
    obj_roots.insert("other", obj_other);
    obj_roots.insert("synced", obj_synced);

    docRoot.insert("roots", obj_roots);
    docRoot.insert("checksum", "");
    docRoot.insert("version", "1");

    QJsonDocument jsonDoc;
    jsonDoc.setObject(docRoot);
    QFile file(R"(C:\Users\slamdd\Desktop\Bookmarks.json)");
    if(file.open(QIODevice::WriteOnly)){
        file.write(jsonDoc.toJson());
        file.close();
    }

    emit saveFinished();
    qInfo()<<__FUNCTION__<<":" << timer.elapsed() << "milliseconds";
}
