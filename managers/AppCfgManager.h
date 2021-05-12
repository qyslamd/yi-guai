#ifndef APPCFGMANAGER_H
#define APPCFGMANAGER_H

#include <QObject>
#include <QtDebug>

class QSettings;

class AppCfgMgr : public QObject
{
    Q_OBJECT
    explicit AppCfgMgr(QObject *parent = nullptr);
    AppCfgMgr(const AppCfgMgr &);
    AppCfgMgr& operator=(const AppCfgMgr &);
public:
    static AppCfgMgr& instance(){
        static AppCfgMgr inst;
        return inst;
    }
    static QByteArray windowGeometry();
    static void setWindowGeometry(const QByteArray &data);

    ~AppCfgMgr() {
        qInfo()<<__FUNCTION__;
    }
signals:
    void configChanged();
private:
    QSettings *settings_ = nullptr;

    QVariant value(const QString &group,
                   const QString &key,
                   const QVariant &defualt);
    QVariant value(const QString &key,
                   const QVariant &defualt = QVariant());

    void setValue(const QString &group,
                  const QString &key,
                  const QVariant &data);
    void setValue(const QString &key,
                  const QVariant &data);
};

#endif // APPCFGMANAGER_H
