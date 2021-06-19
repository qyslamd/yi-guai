#include "BookmarkManager.h"

#include <QUrl>
#include <QDir>
#include <QUuid>
#include <QTimer>
#include <QThread>
#include <QAction>
#include <QJsonObject>
#include <QMutexLocker>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonParseError>
#include <QElapsedTimer>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QPushButton>

#include "utils/util_qt.h"
#include "FaviconManager.h"
#include "popups/StyledMenu.h"

BookmarkMgr* BookmarkMgr::gInst = nullptr;
QMutex BookmarkMgr::gMutex;
QStandardItemModel *BookmarkMgr::gBookmarkModel = nullptr;
ToolBarProviderWnd *BookmarkMgr::gProviderWidget = nullptr;
QSet<quint32> BookmarkMgr::gIdSet;

BookmarkMgr::BookmarkMgr(QObject *parent)
    : QObject(parent)
{
    initActions();
    qInfo()<<"\033[34m[Thread]"<<__FUNCTION__<<QThread::currentThreadId()<<"\033[0m";

    if(!gBookmarkModel){
        gBookmarkModel = new QStandardItemModel(this);
    }

    if(!gProviderWidget){
        gProviderWidget = new ToolBarProviderWnd;
        gProviderWidget->hide();
    }

    connect(this, &BookmarkMgr::bookmarksChanged, gProviderWidget, &ToolBarProviderWnd::onBookmarksChanged);

    worker_ = new BookmarkWorker;
    worker_->moveToThread(&worker_thread_);
//    connect(&worker_thread_, &QThread::finished, worker_, &BookmarkWorker::deleteLater);
    connect(&worker_thread_, &QThread::finished, this, [](){qInfo()<<"worker thread finished!";});
    connect(this, &BookmarkMgr::load, worker_, &BookmarkWorker::loadFromFile);
    connect(this, &BookmarkMgr::save, worker_, &BookmarkWorker::saveToFile);
    connect(worker_, &BookmarkWorker::loadFinished, this, &BookmarkMgr::onWokerLoadFinished);
    connect(worker_, &BookmarkWorker::saveFinished, this, &BookmarkMgr::onWokerSaveFinished);

    QTimer::singleShot(0, this, &BookmarkMgr::doLoadWork);
}

void BookmarkMgr::initActions()
{
    action_open_new_tab_ = new QAction(QIcon(), tr("open"), this);
    action_open_new_wnd_ = new QAction(QIcon(), tr("open in new window"), this);
    action_open_in_private_ = new QAction(QIcon(), tr("open in private window"), this);
    action_modify_ = new QAction(QIcon(), tr("modify"), this);
    action_rename_ = new QAction(QIcon(), tr("rename"), this);
    action_cut_ = new QAction(QIcon(), tr("cut"), this);
    action_copy_ = new QAction(QIcon(), tr("copy"), this);
    action_paste_ = new QAction(QIcon(), tr("paste"), this);
    action_delete_ = new QAction(QIcon(), tr("delete"), this);
    action_add_current_ = new QAction(QIcon(), tr("add current tab to favorite"), this);
    action_add_folder_ = new QAction(QIcon(), tr("add folder"), this);
    action_show_bookmark_bar_ = new QAction(QIcon(), tr("show bookmark bar"), this);
    action_show_bookmakr_btn_ = new QAction(QIcon(), tr("show bookmark button"), this);
    action_manage_bookmarks_ = new QAction(QIcon(), tr("open bookmark manager"), this);
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

    delete worker_;

    delete gProviderWidget;
}

void BookmarkMgr::doLoadWork()
{
    worker_thread_.start();
    emit load();
}

void BookmarkMgr::onWokerLoadFinished()
{
    loaded_ = true;
    worker_thread_.quit();
    worker_thread_.wait();
    emit bookmarksChanged();
}

void BookmarkMgr::onWokerSaveFinished()
{

}

BookmarkWorker::BookmarkWorker()
{
    qInfo()<<"\033[34m[Thread]"<<__FUNCTION__<<QThread::currentThreadId()<<"\033[0m";
    auto loc = UtilQt::appDataPath();
    file_path_ = QDir(loc).filePath("Bookmarks");
    if(!QFileInfo::exists(file_path_)){
        createFileIfNotExist();
    }
}

BookmarkWorker::~BookmarkWorker()
{
    qInfo()<<__FUNCTION__;
}

void BookmarkWorker::loadFromFile()
{
    qInfo()<<"\033[34m[Thread]"<<__FUNCTION__<<QThread::currentThreadId()<<"\033[0m";

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
        qDebug() << __FUNCTION__ <<"json error:"<<json_error.errorString();
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
    qInfo()<<"\033[32m[Execute Time]"<<__FUNCTION__<<":" << timer.elapsed() << "ms"<<"\033[0m";
    qInfo()<<__FUNCTION__<<"count:"<<BookmarkMgr::gIdSet.count();
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
    qInfo()<<"\033[32m[Execute Time]"<<__FUNCTION__<<":" << timer.elapsed() << "ms"<<"\033[0m";
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
    auto type = obj.value("type").toString();
    auto id = obj.value("id").toString();
    BookmarkMgr::gIdSet.insert(id.toUInt());

    item->setData(type, BookmarkMgr::Type);
    item->setData(obj.value("guid").toString(), BookmarkMgr::Guid);
    item->setData(id, BookmarkMgr::Id);
    item->setData(obj.value("date_added").toString(), BookmarkMgr::DateAdded);
    item->setData(obj.value("name").toString(), BookmarkMgr::Name);

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

ToolBarProviderWnd::ToolBarProviderWnd(QWidget *parent)
    : QWidget (parent)
{

}

ToolBarProviderWnd::~ToolBarProviderWnd()
{
    qInfo()<<__FUNCTION__;
}

void ToolBarProviderWnd::onBookmarksChanged()
{
    QElapsedTimer timer;
    timer.start();

    auto model = BookmarkMgr::Instance()->gBookmarkModel;
    auto barItem = model->item(0);
    if(barItem){
        for (int i = 0; i< barItem->rowCount(); i++)
        {
            auto child = barItem->child(i);
            if(child){
                auto type = child->data(BookmarkMgr::Type).toString();
                auto name = child->data(BookmarkMgr::Name).toString();
                auto icon = type=="folder"?FaviconMgr::systemDirIcon:FaviconMgr::systemFileIcon;
                auto action = new QAction(icon, name, this);
                action->setText(UtilQt::getElideText(action->text(),action->font(),178));
                buttons.append(action);

                if(type == "folder"){
                    action->setMenu(makeMenu(child));
                }else if(type == "url"){
                    auto url = child->data(BookmarkMgr::Url).toString();
                    auto text1 = UtilQt::getElideText(name,action->font(), 470);
                    auto text2 = UtilQt::getElideText(QUrl(url).toDisplayString(),
                                                         action->font(),
                                                         470);
                    action->setToolTip(text1 + "\n" + text2);
//                    connect(action, &QAction::clicked, [=](){
//                        emit cmdTriggered(BookmarkCmd::Open, url);
//                    });
                }
                action->setData(QVariant::fromValue<void *>(child));
            }
        }
//        label_empty_->setVisible(barItem->rowCount() == 0);
    }
    auto otherItem = model->item(1);
    if(otherItem){
        others_menu_ = makeMenu(otherItem);
    }

//    loaded_ = true;

    emit loadToUiFinished();
    qInfo()<<"\033[32m[Execute Time]"<<__FUNCTION__<<":" << timer.elapsed() << "ms"<<"\033[0m";
}

BookmarkMenu *ToolBarProviderWnd::makeMenu(const QStandardItem *item)
{
    auto name = item->data(BookmarkMgr::Name).toString();
    BookmarkMenu *menu = new BookmarkMenu(name, this);
    for (int i = 0; i< item->rowCount(); i++){
        auto child = item->child(i);
        if(child){
            auto type = child->data(BookmarkMgr::Type).toString();
            auto name = child->data(BookmarkMgr::Name).toString();
            auto icon = type=="folder"?FaviconMgr::systemDirIcon:FaviconMgr::systemFileIcon;
            auto action = new QAction(icon, name, this);
            menu->addAction(action);
            if(type == "folder"){
                action->setMenu(makeMenu(child));
            }else if(type == "url"){
//                connect(action, &QAction::triggered,[=](){
//                    if(auto dataItem = (QStandardItem *)action->data().value<void *>()){
//                        emit cmdTriggered(BookmarkCmd::Open, dataItem->data(BookmarkMgr::Url));
//                    }
//                });
                auto url = child->data(BookmarkMgr::Url).toString();
                action->setToolTip(name + "\n" + url);
            }
            action->setData(QVariant::fromValue<void *>(child));
        }
    }
    return menu;
}

BookmarkMenu::BookmarkMenu(const QString &title, QWidget *parent)
    : QMenu(title, parent)
{
    initUi();
}

BookmarkMenu::BookmarkMenu(QWidget *parent)
    : QMenu( parent)
{
    initUi();
}

void BookmarkMenu::initUi()
{
    setWindowFlag(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setAutoFillBackground(true);
    setContextMenuPolicy(Qt::CustomContextMenu);
    setMaximumWidth(560);
    setToolTipsVisible(true);

    connect(this, &BookmarkMenu::customContextMenuRequested,
            this, &BookmarkMenu::onCustomContextMenuRequested);
}

void BookmarkMenu::onCustomContextMenuRequested(const QPoint &pos)
{
    static StyledMenu menu;
    menu.clear();
    auto action = activeAction();
    // without menu
    if(action){
        if(action->isSeparator()){
            return;
        }
    }else{
        action = menuAction();
    }
    if(auto dataItem = (QStandardItem *)action->data().value<void *>()){
        auto type = dataItem->data(BookmarkMgr::Type).toString();
        auto openAction = BookmarkMgr::Instance()->action_open_new_tab_;
        auto openWndAction = BookmarkMgr::Instance()->action_open_new_wnd_;
        auto openPrivateAction = BookmarkMgr::Instance()->action_open_in_private_;

        if(type == "folder"){
            openAction->setText(tr("open all") + QString("(%1)").arg(dataItem->rowCount()));
            openWndAction->setText(tr("open all in new window") + QString("(%1)").arg(dataItem->rowCount()));
            openPrivateAction->setText(tr("open all in private window") + QString("(%1)").arg(dataItem->rowCount()));
            menu.addAction(openAction);
            menu.addAction(openWndAction);
            menu.addAction(openPrivateAction);
            menu.addSeparator();
            menu.addAction(BookmarkMgr::Instance()->action_rename_);

        }else if(type == "url"){
            openAction->setText(tr("open") );
            openWndAction->setText(tr("open in new window"));
            openPrivateAction->setText(tr("open in private window"));
            menu.addAction(openAction);
            menu.addAction(openWndAction);
            menu.addAction(openPrivateAction);
            menu.addSeparator();
            menu.addAction(BookmarkMgr::Instance()->action_modify_);
        }
    }
    menu.addSeparator();
    menu.addAction(BookmarkMgr::Instance()->action_cut_);
    menu.addAction(BookmarkMgr::Instance()->action_copy_);
    menu.addAction(BookmarkMgr::Instance()->action_paste_);
    menu.addSeparator();
    menu.addAction(BookmarkMgr::Instance()->action_delete_);
    menu.addSeparator();
    menu.addAction(BookmarkMgr::Instance()->action_add_current_);
    menu.addAction(BookmarkMgr::Instance()->action_add_folder_);
    menu.addSeparator();
    menu.addAction(BookmarkMgr::Instance()->action_show_bookmark_bar_);
    menu.addAction(BookmarkMgr::Instance()->action_show_bookmakr_btn_);
    menu.addAction(BookmarkMgr::Instance()->action_manage_bookmarks_);
    menu.exec(QCursor::pos());
}
