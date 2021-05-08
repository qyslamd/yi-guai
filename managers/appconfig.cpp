#include "appconfig.h"
#include "utils/util_qt.h"

#include <QSettings>
#include <QDir>

AppConfig::AppConfig(QObject *parent)
    : QObject(parent)
{
    if(settings_ == nullptr)
    {
        auto loc = UtilQt::appDataPath();
        loc = QDir(loc).filePath("app-config");

        settings_ = new QSettings(loc, QSettings::IniFormat);
//        settings_->setIniCodec("utf-8");
    }
}

QByteArray AppConfig::windowGeometry()
{
    return instance().value("General","WindowPos", "").toByteArray();
}

void AppConfig::setWindowGeometry(const QByteArray &data)
{
    instance().setValue("General","WindowPos", data);
}

QVariant AppConfig::value(const QString &group,
                          const QString &key,
                          const QVariant &defualt)
{
    settings_->beginGroup(group);
    auto ret = settings_->value(key, defualt);
    settings_->endGroup();

    return ret;
}

QVariant AppConfig::value(const QString &key,
                          const QVariant &defualt)
{
    return settings_->value(key, defualt);
}

void AppConfig::setValue(const QString &group,
                         const QString &key,
                         const QVariant &data)
{
    settings_->beginGroup(group);
    settings_->setValue(key, data);
    settings_->endGroup();

    emit configChanged();
}

void AppConfig::setValue(const QString &key, const QVariant &data)
{
    settings_->setValue(key, data);
    emit configChanged();
}
