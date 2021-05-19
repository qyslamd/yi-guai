#include "MainWindowManager.h"

#include "mainwindow.h"
#include <QApplication>
#include <QScreen>

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
    MainWindow *window = new MainWindow(cfg);

    // The Qt::WA_DeleteOnClose attribute must be set,
    // otherwise the resource will not be released
    window->setAttribute(Qt::WA_DeleteOnClose, true);

    connect(window, &MainWindow::destroyed, this, [=](){
        windows_.remove(window);
        if(need_quit_app_){
            qApp->quit();
        }
    });

    if(windows_.isEmpty()){
        // 首个窗口还原配置中保存的位置大小信息
        auto geo = AppCfgMgr::windowGeometry();
        if(geo.isEmpty() || !window->restoreGeometry(geo)){
            const QRect availableGeometry =  qApp->primaryScreen()->availableGeometry();
            const QSize size = (availableGeometry.size() * 4) / 5;
            auto pos = availableGeometry.center() - QPoint(size.width(), size.height()) / 2;
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

    windows_.insert(window);
    if(!cfg.initially_hidden)
    {
        window->show();
    }
}

QRect MainWndMgr::lastWindowGeometry() const
{
    auto iter = windows_.end();
    --iter;
    auto window = *iter;
    return window->geometry();
}

void MainWndMgr::closeAllWindows()
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
}

void MainWndMgr::setNeedQuitApp(bool need)
{
    need_quit_app_ = need;
}
