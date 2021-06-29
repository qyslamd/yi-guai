#include "BookmarkManager.h"

#include <QUrl>
#include <QDir>
#include <QUuid>
#include <QTimer>
#include <QThread>
#include <QJsonObject>
#include <QMutexLocker>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonParseError>
#include <QElapsedTimer>
#include <QStandardItem>
#include <QStandardItemModel>

#include "utils/util_qt.h"
#include "FaviconManager.h"


int BookmarkNode::count = 0;
int BookmarkUrl::url_count = 0;
int BookmarkFolder::folder_count = 0;


BookmarkMgr* BookmarkMgr::gInst = nullptr;
QMutex BookmarkMgr::gMutex;
QStandardItemModel *BookmarkMgr::gBookmarkModel = nullptr;

BookmarkMgr::BookmarkMgr(QObject *parent)
    : QObject(parent)
{
    if(!gBookmarkModel){
        gBookmarkModel = new QStandardItemModel(this);
    }
    connect(&FaviconMgr::Instance(), &FaviconMgr::iconUpdated, this, &BookmarkMgr::onFaviconUpdated);

    worker_ = new BookmarkWorker;
    worker_->moveToThread(&worker_thread_);
    connect(&worker_thread_, &QThread::finished, worker_, &BookmarkWorker::deleteLater);
    connect(&worker_thread_, &QThread::finished, this, [](){qInfo()<<"worker thread finished!";});
    connect(this, &BookmarkMgr::load, worker_, &BookmarkWorker::loadFromFile);
    connect(this, &BookmarkMgr::save, worker_, &BookmarkWorker::saveToFile);
    connect(worker_, &BookmarkWorker::loadFinished, this, &BookmarkMgr::onWokerLoadFinished);
    connect(worker_, &BookmarkWorker::saveFinished, this, &BookmarkMgr::onWokerSaveFinished);
    worker_thread_.start();

    QTimer::singleShot(1, this, &BookmarkMgr::load);
}

void BookmarkMgr::onWokerLoadFinished()
{
    qInfo()<<__FUNCTION__<<"total-count:"<<BookmarkNode::count
          <<"folder-count:"<<BookmarkFolder::folder_count
         <<"url-count:"<<BookmarkUrl::url_count;
    emit bookmarksChanged();
}

void BookmarkMgr::onWokerSaveFinished()
{
    qInfo()<<__FUNCTION__;
}

void BookmarkMgr::onFaviconUpdated(const QString &urlDomain)
{
    auto rows = gBookmarkModel->rowCount();
    auto columns = gBookmarkModel->columnCount();
    qInfo()<<__FUNCTION__<<rows<<columns;
    return;
    for(int i = 0; i < rows; i++){
        for(int j = 0; j < columns; j++){
            auto item = gBookmarkModel->item(i, j);
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

void BookmarkWorker::updateModel()
{
    QElapsedTimer timer;
    timer.start();

    qRegisterMetaType<QVector<int>>();

    QMutexLocker locker(&mutext2_);
    BookmarkMgr::gBookmarkModel->clear();

    QStandardItem *barItem = new QStandardItem(FaviconMgr::systemDirIcon,
                                               bkmk_toolbar_node_->name_);
    barItem->setData(QVariant::fromValue((void *)bkmk_toolbar_node_),
                     Qt::UserRole + 1);
    if(auto node = dynamic_cast<const BookmarkFolder*>(bkmk_toolbar_node_)){
        for(auto child : node->children_){
            parseNode2Item(barItem, child);
        }
    }
    BookmarkMgr::gBookmarkModel->appendRow(barItem);


    QStandardItem *otherItem = new QStandardItem(FaviconMgr::systemDirIcon,
                                                 other_node_->name_);
    barItem->setData(QVariant::fromValue((void *)other_node_), Qt::UserRole + 1);
    if(auto node = dynamic_cast<const BookmarkFolder*>(other_node_)){
        for(auto child : node->children_){
            parseNode2Item(otherItem, child);
        }
    }
    BookmarkMgr::gBookmarkModel->appendRow(otherItem);

    QStandardItem *syncedItem = new QStandardItem(FaviconMgr::systemDirIcon,
                                                  synced_node_->name_);
    barItem->setData(QVariant::fromValue((void *)syncedItem), Qt::UserRole + 1);
    if(auto node = dynamic_cast<const BookmarkFolder*>(synced_node_)){
        for(auto child : node->children_){
            parseNode2Item(syncedItem, child);
        }
    }
    BookmarkMgr::gBookmarkModel->appendRow(syncedItem);
    qInfo()<<"\033[32m[Time:]"<<__FUNCTION__<<":" << timer.elapsed() << "ms"<<"\033[0m";
}

void BookmarkWorker::parseNode2Item(QStandardItem *parent, const BookmarkNode *node)
{
    QStandardItem *item = new QStandardItem(FaviconMgr::systemDirIcon,
                                            node->name_);
    item->setData(QVariant::fromValue((void *)node), Qt::UserRole + 1);
    if(node->type_ == "folder"){
        if(auto folder = dynamic_cast<const BookmarkFolder*>(node)){
            for(auto child : folder->children_){
                parseNode2Item(item, child);
            }
        }
    }else if(node->type_ == "url"){
        if(auto url = dynamic_cast<const BookmarkUrl*>(node)){
            QIcon icon(FaviconMgr::Instance().iconFilePath(url->url_));
            if(icon.isNull()){
                icon = FaviconMgr::systemFileIcon;
            }
            item->setIcon(icon);
        }
    }

    parent->appendRow(item);
}

BookmarkMgr::~BookmarkMgr()
{
    qInfo()<<__FUNCTION__;
    worker_thread_.quit();
    worker_thread_.wait();

    delete worker_;
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

BookmarkWorker::BookmarkWorker()
{
    auto loc = UtilQt::appDataPath();
    file_path_ = QDir(loc).filePath("Bookmarks");
    if(!QFileInfo::exists(file_path_)){
        createFileIfNotExist();
    }
}

BookmarkWorker::~BookmarkWorker()
{
    /*delete 一个nullptr是合法的*/
    delete bkmk_toolbar_node_;
    delete other_node_;
    delete synced_node_;
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
        delete bkmk_toolbar_node_;
        bkmk_toolbar_node_ = parseNode(obj_roots.value("bookmark_bar").toObject());

        delete other_node_;
        other_node_ = parseNode(obj_roots.value("other").toObject());

        delete synced_node_;
        synced_node_ = parseNode(obj_roots.value("synced").toObject());
    }
    if(root.contains("version")){
        version = root.value("version").toString();
    }
    qInfo()<<"\033[32m[Time:]"<<__FUNCTION__<<":" << timer.elapsed() << "ms"<<"\033[0m";
    updateModel();
    emit loadFinished();
}

BookmarkNode *BookmarkWorker::parseNode(const QJsonObject &obj)
{
    BookmarkNode *ret = nullptr;
    auto type = obj.value("type").toString();
    if(type == "folder"){
        auto folder = new BookmarkFolder;
        folder->guid_ = obj.value("guid").toString();
        folder->id_ = obj.value("id").toString();
        folder->date_added_ = obj.value("date_added").toString();
        folder->name_ = obj.value("name").toString();
        folder->type_ = obj.value("type").toString();
        folder->date_modified_ = obj.value("date_modified").toString();
        foreach(auto item , obj.value("children").toArray()){
            folder->children_.append(parseNode(item.toObject()));
        }
        ret = folder;
    }else if(type == "url"){
        auto url = new BookmarkUrl;
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

QJsonObject BookmarkWorker::createObject(BookmarkNode *node)
{
    QJsonObject obj;
    obj.insert("guid",       node->guid_);
    obj.insert("id",         node->id_);
    obj.insert("date_added", node->date_added_);
    obj.insert("name",       node->name_);
    obj.insert("type",       node->type_);
    if(node->type_ == "folder"){
        if(auto folder = dynamic_cast<BookmarkFolder*>(node))
        {
            obj.insert("date_modified", folder->date_modified_);
            QJsonArray array;
            foreach(auto child , folder->children_){
                array.append(createObject(child));
            }
            obj.insert("children", array);
        }
    }else if(node->type_ == "url"){
        if(auto url = dynamic_cast<BookmarkUrl*>(node))
            obj.insert("url", url->url_);
    }
    return obj;
}

void BookmarkWorker::saveToFile()
{
    QElapsedTimer timer;
    timer.start();

    QJsonObject docRoot, obj_roots, obj_bookmark_bar, obj_other, obj_synced;

    if(bkmk_toolbar_node_){
        obj_bookmark_bar = createObject(bkmk_toolbar_node_);
    }else{
        obj_bookmark_bar.insert("children",QJsonArray());
        obj_bookmark_bar.insert("date_added", makeEpochStr());
        obj_bookmark_bar.insert("date_modified", makeEpochStr());
        obj_bookmark_bar.insert("guid", makeUUidStr());
        obj_bookmark_bar.insert("id","1");
        obj_bookmark_bar.insert("name", QJsonValue(QStringLiteral("书签栏")));
        obj_bookmark_bar.insert("type","folder");
    }

    if(other_node_){
        obj_other = createObject(other_node_);
    }else{
        obj_other.insert("children",QJsonArray());
        obj_other.insert("date_added", makeEpochStr());
        obj_other.insert("date_modified", makeEpochStr());
        obj_other.insert("guid", makeUUidStr());
        obj_other.insert("id","2");
        obj_other.insert("name", QJsonValue(QStringLiteral("其他书签")));
        obj_other.insert("type","folder");
    }

    if(synced_node_){
        obj_synced = createObject(synced_node_);
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
    qInfo()<<"\033[32m[Time:]"<<__FUNCTION__<<":" << timer.elapsed() << "ms"<<"\033[0m";
}
