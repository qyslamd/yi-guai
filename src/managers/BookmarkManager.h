#ifndef BOOKMARKMANAGER_H
#define BOOKMARKMANAGER_H

#include <QtDebug>
#include <QMetaType>
#include <QObject>
#include <QMutex>
#include <QVector>
#include <QSet>
#include <QThread>

class QAction;
class BookmarkWorker;
class QStandardItem;
class QStandardItemModel;
class BookmarkMgr : public QObject
{
    Q_OBJECT
public:
    enum ItemDataType{
        Type = Qt::UserRole + 1,
        Name,
        Id,
        Guid,
        DateAdded,
        Url,
        DateModified
    };
    static BookmarkMgr* Instance();
    static QStandardItemModel *gBookmarkModel;
    static QSet<quint32> gIdSet;

    ~BookmarkMgr();
public:
    QAction *action_open_new_tab_ = nullptr;
    QAction *action_open_new_wnd_ = nullptr;
    QAction *action_open_in_private_ = nullptr;
    QAction *action_modify_ = nullptr;
    QAction *action_rename_;
    QAction *action_cut_;
    QAction *action_copy_;
    QAction *action_paste_;
    QAction *action_delete_;
    QAction *action_add_current_;
    QAction *action_add_folder_;
    QAction *action_show_bookmark_bar_;
    QAction *action_show_bookmakr_btn_;
    QAction *action_manage_bookmarks_;
private:
    explicit BookmarkMgr(QObject *parent = nullptr);
    BookmarkMgr(const BookmarkMgr& other);
    BookmarkMgr& operator=(const BookmarkMgr & other);
    void initActions();
signals:
    void load();
    void save();
    void bookmarksChanged();
private:
    static QMutex gMutex;
    static BookmarkMgr *gInst;

    struct Gc{
        ~Gc() {
            if(gInst){ delete gInst; gInst = nullptr;}
        }
    };
    QThread worker_thread_;
    BookmarkWorker *worker_ = nullptr;
private slots:
    void doLoadWork();
    void onWokerLoadFinished();
    void onWokerSaveFinished();
};

class BookmarkWorker : public QObject
{
    Q_OBJECT
public:
    BookmarkWorker();
    ~BookmarkWorker();

public slots:
    void loadFromFile();
    void saveToFile();
signals:
    void loadFinished();
    void saveFinished();

private:
    QMutex mutext_, mutext2_;
    QString file_path_;

    void createFileIfNotExist();
    QString makeEpochStr(bool msecond = false);
    QString makeUUidStr();

    QStandardItem* parseObj2Item(const QJsonObject &obj);
    QJsonObject paseItem2Obj(QStandardItem *item);
};

#endif // BOOKMARKMANAGER_H
