#ifndef APPCONFIG_H
#define APPCONFIG_H

#include <QObject>
#include <QtDebug>

class QSettings;

class AppConfig : public QObject
{
    Q_OBJECT
    explicit AppConfig(QObject *parent = nullptr);
    AppConfig(const AppConfig &);
    AppConfig& operator=(const AppConfig &);
public:
    static AppConfig& instance(){
        static AppConfig inst;
        return inst;
    }
    static QByteArray windowGeometry();
    static void setWindowGeometry(const QByteArray &data);

    ~AppConfig() {
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

#endif // APPCONFIG_H
