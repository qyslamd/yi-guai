#ifndef BOOKMARKMGR_H
#define BOOKMARKMGR_H

#include <QObject>
#include <QMutex>
#include <QVector>
#include <QJsonDocument>
#include <QThread>

namespace bookmark {
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
        qInfo()<<__FUNCTION__<<type_<<name_<<count;
    }
};
//Q_DECLARE_METATYPE(QVector<BookmarkNode *>);
// 上边这句是Qt提示让我写的，个人猜测是 在信号和槽中传递参数时需要这么注册参数
// 后边改了，不适用信号和槽传递参数了，这句就保留在这儿吧

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
}   // namespace bookmark

class BookmarkWorker;
class QStandardItem;
class QStandardItemModel;
class BookmarkMgr : public QObject
{
    Q_OBJECT
public:
    static BookmarkMgr* Instance();
    ~BookmarkMgr();
    static QStandardItemModel *gBookmarkModel_;
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
    friend class BookmarkWorker;
    bookmark::BookmarkNode *bkmk_toolbar_bkmks_ = nullptr;   /*书签栏工具栏书签*/
    bookmark::BookmarkNode *other_bkmks_ = nullptr; /*其它书签*/
    bookmark::BookmarkNode *synced_bkmks_ = nullptr;    /*同步的书签*/

    QThread worker_thread_;
    BookmarkWorker *worker_;
private slots:
    void onWokerLoadFinished();
    void onWokerSaveFinished();
    void onFaviconUpdated(const QString &urlDomain);
private:
    void updateModel();
    void parseNode2Item(QStandardItem *parent, const bookmark::BookmarkNode *node);
};

class BookmarkWorker : public QObject
{
    Q_OBJECT
public:
    explicit BookmarkWorker(QObject *parent = nullptr);
public slots:
    void loadFromFile();
    void saveToFile();
signals:
    void loadFinished();
    void saveFinished();

private:
    QMutex mutext_;
    QString file_path_;
    void createFileIfNotExist();
    QString makeEpochStr(bool msecond = false);
    QString makeUUidStr();

    bookmark::BookmarkNode* parseNode(const QJsonObject &obj);
    QJsonObject createObject(bookmark::BookmarkNode *node);
};

#endif // BOOKMARKMGR_H
