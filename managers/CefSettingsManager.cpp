#include "CefSettingsManager.h"

#include <QCoreApplication>
#include <QDir>
#include <QDebug>
#include <QFileInfo>
#include <stdlib.h>

#include "utils/util_qt.h"

CefString CefSettingsMgr::browser_sub_process_path;

CefString CefSettingsMgr::resource_directory_path;

CefString CefSettingsMgr::locales_directory_path;

CefString CefSettingsMgr::user_agent;

CefString CefSettingsMgr::cache_path;

CefString CefSettingsMgr::root_cache_path;

CefString CefSettingsMgr::user_data_path;

int CefSettingsMgr::persist_session_cookies;

int CefSettingsMgr::persist_user_preferences;

CefString CefSettingsMgr::locale;

int CefSettingsMgr::remote_debugging_port = 33221;

cef_color_t CefSettingsMgr::background_color;

CefString CefSettingsMgr::accept_language_list;

CefSettingsMgr& CefSettingsMgr::Instance()
{
    static CefSettingsMgr s_instance;
    return s_instance;
}

CefSettingsMgr::CefSettingsMgr()
{
    auto dataPath = UtilQt::appDataPath();

    cache_path.FromString(dataPath.toStdString());

    locale.FromString("zh-CN");
    accept_language_list.FromString("zh-CN");
}
