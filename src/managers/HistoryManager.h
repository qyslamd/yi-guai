#ifndef HISTORYMANAGER_H
#define HISTORYMANAGER_H

#include <QObject>
#include <QList>
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
    static HistoryMgr* Instance();
    ~HistoryMgr();

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

    bool exist(const QString &url);

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
