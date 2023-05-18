#include "mainwindow.h"

#include <QApplication>
#include <QtDebug>
#include <QFile>
#include <QTranslator>
#include <QMessageBox>
#include <QScreen>
#include <QHBoxLayout>
#include <QTimer>

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

#if defined(__linux__) || defined(__linux)

int main(int argc, char *argv[])
{
    CefMainArgs main_args(argc, argv);

    // Parse command-line arguments for use in this method.
    CefRefPtr<CefCommandLine> command_line = CefCommandLine::CreateCommandLine();
    command_line->InitFromArgv(argc, argv);

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
    // Execute the secondary process, if any.
    int exit_code = CefExecuteProcess(main_args, app, nullptr);
    if (exit_code >= 0) {
        // The sub-process has completed so return here.
        return exit_code;
    }

    // initialize Qt.
    QApplication qt_app(argc, argv);
//    qt_app.setQuitOnLastWindowClosed(false);
    QFile file(":/styles/resources/styles/normal.qss");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)){
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

    std::unique_ptr<client::MainMessageLoop> message_loop;
//    if (settings.multi_threaded_message_loop) {
//        qInfo() << __FUNCTION__<<"multi_thread_message_loop";
//        message_loop.reset(new client::MainMessageLoopMultithreadedGtk);
//    } else if (settings.external_message_pump) {
//        qInfo() << __FUNCTION__<<"external_message_pump";
//        message_loop = client::MainMessageLoopExternalPump::Create();
//    } else {
//        qInfo() << __FUNCTION__<<"MainMessageLoopStd";
//        message_loop.reset(new client::MainMessageLoopStd);
//    }
//    message_loop.reset(new client::MainMessageLoopMultithreadedGtk);
    message_loop = client::MainMessageLoopExternalPump::Create();
//    message_loop.reset(new client::MainMessageLoopStd);

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

//    QWidget ww;
//    auto lay = new QVBoxLayout(&ww);

//    QWindow win;
//    win.resize(700, 500);

//    CefWindowInfo info;
//    CefRect wnd_rect{0, 0, win.width(), win.height()};
//    info.SetAsChild(win.winId(), wnd_rect);
//    CefBrowserSettings cbs;
//    CefBrowserHost::CreateBrowser(info, nullptr, "https://cn.bing.com", cbs, nullptr, nullptr);
//    win.show();

//    auto child_proxy = QWidget::createWindowContainer(&win);
//    lay->addWidget(child_proxy);
//    ww.show();

    message_loop->Run();

    // run Qt message loop.
    auto result = qt_app.exec();

    // Shut down CEF.
    CefShutdown();

    // Release objects in reverse order of creation.
    message_loop.reset();

    return result;
}

#endif
