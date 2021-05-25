#include "MainWindowManager.h"

#include "mainwindow.h"
#include <QApplication>
#include <QScreen>
#include <QElapsedTimer>

#include "AppCfgManager.h"

MainWndMgr::MainWndMgr(QObject *parent)
    : QObject(parent)
{

}


MainWndMgr::~MainWndMgr() {
    qInfo()<<__FUNCTION__;
}

MainWndMgr& MainWndMgr::Instance(){
    static MainWndMgr inst;
    return inst;
}

void MainWndMgr::createWindow(const MainWindowConfig &cfg)
{
    QElapsedTimer timer;
    timer.start();

    MainWindow *window = new MainWindow(cfg);

    // The Qt::WA_DeleteOnClose attribute must be set,
    // otherwise the resource will not be released
    window->setAttribute(Qt::WA_DeleteOnClose, true);

    // if window destroyed,remove reference in set and map.
    connect(window, &MainWindow::destroyed, this, [=](QObject *){
        windows_.remove(window);
        wnd_map_.remove( wnd_map_.key(window));

        // if quit application flag is set and window set is empty,quit the application
        if(quit_app_flag_ && windows_.isEmpty())
        {
            qApp->quit();
        }
    });

    // calculate window geometry
    const QRect availableScrnGeometry =  qApp->primaryScreen()->availableGeometry();
    if(windows_.isEmpty()){
        // 首个窗口还原配置中保存的位置大小信息
        auto geo = AppCfgMgr::windowGeometry();
        if(geo.isEmpty() || !window->restoreGeometry(geo)){

            const QSize size = (availableScrnGeometry.size() * 4) / 5;
            auto pos = availableScrnGeometry.center() - QPoint(size.width(), size.height()) / 2;
            window->setGeometry(pos.x(),pos.y(),
                                size.width(), size.height());
        }
    }else{
        QRect targetRect = cfg.bounds;
        if(cfg.bounds.isEmpty())
        {
            auto rect = MainWndMgr::Instance().lastWindowGeometry();
            targetRect = QRect{rect.x() + AppCfgMgr::newWndOffsetX,
                                      rect.y() + AppCfgMgr::newWndOffsetY,
                                      rect.width(),
                                      rect.height()};

        }
        window->setGeometry(targetRect);
    }

    static int wnd_index = 0;
    windows_.insert(window);
    wnd_map_.insert(wnd_index++, window);

    if(!cfg.initially_hidden)
    {
        window->show();
    }

    qInfo() << "The slow operation took" << timer.elapsed() << "milliseconds";
}

QRect MainWndMgr::lastWindowGeometry() const
{
    auto window = wnd_map_.last();
    return window->geometry();
}

void MainWndMgr::quitApplication()
{
    auto it = windows_.begin();
    auto end = windows_.end();
    for(; it != end; ++it)
    {
        if((*it) != nullptr)
        {
            (*it)->close();
        }
    }
    quit_app_flag_ = true;
}
