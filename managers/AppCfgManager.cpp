#include "AppCfgManager.h"
#include "utils/util_qt.h"

#include <QSettings>
#include <QDir>

AppCfgMgr::AppCfgMgr(QObject *parent)
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

QByteArray AppCfgMgr::windowGeometry()
{
    return instance().value("General","WindowPos", "").toByteArray();
}

void AppCfgMgr::setWindowGeometry(const QByteArray &data)
{
    instance().setValue("General","WindowPos", data);
}

QVariant AppCfgMgr::value(const QString &group,
                          const QString &key,
                          const QVariant &defualt)
{
    settings_->beginGroup(group);
    auto ret = settings_->value(key, defualt);
    settings_->endGroup();

    return ret;
}

QVariant AppCfgMgr::value(const QString &key,
                          const QVariant &defualt)
{
    return settings_->value(key, defualt);
}

void AppCfgMgr::setValue(const QString &group,
                         const QString &key,
                         const QVariant &data)
{
    settings_->beginGroup(group);
    settings_->setValue(key, data);
    settings_->endGroup();

    emit configChanged();
}

void AppCfgMgr::setValue(const QString &key, const QVariant &data)
{
    settings_->setValue(key, data);
    emit configChanged();
}
