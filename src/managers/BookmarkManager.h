#ifndef BOOKMARKMANAGER_H
#define BOOKMARKMANAGER_H

#include <QtDebug>
#include <QMetaType>
#include <QObject>
#include <QMutex>
#include <QVector>
#include <QSet>
#include <QThread>
#include <QWidget>
#include <QMenu>
#include <QModelIndex>

#include "globaldef.h"

class QAction;
class BookmarkWorker;
class QStandardItem;
class QStandardItemModel;
class ToolBarProviderWnd;
///
/// \brief The BookmarkMgr class
/// 书签管理类，负责书签文件的读写和同步
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
    ~BookmarkMgr();
    static BookmarkMgr* Instance();
    static bool exists(const QString &url);
    static QStandardItemModel *gBookmarkModel;
    static ToolBarProviderWnd *gToolbarProvider;
    static QSet<quint32> gIdSet;

    QStandardItem *addBookmarkUrl(const QModelIndex index, const QString &url, const QString &title);
    QStandardItem * addBookmarkFolder(const QModelIndex index, const QString &name);
    bool isLoaded() const{return loaded_;}
    void setMenuTriggerItem(QStandardItem *item);
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
    QAction *action_add_all_;
    QAction *action_add_folder_;
    QAction *action_import_bookmarks_;
    QAction *action_export_bookmarks_;
    QAction *action_delete_duplicate_;
    QAction *action_show_bookmark_bar_;
    QAction *action_show_bookmark_btn_;
    QAction *action_manage_bookmarks_;
private:
    explicit BookmarkMgr(QObject *parent = nullptr);
    BookmarkMgr(const BookmarkMgr& other);
    BookmarkMgr& operator=(const BookmarkMgr & other);
    static QMutex gMutex;
    static BookmarkMgr *gInst;

    struct Gc{
        ~Gc(){if(gInst){ delete gInst; gInst = nullptr; } }
    };
    QThread worker_thread_;
    BookmarkWorker *worker_ = nullptr;

    bool loaded_ = false;
    QStandardItem *menu_trigger_item_ = nullptr;
    void initActions();
signals:
    void load();
    void save();
    void loadFinished();
    void bookmarksChanged();
    void menuCmd(BookmarkCmd cmd,  const QVariant &para);
private slots:
    void doLoadWork();
    void doSaveWork();
    void onWokerLoadFinished();
    void onWokerSaveFinished();
};

///
/// \brief The BookmarkWorker class
/// 书签文件读写线程的具体工作类
/// 读文件，并创建好具体的 QStandardItemModel
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

    QStandardItem* parseObj2Item(const QJsonObject &obj);
    QJsonObject paseItem2Obj(QStandardItem *item);
};

class QPushButton;
class BookmarkMenu;
///
/// \brief The ToolBarProviderWnd class
/// 创建一个QWidget来承载书签工具栏的所有QAction和QMenu
/// 在多个顶层窗口之间共享
/// 此窗口是典型的“工具人”
class ToolBarProviderWnd: public QWidget{
    Q_OBJECT
public:
    explicit ToolBarProviderWnd(QWidget *parent = nullptr);
    ~ToolBarProviderWnd();

    QList<QAction *> buttons;
    QMenu *others_menu_;
    bool  loaded() const {return loaded_;}

signals:
    void menuActionTriggered(const QVariant &data);
    void loadToUiFinished();
public slots:
    void onBookmarksLoaded();

private:
    bool loaded_ = false;
    QList<QMenu *> added_menu_;
    BookmarkMenu *makeMenu(const QStandardItem *item);
};

class BookmarkMenu : public QMenu
{
    Q_OBJECT
public:
    BookmarkMenu(const QString &title, QWidget *parent = nullptr);
    BookmarkMenu(QWidget *parent = nullptr);

private:
    void initUi();

private slots:
    void onCustomContextMenuRequested(const QPoint &pos);
};

#endif // BOOKMARKMANAGER_H
