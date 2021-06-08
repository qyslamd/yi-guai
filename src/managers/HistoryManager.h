#ifndef HISTORYMANAGER_H
#define HISTORYMANAGER_H

#include <QObject>
#include <QList>
#include <memory>

#include "globaldef.h"

class HistoryMgr : public QObject
{
    Q_OBJECT
    explicit HistoryMgr(QObject *parent = nullptr);
    HistoryMgr(const HistoryMgr& other);
    HistoryMgr& operator=(const HistoryMgr & other);
public:
    static HistoryMgr& Instance();
    ~HistoryMgr();

    void addHistoryRecord(const History &data);
    QList<History> allHistories();
private:
    QString record_file_path_;
    QList<History> histories_cache_;

    void loadHistories();
    void saveToFile();
};

#endif // HISTORYMANAGER_H
