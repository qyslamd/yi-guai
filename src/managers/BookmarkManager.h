#ifndef BOOKMARKMGR_H
#define BOOKMARKMGR_H

#include <QObject>
#include <QMutex>
#include <QVector>
#include <QJsonDocument>
#include <QThread>


#include "globaldef.h"

class BookmarkWorker;
class BookmarkMgr : public QObject
{
    Q_OBJECT
public:
    static BookmarkMgr* Instance();
    ~BookmarkMgr();
    const BookmarkNode * bookmarkBarNodes();
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
    BookmarkNode *bkmk_toolbar_bkmks_ = nullptr;   /*书签栏工具栏书签*/
    BookmarkNode *other_bkmks_ = nullptr; /*其它书签*/
    BookmarkNode *synced_bkmks_ = nullptr;    /*同步的书签*/

    QThread worker_thread_;
    BookmarkWorker *worker_;
private slots:
    void onWokerLoadFinished();
    void onWokerSaveFinished();
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

    BookmarkNode* parseNode(const QJsonObject &obj);
    QJsonObject createObject(BookmarkNode *node);
};

#endif // BOOKMARKMGR_H
