#include "mainwindow.h"

#ifdef Q_OS_WIN
#include <windows.h>
#include <include/cef_sandbox_win.h>
#include "browser/message_loop/main_message_loop_multithreaded_win.h"

#include <QApplication>
#include <QtDebug>
#include <QFile>
#include <QTranslator>
#include <QMessageBox>
#include <QScreen>

#include <include/internal/cef_ptr.h>
#include <include/cef_command_line.h>
#include "managers/MainWindowManager.h"
#include "managers/AppCfgManager.h"
#include "browser/CefManager.h"
#include "browser/cef_app_browser.h"
#include "browser/cef_app_render.h"
#include "browser/cef_app_other.h"
#include "browser/scheme_handler.h"
#include "browser/message_loop/main_message_loop.h"
#include "browser/message_loop/main_message_loop_external_pump.h"

#include "cef_qwidget.h"
#include "utils/util_qt.h"


int main(int argc, char *argv[])
{
//    AllocConsole();
//    freopen("CONOUT$","w+t",stdout);
//    freopen("CONIN$","r+t",stdin);

    // Enable High-DPI support on Windows 7 or newer.
    CefEnableHighDPISupport();

    // Provide CEF with command-line arguments.
    auto hInstance = GetModuleHandle(NULL);
    CefMainArgs main_args(hInstance);

    // Parse command-line arguments for use in this method.
    CefRefPtr<CefCommandLine> command_line = CefCommandLine::CreateCommandLine();
    command_line->InitFromString(::GetCommandLineW());

    CefRefPtr<ClientApp> app;
    ClientApp::ProcessType process_type = ClientApp::GetProcessType(command_line);
    qInfo()<<__FUNCTION__<<"ProcessType: "<<QString::fromStdString(ClientApp::processTypeToString(process_type));
    if (process_type == ClientApp::BrowserProcess) {
        app = new CefAppBrowser;
    } else if (process_type == ClientApp::RendererProcess
                       || process_type == ClientApp::ZygoteProcess) {
        // On Linux the zygote process is used to spawn other process types. Since
        // we don't know what type of process it will be give it the renderer
        // client.
        app = new CefAppRender();
    }
    else if (process_type == ClientApp::OtherProcess) {
        app = new CefAppOther();
    }
    void* sandboxInfo = nullptr;

    // CEF applications have multiple sub-processes (render, plugin, GPU, etc)
    // that share the same executable. This function checks the command-line and,
    // if this is a sub-process, executes the appropriate logic.
    int exit_code = CefExecuteProcess(main_args, app, sandboxInfo);
    if (exit_code >= 0) {
        // The sub-process has completed so return here.
        //            freeConsoleWin();
        return exit_code;
    }

    QApplication qt_app(argc, argv);
    QFile file(":/styles/resources/styles/normal.qss");
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
        auto all = file.readAll();
        file.close();
        qt_app.setStyleSheet(all);
    }
    QTranslator *ts = new QTranslator(&qt_app);
    ts->load(":/i18ns/resources/i18n/YiGuai_zh.qm");
    qt_app.installTranslator(ts);


    // Specify CEF global settings here.
    CefSettings settings;
    CefManager::Instance().populateSettings(settings, command_line);
    scoped_ptr<client::MainMessageLoop> message_loop;
    message_loop = client::MainMessageLoopExternalPump::Create();

    // Initialize CEF.
    if(!CefInitialize(main_args, settings, app, nullptr))
    {
        qInfo()<<"CefInitialize initialized failed!";
    }else{
        qInfo()<<"CefInitialize initialized succeed!";
    }

    custom_scheme::RegisterSchemeHandlers();

    MainWindowConfig cfg;
    cfg.url_ = "https://cn.bing.com/";
    MainWndMgr::Instance().createWindow(cfg);

    message_loop->Run();

    int result = qt_app.exec();

    CefShutdown();

    message_loop.reset();

    // FreeConsole();

    return result;
}


#endif // Q_OS_WIN
