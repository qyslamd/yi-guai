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

BookmarkMgr* BookmarkMgr::gInst = nullptr;
QMutex BookmarkMgr::gMutex;
QStandardItemModel *BookmarkMgr::gBookmarkModel = nullptr;

BookmarkMgr::BookmarkMgr(QObject *parent)
    : QObject(parent)
{
    if(!gBookmarkModel){
        gBookmarkModel = new QStandardItemModel(this);
    }

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

BookmarkMgr::~BookmarkMgr()
{
    qInfo()<<__FUNCTION__;
    worker_thread_.quit();
    worker_thread_.wait();
}

void BookmarkMgr::onWokerLoadFinished()
{
    emit bookmarksChanged();
}

void BookmarkMgr::onWokerSaveFinished()
{

}

int BookmarkWorker::count = 0;
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
        BookmarkMgr::gBookmarkModel->clear();

        QJsonObject obj_roots = root.value("roots").toObject();

        BookmarkMgr::gBookmarkModel->appendRow(parseObj2Item(obj_roots.value("bookmark_bar").toObject()));
        BookmarkMgr::gBookmarkModel->appendRow(parseObj2Item(obj_roots.value("other").toObject()));
        BookmarkMgr::gBookmarkModel->appendRow(parseObj2Item(obj_roots.value("synced").toObject()));
    }
    if(root.contains("version")){
        version = root.value("version").toString();
    }
    qInfo()<<"\033[32m[Time:]"<<__FUNCTION__<<":" << timer.elapsed() << "ms"<<"\033[0m";
    qInfo()<<__FUNCTION__<<"count:"<<count;
    emit loadFinished();
}

void BookmarkWorker::saveToFile()
{
    QElapsedTimer timer;
    timer.start();

    QJsonObject docRoot, obj_roots, obj_bookmark_bar, obj_other, obj_synced;

    auto model = BookmarkMgr::gBookmarkModel;
    auto toolBarItem = model->item(0, 0);
    if(toolBarItem){
        obj_bookmark_bar = paseItem2Obj(toolBarItem);
    }else{
        obj_bookmark_bar.insert("children",QJsonArray());
        obj_bookmark_bar.insert("date_added", makeEpochStr());
        obj_bookmark_bar.insert("date_modified", makeEpochStr());
        obj_bookmark_bar.insert("guid", makeUUidStr());
        obj_bookmark_bar.insert("id","1");
        obj_bookmark_bar.insert("name", QJsonValue(QStringLiteral("书签栏")));
        obj_bookmark_bar.insert("type","folder");
    }

    auto otherItem = model->item(1, 0);
    if(otherItem){
        obj_other = paseItem2Obj(otherItem);
    }else{
        obj_other.insert("children",QJsonArray());
        obj_other.insert("date_added", makeEpochStr());
        obj_other.insert("date_modified", makeEpochStr());
        obj_other.insert("guid", makeUUidStr());
        obj_other.insert("id","2");
        obj_other.insert("name", QJsonValue(QStringLiteral("其他书签")));
        obj_other.insert("type","folder");
    }

    auto syncedItem = model->item(2, 0);
    if(syncedItem){
        obj_synced = paseItem2Obj(syncedItem);
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

QStandardItem *BookmarkWorker::parseObj2Item(const QJsonObject &obj)
{
    QStandardItem *item = new QStandardItem(obj.value("name").toString());
    item->setData(obj.value("type").toString(), BookmarkMgr::Type);
    item->setData( obj.value("guid").toString(), BookmarkMgr::Guid);
    item->setData(obj.value("id").toString(), BookmarkMgr::Id);
    item->setData(obj.value("date_added").toString(), BookmarkMgr::DateAdded);
    item->setData(obj.value("name").toString(), BookmarkMgr::Name);
    auto type = obj.value("type").toString();
    if(type == "folder"){
        item->setIcon(FaviconMgr::systemDirIcon);
        item->setData(obj.value("date_modified").toString(), BookmarkMgr::DateModified);
        foreach(auto child , obj.value("children").toArray()){
            item->appendRow(parseObj2Item(child.toObject()));
        }
    }else if(type == "url"){
        auto url = obj.value("url").toString();
        item->setIcon(FaviconMgr::systemFileIcon);
        item->setData(url, BookmarkMgr::Url);
    }
    count++;
    return item;
}

QJsonObject BookmarkWorker::paseItem2Obj(QStandardItem *item)
{
    QJsonObject obj;
    obj.insert("guid",item->data(BookmarkMgr::Guid).toString());
    obj.insert("id", item->data(BookmarkMgr::Id).toString());
    obj.insert("date_added", item->data(BookmarkMgr::DateAdded).toString());
    obj.insert("name", item->data(BookmarkMgr::Name).toString());
    obj.insert("type", item->data(BookmarkMgr::Type).toString());

    auto type = item->data(BookmarkMgr::Type).toString();
    if(type == "folder"){
        QJsonArray array;
        for(int i = 0; i < item->rowCount(); i ++){
            if(item->child(i)){
                array.append(paseItem2Obj(item->child(i)));
            }
        }
        obj.insert("children", array);
    }else if(type == "url"){
        obj.insert("url", item->data(BookmarkMgr::Url).toString());
    }
    return obj;
}
