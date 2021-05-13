#ifndef ADDRINPUTMANAGER_H
#define ADDRINPUTMANAGER_H

#include <QObject>
#include <QStringList>

class AddrInputMgr : public QObject
{
    Q_OBJECT
    explicit AddrInputMgr(QObject *parent = nullptr);
    AddrInputMgr(const AddrInputMgr& other);
    AddrInputMgr& operator=(const AddrInputMgr & other);
public:
    static AddrInputMgr& Instance();

    ~AddrInputMgr();
    QStringList inputList() const;
    void addRecord(const QString &data);
    void addRecords(const QStringList &data);
signals:

private:
    QString file_path_;
    QStringList cache_;
    void loadToCache();
    void saveToFile();
};

#endif // ADDRINPUTMANAGER_H
