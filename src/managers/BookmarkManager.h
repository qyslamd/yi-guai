#ifndef BOOKMARKMANAGER_H
#define BOOKMARKMANAGER_H

#include <QtDebug>
#include <QMetaType>
#include <QObject>
#include <QMutex>
#include <QVector>
#include <QThread>

class BookmarkWorker;
class QStandardItem;
class QStandardItemModel;
class BookmarkMgr : public QObject
{
    Q_OBJECT
public:
    enum ModelDataType{
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

    ~BookmarkMgr();
private:
    explicit BookmarkMgr(QObject *parent = nullptr);
    BookmarkMgr(const BookmarkMgr& other);
    BookmarkMgr& operator=(const BookmarkMgr & other);

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
