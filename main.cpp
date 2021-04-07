#include "mainwindow.h"

#include <QApplication>
#include <QtDebug>


#include <include/base/cef_scoped_ptr.h>
#include <include/cef_command_line.h>
#include <include/cef_sandbox_win.h>

#include "cef_app_browser.h"
#include "message_loop/main_message_loop.h"
#include "message_loop/main_message_loop_multithreaded_win.h"
#include "message_loop/main_message_loop_external_pump.h"

#ifdef Q_OS_WIN
#include <windows.h>
#endif

#include "cef_qwidget.h"

#if defined(CEF_USE_SANDBOX)
// The cef_sandbox.lib static library may not link successfully with all VS
// versions.
#pragma comment(lib, "cef_sandbox.lib")
#endif


int main(int argc, char *argv[])
{
    using namespace client;
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

    // Parse command-line arguments for use in this method.
    CefRefPtr<CefCommandLine> command_line = CefCommandLine::CreateCommandLine();
    command_line->InitFromString(::GetCommandLineW());

    // Specify CEF global settings here.
    CefSettings settings;
    settings.log_severity = LOGSEVERITY_WARNING;
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
//    settings.log_severity = LOGSEVERITY_WARNING;

    if (command_line->HasSwitch("enable-chrome-runtime")) {
        // Enable experimental Chrome runtime. See issue #2969 for details.
        settings.chrome_runtime = true;
    }

#if !defined(CEF_USE_SANDBOX)
    settings.no_sandbox = true;
#endif

    CefRefPtr<CefApp> app(new CefAppBrowser);

    // Create the main message loop object.
    scoped_ptr<MainMessageLoop> message_loop;
    if (settings.multi_threaded_message_loop)
    {
        qInfo()<<"multi_threaded_message_loop";
        message_loop.reset(new MainMessageLoopMultithreadedWin);
    }
    else if (settings.external_message_pump)
    {
        qInfo()<<"external_message_pump";
        message_loop = MainMessageLoopExternalPump::Create();
    }
    else
    {
        qInfo()<<"MainMessageLoopStd";
        message_loop.reset(new MainMessageLoopStd);
    }

    // Initialize CEF.
    if(!CefInitialize(main_args, settings, app.get(), sandbox_info))
    {
        qInfo()<<"CefInitialize initialized failed!";
        return -1;
    }else{
        qInfo()<<"CefInitialize initialized succeed!";
    }

    MainWindow w;
    w.show();

    message_loop->Run();

    int result = qt_app.exec();

    // Shut down CEF.
    CefShutdown();

    message_loop.reset();
    qInfo()<<__FUNCTION__<<"after cef shutdown:"<<message_loop.get();

    return result;
}
