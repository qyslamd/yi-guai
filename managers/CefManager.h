#ifndef CEF_MANAGER_H
#define CEF_MANAGER_H
#pragma once

#include <include/cef_base.h>

class CefManager
{
protected:
    CefManager();
public:
    static CefManager& Instance();
    static std::string cefVersion();
    void populateSettings(CefSettings &settings);

    ///
    /// \brief 分离式时，除了浏览器进程之外的进程可执行体路径
    ///
	static CefString browser_sub_process_path;
#ifdef OS_MAC
    static CefString framework_dir_path;
    static CefString main_bundle_path;
#endif

    static int chrome_runtime;

	static CefString resource_directory_path;

	static CefString locales_directory_path;
	static CefString user_agent;
    ///
    /// \brief 缓存数据存放目录
    ///
	static CefString cache_path;

    static CefString root_cache_path;
	static CefString user_data_path;
	static int persist_session_cookies;
	static int persist_user_preferences;
	static CefString locale;
	static int remote_debugging_port;
	static cef_color_t background_color;
	static CefString accept_language_list;
};

#endif  // CEF_MANAGER_H

