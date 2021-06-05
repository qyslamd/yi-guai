#include "displayinfo.h"
#include <QScreen>
#include <QApplication>
#include <QDesktopWidget>
#include "utils.h"
#include "logging.h"

#include <QtDebug>

QSize DisplayInfo::max_size_{0, 0};

DisplayInfo::DisplayInfo(QObject *parent)
    : QObject(parent)
{
    connect(qApp, &QGuiApplication::screenAdded, [this](QScreen * screen){
        emit added(screen);
        emit changed();
    });

    connect(qApp, &QGuiApplication::screenRemoved, [this](QScreen *screen){
        emit removed(screen);
        emit changed();
    });

    connect(this, &DisplayInfo::changed, this, &DisplayInfo::update);

    update();
}

QList<QScreen*> DisplayInfo::screens()
{
    return QGuiApplication::screens();
}

DisplayInfo::~DisplayInfo()
{
    qInfo()<<__FUNCTION__;
}

void DisplayInfo::update()
{
    auto screens = QGuiApplication::screens();
    CAP_LOG(INFO_MSG) << "[Capturer] Screen Number: " << screens.size();

    auto& width = max_size_.rwidth();
    auto& height = max_size_.rheight();

    foreach(const auto& screen, screens) {
        auto geometry = screen->geometry();
        CAP_LOG(INFO_MSG) << "\t" << screen->name() << " " << geometry << "  DPR: " << screen->devicePixelRatio() << "  DPI: " << screen->logicalDotsPerInch();

        width += geometry.width();

        // height = The max height
        if(height < geometry.height())
            height = geometry.height();
    }

    CAP_LOG(INFO_MSG);

    CAP_LOG(INFO_MSG) << "[Capturer] Vitual Desktop Max Size: " << max_size_;
}

QSize DisplayInfo::maxSize()
{
    return max_size_;
}
