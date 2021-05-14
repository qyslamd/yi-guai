#include "mainwindow.h"

#ifdef Q_OS_WIN
#include <windows.h>
#endif

#include <QApplication>
#include <QtDebug>
#include <QFile>
#include <QTranslator>
#include <QMessageBox>
#include <QScreen>

#include <include/base/cef_scoped_ptr.h>
#include <include/cef_command_line.h>
#include <include/cef_sandbox_win.h>

#include "managers/MainWindowManager.h"
#include "managers/CefManager.h"
#include "managers/AppCfgManager.h"
#include "browser/cef_app_browser.h"
#include "browser/scheme_handler.h"
#include "browser/message_loop/main_message_loop.h"
#include "browser/message_loop/main_message_loop_multithreaded_win.h"
#include "browser/message_loop/main_message_loop_external_pump.h"

#include "cef_qwidget.h"
#include "widgets/framewidget.h"
#include "utils/util_qt.h"
#include "tests/testwidget.h"

#if defined(CEF_USE_SANDBOX)
// The cef_sandbox.lib static library may not link successfully with all VS
// versions.
#pragma comment(lib, "cef_sandbox.lib")
#endif

void intializeQtApp(QApplication *app);
int initializeCef(int argc, char *argv[]);

// message loop object.
scoped_ptr<client::MainMessageLoop> message_loop;

int main(int argc, char *argv[])
{
    // Enable High-DPI support on Windows 7 or newer.
    CefEnableHighDPISupport();

    void* sandbox_info = nullptr;

#if defined(CEF_USE_SANDBOX)
    // Manage the life span of the sandbox information object. This is necessary
    // for sandbox support on Windows. See cef_sandbox_win.h for complete details.
    CefScopedSandboxInfo scoped_sandbox;
    sandbox_info = scoped_sandbox.sandbox_info();
#endif

#if defined(Q_OS_WIN)
    // Provide CEF with command-line arguments.
    auto hInstance = GetModuleHandle(NULL);
    CefMainArgs main_args(hInstance);
#else
    CefMainArgs main_args(argc, argv);
#endif

    // CEF applications have multiple sub-processes (render, plugin, GPU, etc)
    // that share the same executable. This function checks the command-line and,
    // if this is a sub-process, executes the appropriate logic.
    int exit_code = CefExecuteProcess(main_args, nullptr, sandbox_info);
    if (exit_code >= 0) {
        // The sub-process has completed so return here.
        return exit_code;
    }

    QApplication qt_app(argc, argv);
    intializeQtApp(&qt_app);
    if(initializeCef(argc, argv) == -1)
    {
        QMessageBox::warning(nullptr, QObject::tr("warning"),
                             QObject::tr("intialize cef failed!\n application will quit!"));
        return -1;
    }
    MainWindowConfig cfg{
        false,
        false,
        false,
        QRect(),
        QString("https://cn.bing.com/")};
    MainWndMgr::Instance().createWindow(cfg);

//    TestWidget w;
//    w.show();

    message_loop->Run();

    int result = qt_app.exec();

    // Shut down CEF.
    CefShutdown();

    message_loop.reset();
    qInfo()<<__FUNCTION__<<"after cef shutdown:"<<message_loop.get();

    return result;
}

void intializeQtApp(QApplication *app)
{
    // 样式表
    QFile file(":/styles/resources/styles/normal.qss");
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
        auto all = file.readAll();
        file.close();
        app->setStyleSheet(all);
    }

    //翻译
    QTranslator *ts = new QTranslator(app);
    ts->load(":/i18ns/resources/i18n/YiGuai_zh.qm");
    app->installTranslator(ts);
}

int initializeCef(int argc, char *argv[])
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

    // Specify CEF global settings here.
    CefSettings settings;
    CefManager::Instance().populateSettings(settings, argc, argv);

#ifdef OS_WIN
    settings.multi_threaded_message_loop =
            command_line->HasSwitch("multi-threaded-message-loop");

    if(!settings.multi_threaded_message_loop)
    {
        settings.external_message_pump =
                command_line->HasSwitch("external-message-pump");

    }
    settings.external_message_pump = true;
#endif
    if (command_line->HasSwitch("enable-chrome-runtime")) {
        // Enable experimental Chrome runtime. See issue #2969 for details.
        settings.chrome_runtime = true;
    }

#if !defined(CEF_USE_SANDBOX)
    settings.no_sandbox = true;
#endif
    //Create the main message loop
    if (settings.multi_threaded_message_loop)
    {
        qInfo()<<"multi_threaded_message_loop";
        message_loop.reset(new client::MainMessageLoopMultithreadedWin);
    }
    else if (settings.external_message_pump)
    {
        qInfo()<<"external_message_pump";
        message_loop = client::MainMessageLoopExternalPump::Create();
    }
    else
    {
        qInfo()<<"MainMessageLoopStd";
        message_loop.reset(new client::MainMessageLoopStd);
    }

    // Initialize CEF.
    CefRefPtr<CefApp> app(new CefAppBrowser);
#if defined(Q_OS_WIN)
    // Provide CEF with command-line arguments.
    auto hInstance = GetModuleHandle(NULL);
    CefMainArgs main_args(hInstance);
#else
    CefMainArgs main_args(argc, argv);
#endif
    if(!CefInitialize(main_args, settings, app.get(), nullptr))
    {
        qInfo()<<"CefInitialize initialized failed!";
    }else{
        qInfo()<<"CefInitialize initialized succeed!";
    }

    custom_scheme::RegisterSchemeHandlers();
    return 0;
}
