#if defined(__linux__) || defined(__linux)
#include <gtk/gtk.h>
#endif

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

#ifdef Q_OS_WIN
#include <include/cef_sandbox_win.h>
#include "browser/message_loop/main_message_loop_multithreaded_win.h"
#endif

#include "managers/MainWindowManager.h"
#include "managers/CefManager.h"
#include "managers/AppCfgManager.h"
#include "browser/cef_app_browser.h"
#include "browser/cef_app_render.h"
#include "browser/cef_app_other.h"
#include "browser/scheme_handler.h"
#include "browser/message_loop/main_message_loop.h"
#include "browser/message_loop/main_message_loop_external_pump.h"

#include "demo_cef_qwidget.h"

#if defined(__linux__) || defined(__linux)
#include <gtk/gtkgl.h>
#include <X11/Xlib.h>
#endif

#if defined(__linux__) || defined(__linux)
int XErrorHandlerImpl(Display* display, XErrorEvent* event) {
    LOG(WARNING) << "X error received: "
                 << "type " << event->type << ", "
                 << "serial " << event->serial << ", "
                 << "error_code " << static_cast<int>(event->error_code) << ", "
                 << "request_code " << static_cast<int>(event->request_code)
                 << ", "
                 << "minor_code " << static_cast<int>(event->minor_code);
    return 0;
}

int XIOErrorHandlerImpl(Display* display) {
    return 0;
}

void TerminationSignalHandler(int signatl) {
    LOG(ERROR) << "Received termination signal: " << signatl;
//  MainContext::Get()->GetRootWindowManager()->CloseAllWindows(true);
}
#endif

int initializeCef(int argc, char *argv[]);
void allocConsoleWin();
void freeConsoleWin();

namespace  {
// message loop object.
scoped_ptr<client::MainMessageLoop> message_loop;
}

int main(int argc, char *argv[])
{
//    allocConsoleWin();

    // Enable High-DPI support on Windows 7 or newer.
    CefEnableHighDPISupport();

#if defined(Q_OS_WIN)
    // Provide CEF with command-line arguments.
    auto hInstance = GetModuleHandle(NULL);
    CefMainArgs main_args(hInstance);
#else
    CefMainArgs main_args(argc, argv);
#endif

    // Parse command-line arguments for use in this method.
    CefRefPtr<CefCommandLine> command_line = CefCommandLine::CreateCommandLine();
#ifdef OS_WIN
    command_line->InitFromString(::GetCommandLineW());
#else
    command_line->InitFromArgv(argc, argv);
#endif
    ClientApp::ProcessType process_type = ClientApp::GetProcessType(command_line);
    CefRefPtr<ClientApp> app;
    qInfo()<<__FUNCTION__<<"ProcessType: "<<QString::fromStdString(ClientApp::processTypeToString(process_type));
    if (process_type != ClientApp::BrowserProcess)
    {
        if (process_type == ClientApp::RendererProcess
                 || process_type == ClientApp::ZygoteProcess)
        {
            // On Linux the zygote process is used to spawn other process types. Since
            // we don't know what type of process it will be give it the renderer
            // client.
            app = new CefAppRender();
        }
        else if (process_type == ClientApp::OtherProcess)
        {
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
    }

    QApplication qt_app(argc, argv);
    if(initializeCef(argc, argv) == -1)
    {
        QMessageBox::warning(nullptr, QObject::tr("warning"),
                             QObject::tr("intialize cef failed!\n application will quit!"));
        return -1;
    }

    DemoCefQWidget w/*("https://www.baidu.com/")*/;
    w.resize(1024, 768);
    w.show();

    message_loop->Run();

    int result = qt_app.exec();

    // Shut down CEF.
    CefShutdown();

    message_loop.reset();
    qInfo()<<__FUNCTION__<<"after cef shutdown:"<<message_loop.get();

//    freeConsoleWin();
    return result;
}

int initializeCef(int argc, char *argv[])
{
#ifdef Q_OS_LINUX
    CefScopedArgArray scoped_arg_array(argc, argv);
    char** argv_copy = scoped_arg_array.array();
    Q_UNUSED(argv_copy);
#endif

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
    settings.no_sandbox = true;

#ifdef OS_WIN
    settings.multi_threaded_message_loop =
            command_line->HasSwitch("multi-threaded-message-loop");

    if(!settings.multi_threaded_message_loop)
    {
        settings.external_message_pump =
                command_line->HasSwitch("external-message-pump");

    }
#endif
    settings.external_message_pump = true;
    if (command_line->HasSwitch("enable-chrome-runtime")) {
        // Enable experimental Chrome runtime. See issue #2969 for details.
        settings.chrome_runtime = true;
    }

#if defined(Q_OS_WIN)
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
#else
    message_loop = client::MainMessageLoopExternalPump::Create();
#endif

    // Initialize CEF.
    CefRefPtr<CefApp> app(new CefAppBrowser);
#if defined(Q_OS_WIN)
    // Provide CEF with command-line arguments.
    auto hInstance = GetModuleHandle(NULL);
    CefMainArgs main_args(hInstance);
#else
    CefMainArgs main_args(argc, argv);
#endif
    if(!CefInitialize(main_args, settings, app, nullptr))
    {
        qInfo()<<"CefInitialize initialized failed!";
    }else{
        qInfo()<<"CefInitialize initialized succeed!";
    }

#if defined(Q_OS_LINUX1)
    // The Chromium sandbox requires that there only be a single thread during
    // initialization. Therefore initialize GTK after CEF.
    gtk_init(&argc, &argv_copy);

    // Perform gtkglext initialization required by the OSR example.
    gtk_gl_init(&argc, &argv_copy);

    // Install xlib error handlers so that the application won't be terminated
    // on non-fatal errors. Must be done after initializing GTK.
    XSetErrorHandler(XErrorHandlerImpl);
    XSetIOErrorHandler(XIOErrorHandlerImpl);

    // Install a signal handler so we clean up after ourselves.
    signal(SIGINT, TerminationSignalHandler);
    signal(SIGTERM, TerminationSignalHandler);
#endif

    custom_scheme::RegisterSchemeHandlers();
    return 0;
}

void allocConsoleWin()
{
#ifdef Q_OS_WIN
    AllocConsole();
    freopen("CONOUT$","w+t",stdout);
    freopen("CONIN$","r+t",stdin);
#endif
}

void freeConsoleWin()
{
#ifdef Q_OS_WIN
    FreeConsole();
#endif
}

