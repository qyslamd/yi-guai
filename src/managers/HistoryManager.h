#ifndef HISTORYMANAGER_H
#define HISTORYMANAGER_H

#include <QObject>
#include <QList>
#include <QStack>
#include <memory>
#include <QMutex>
#include <QThread>

#include "globaldef.h"

class QStandardItem;
class QStandardItemModel;
class HistoryWorker;
class HistoryMgr : public QObject
{
    Q_OBJECT
public:
    enum ItemDataType{
        LastTime = Qt::UserRole + 1,
        Url,
        Title,
        Count
    };

    static QStandardItemModel *gHistoryModel;
    static QStack<History> RecentlyHistory;
    static HistoryMgr* Instance();
    ~HistoryMgr();

    bool isLoaded() const{return loaded_;}
    void addHistoryRecord(const History &data);
signals:
    void load();
    void save();
    void historyChanged();
private:
    explicit HistoryMgr(QObject *parent = nullptr);
    HistoryMgr(const HistoryMgr& other);
    HistoryMgr& operator=(const HistoryMgr & other);

    static HistoryMgr *gInst;
    static QMutex gMutex;
    struct Gc{
        ~Gc(){if(gInst){ delete gInst; gInst = nullptr; } }
    };
    QThread worker_thread_;
    HistoryWorker *worker_;
    bool loaded_ = false;
    QList<History> pending_list_;
private slots:
    void doLoadWork();
    void doSaveWork();
    void onWorkerLoadFinished();
    void onWorkerSaveFinished();
};

class HistoryWorker : public QObject
{
    Q_OBJECT
public:
    HistoryWorker();
    ~HistoryWorker();
public slots:
    void loadFromFile();
    void saveToFile();
signals:
    void loadFinished();
    void saveFinished();

private:
    QString record_file_path_;

};

#endif // HISTORYMANAGER_H
