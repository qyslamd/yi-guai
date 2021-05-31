#ifndef APPCFGMANAGER_H
#define APPCFGMANAGER_H

#include <QObject>
#include <QtDebug>

class QSettings;

class AppCfgMgr : public QObject
{
    Q_OBJECT
public:
    static QByteArray windowGeometry();
    static void setWindowGeometry(const QByteArray &data);

    static QString homePageUrl();
    static void setHomePageUrl(const QString &data);

    static QString newTabPageUrl();
    static void setNewTabPageUrl(const QString &data);

    static QByteArray devToolGeometry();
    static void setDevToolGeometry(const QByteArray &data);

public:
    static AppCfgMgr& instance(){
        static AppCfgMgr inst;
        return inst;
    }
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

    explicit AppCfgMgr(QObject *parent = nullptr);
    AppCfgMgr(const AppCfgMgr &);
    AppCfgMgr& operator=(const AppCfgMgr &);
};

#endif // APPCFGMANAGER_H
