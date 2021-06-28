#ifndef BOOKMARKMGR_H
#define BOOKMARKMGR_H

#include <QtDebug>
#include <QMetaType>
#include <QObject>
#include <QMutex>
#include <QVector>
#include <QThread>

struct BookmarkNode{
    static int count;

    QString guid_;
    QString id_;
    QString date_added_;
    QString name_;
    QString type_;

    BookmarkNode()
    {
        count++;
    }
    virtual ~BookmarkNode(){
        count--;
        if(count==0){
            qInfo()<<__FUNCTION__<<"All items deleted";
        }
    }
};

struct BookmarkUrl : public BookmarkNode{
    static int url_count;

    QString url_;

    BookmarkUrl()
    {
        url_count++;
    }
    ~BookmarkUrl()
    {
        url_count--;
    }
};

struct BookmarkFolder : public BookmarkNode{
    static int folder_count;

    QVector<BookmarkNode*> children_;
    QString date_modified_;

    BookmarkFolder()
    {
        folder_count++;
    }
    ~BookmarkFolder(){
        folder_count--;
        while(!children_.isEmpty())
        {
            auto item = children_.first();
            if(item){
                delete item;
                item = nullptr;
            }
            children_.removeFirst();
        }
    }
};

class BookmarkWorker;
class QStandardItem;
class QStandardItemModel;
class BookmarkMgr : public QObject
{
    Q_OBJECT
public:
    static BookmarkMgr* Instance();
    static QStandardItemModel *gBookmarkModel;

    ~BookmarkMgr();
signals:
    void load();
    void save();
    void bookmarksChanged();
private:
    explicit BookmarkMgr(QObject *parent = nullptr);
    BookmarkMgr(const BookmarkMgr& other);
    BookmarkMgr& operator=(const BookmarkMgr & other);

    static QMutex gMutex;
    static BookmarkMgr *gInst;

    struct Gc{
        ~Gc() {
            if(gInst){ delete gInst; gInst = nullptr;}
        }
    };
private:
    QThread worker_thread_;
    BookmarkWorker *worker_ = nullptr;
private slots:
    void onWokerLoadFinished();
    void onWokerSaveFinished();
    void onFaviconUpdated(const QString &urlDomain);
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
    BookmarkNode *bkmk_toolbar_node_ = nullptr;   /*书签栏工具栏书签*/
    BookmarkNode *other_node_ = nullptr;          /*其它书签*/
    BookmarkNode *synced_node_ = nullptr;         /*同步的书签*/

    QMutex mutext_, mutext2_;
    QString file_path_;
    void createFileIfNotExist();
    QString makeEpochStr(bool msecond = false);
    QString makeUUidStr();

    BookmarkNode* parseNode(const QJsonObject &obj);
    QJsonObject createObject(BookmarkNode *node);
private:
    void updateModel();
    void parseNode2Item(QStandardItem *parent, const BookmarkNode *node);
};

#endif // BOOKMARKMGR_H
