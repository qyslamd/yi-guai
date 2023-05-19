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
    const char *KeyShowBkmkBar = "ShowBookmarkBar";
    const char *KeyShowBkmkBtn = "ShowBookmarkBtn";


    const char *SectionDevTool = "DevTool";
    const char *KeyDevToolGeo = "DevToolGeo";

}

QString AppCfgMgr::gDefautlUrl = "https://cn.bing.com/";
QString AppCfgMgr::gProjectUrl = "https://gitee.com/slamdd/yi-guai";
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
    return Instance().value(SectionGeneral, KeyWindowGeo, "").toByteArray();
}

void AppCfgMgr::setWindowGeometry(const QByteArray &data)
{
    Instance().setValue(SectionGeneral, KeyWindowGeo, data);
}

QString AppCfgMgr::homePageUrl()
{
    return Instance().value(SectionGeneral, KeyHomePage, "https://cn.bing.com/").toString();
}

void AppCfgMgr::setHomePageUrl(const QString &data)
{
    Instance().setValue(SectionGeneral, KeyHomePage, data);
}

QString AppCfgMgr::newTabPageUrl()
{
    return Instance().value(SectionGeneral, KeyNewPageUrl, "").toString();
}

void AppCfgMgr::setNewTabPageUrl(const QString &data)
{
    Instance().setValue(SectionGeneral, KeyNewPageUrl, data);
}

QByteArray AppCfgMgr::devToolGeometry()
{
    return Instance()
            .value(SectionDevTool, KeyDevToolGeo, "")
            .toByteArray();
}

void AppCfgMgr::setDevToolGeometry(const QByteArray &data)
{
    Instance().setValue(SectionDevTool, KeyDevToolGeo, data);
}

bool AppCfgMgr::bookmarkBarVisible()
{
    return Instance().value(SectionGeneral, KeyShowBkmkBar, "").toBool();
}

void AppCfgMgr::setBookmarkBarVisible(const bool &data)
{
    Instance().setValue(SectionGeneral, KeyShowBkmkBar, data);
}

bool AppCfgMgr::bookmarkBtnVisible()
{
    return Instance().value(SectionGeneral, KeyShowBkmkBtn, "").toBool();
}

void AppCfgMgr::setBookmarkBtnVisible(const bool &data)
{
    Instance().setValue(SectionGeneral, KeyShowBkmkBtn, data);
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

    emit preferenceChanged();
}

void AppCfgMgr::setValue(const QString &key, const QVariant &data)
{
    settings_->setValue(key, data);
    emit preferenceChanged();
}
