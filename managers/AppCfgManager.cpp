#include "AppCfgManager.h"
#include "utils/util_qt.h"

#include <QSettings>
#include <QDir>
#include <QRect>

namespace {
    const char *SectionGeneral = "General";
    const char *KeyWindowGeo = "WindowGeo";
    const char *KeyHomePage = "HomePage";
    const char *KeyNewPageUrl = "HomePage";


    const char *SectionDevTool = "DevTool";
    const char *KeyDevToolGeo = "DevToolGeo";

}

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
    return instance().value(SectionGeneral, KeyWindowGeo, "").toByteArray();
}

void AppCfgMgr::setWindowGeometry(const QByteArray &data)
{
    instance().setValue(SectionGeneral, KeyWindowGeo, data);
}

QString AppCfgMgr::homePageUrl()
{
    return instance().value(SectionGeneral, KeyHomePage, "https://cn.bing.com/").toString();
}

void AppCfgMgr::setHomePageUrl(const QString &data)
{
    instance().setValue(SectionGeneral, KeyHomePage, data);
}

QString AppCfgMgr::newTabPageUrl()
{
    return instance().value(SectionGeneral, KeyNewPageUrl, "").toString();
}

void AppCfgMgr::setNewTabPageUrl(const QString &data)
{
    instance().setValue(SectionGeneral, KeyNewPageUrl, data);
}

QByteArray AppCfgMgr::devToolGeometry()
{
    return instance()
            .value(SectionDevTool, KeyDevToolGeo, "")
            .toByteArray();
}

void AppCfgMgr::setDevToolGeometry(const QByteArray &data)
{
    instance().setValue(SectionDevTool, KeyDevToolGeo, data);
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
