#include <QApplication>
//#include <QOperatingSystemVersion>
#include <QFile>
#include <QTranslator>
#include "utils.h"
#include "displayinfo.h"
#include "capturer.h"
#include "logging.h"
#include "core/captureconfig.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setQuitOnLastWindowClosed(false);

    // log pattern
    qSetMessagePattern("%{time yyyy-MM-dd hh:mm:ss.zzz} (%{type}) %{file}:%{line}] %{message}");


//    LOG(INFO_MSG) << "Operating System: " << QOperatingSystemVersion::current().name() << " "
//              <<  QOperatingSystemVersion::current().majorVersion() << "."
//              <<  QOperatingSystemVersion::current().minorVersion() << "."
//              <<  QOperatingSystemVersion::current().microVersion() << " ("
//              << QSysInfo::currentCpuArchitecture() << ")";

    CAP_LOG(INFO_MSG) << "Application Dir: " << QCoreApplication::applicationDirPath();

    // displays
    DisplayInfo::instance();

    LOAD_QSS(qApp, ":/qss/menu/menu.qss");

    auto language = CaptureConfig::instance()["language"].get<QString>();
    CAP_LOG(INFO_MSG) << "LANGUAGE: " << language;

    QTranslator translator;
    translator.load("languages/capturer_" + language);
    qApp->installTranslator(&translator);

    Capturer window;

    return a.exec();
}
