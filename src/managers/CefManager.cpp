#include "CefManager.h"

#include <QCoreApplication>
#include <QDir>
#include <QDebug>
#include <QFileInfo>
#include <stdlib.h>
#include <string>
#include <QApplication>

#include "utils/util_qt.h"
#include <include/internal/cef_types.h>
#include <include/cef_version.h>
#include <include/base/cef_scoped_ptr.h>
#include <include/cef_command_line.h>

#include "browser/client_switches.h"

bool CefManager::seperate_sub_process;
CefString CefManager::browser_sub_process_path;

CefString CefManager::resource_directory_path;

CefString CefManager::locales_directory_path;

CefString CefManager::user_agent="Mozilla/5.0(Windows NT 6.1;WOW64) "\
"AppleWebKit/537.36(KHTML,like Gecko) "\
"Chrome/86.0.4240.183 Safari/537.36 YiGuai/1.0.0";

CefString CefManager::cache_path;

CefString CefManager::root_cache_path;

CefString CefManager::user_data_path;

int CefManager::persist_session_cookies = 1;

int CefManager::persist_user_preferences = 1;

CefString CefManager::locale;

uint CefManager::remote_debugging_port = 12233;

cef_color_t CefManager::background_color;

CefString CefManager::accept_language_list;

QMap<int, QString> CefManager::zoom_map;

CefManager& CefManager::Instance()
{
    static CefManager s_instance;
    return s_instance;
}

std::string CefManager::cefVersion()
{
    std::stringstream ss;
    ss<<cef_version_info(0)
     <<"."<<cef_version_info(1)
    <<"."<<cef_version_info(2)
    <<"+chromium-"
    <<cef_version_info(4)
    <<"."<<cef_version_info(5)
    <<"."<<cef_version_info(6)
    <<"."<<cef_version_info(7);

    return ss.str();
}

void CefManager::populateSettings(CefSettings &settings, int argc, char *argv[])
{
    // Parse command-line arguments for use in this method.
    CefRefPtr<CefCommandLine> command_line = CefCommandLine::CreateCommandLine();
#ifdef OS_WIN
    Q_UNUSED(argc);
    Q_UNUSED(argv);
    command_line->InitFromString(::GetCommandLineW());
#else
    command_line->InitFromArgv(argc, argv);
#endif

    CefString(&settings.cache_path) = cache_path;
    CefString(&settings.root_cache_path) = root_cache_path;
    // linux会默认加载Locale
#ifndef OS_LINUX
    CefString(&settings.locale) = locale;
    CefString(&settings.accept_language_list) = accept_language_list;
#endif

    if(command_line->HasSwitch("remote-debugging-port"))
    {
        std::string port = command_line->GetSwitchValue("remote-debugging-port");
        uint portI = QString::fromStdString(port).toUInt();
        settings.remote_debugging_port = portI == 0 ? remote_debugging_port : portI;
    }else{
        settings.remote_debugging_port = remote_debugging_port;
    }
    settings.log_severity = LOGSEVERITY_WARNING;
#ifdef Q_OS_LINUX
    settings.log_severity = LOGSEVERITY_ERROR;
    seperate_sub_process = true;
//    CefString(&settings.browser_subprocess_path) = browser_sub_process_path;
#endif
    settings.background_color = background_color;
    settings.persist_session_cookies = persist_session_cookies;
    settings.persist_user_preferences = persist_user_preferences;
}

CefManager::CefManager()
{
    auto dataPath = UtilQt::appDataPath();
    QDir dir(dataPath);
    const char * sudDir = "CEF";
    if(!dir.cd(sudDir)){
        dir.mkdir(sudDir);
    }
    dir.cd(sudDir);
    auto cachePath = dir.absolutePath();

    cache_path.FromString(cachePath.toStdString());
    root_cache_path.FromString(dataPath.toStdString());

    locale.FromString("zh-CN");
    accept_language_list.FromString("zh-CN");
    background_color = CefColorSetARGB(255, 163, 198, 218);    //163,198,218

    // render process path
    auto path = qApp->applicationDirPath();
    browser_sub_process_path.FromString(path.toStdString());
    qInfo()<<__FUNCTION__<<path;

    // cef 缩放对照表
    zoom_map.insert(-7, "25%");
    zoom_map.insert(-6, "33%");
    zoom_map.insert(-5, "50%");
    zoom_map.insert(-4, "67%");
    zoom_map.insert(-3, "75%");
    zoom_map.insert(-2, "80%");
    zoom_map.insert(-1, "90%");
    zoom_map.insert(0, "100%");
    zoom_map.insert(1, "110%");
    zoom_map.insert(2, "125%");
    zoom_map.insert(3, "150%");
    zoom_map.insert(4, "175%");
    zoom_map.insert(5, "200%");
    zoom_map.insert(6, "250%");
    zoom_map.insert(7, "300%");
    zoom_map.insert(8, "400%");
    zoom_map.insert(9, "500%");
}
