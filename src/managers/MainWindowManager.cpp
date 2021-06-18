#include "MainWindowManager.h"

#include "mainwindow.h"
#include <QApplication>
#include <QScreen>
#include <QElapsedTimer>

#include "AppCfgManager.h"
#include "BookmarkManager.h"
#include "FaviconManager.h"
#include "HistoryManager.h"
#include "AddrInputManager.h"

int MainWndMgr::newWndOffsetX = 22;
int MainWndMgr::newWndOffsetY = 30;

MainWndMgr::MainWndMgr(QObject *parent)
    : QObject(parent)
{
    // 窗口创建之前需要准备好所有的数据

    // icon数据
    FaviconMgr::Instance();
    // 书签数据
    BookmarkMgr::Instance();
    // 历史记录数据
    HistoryMgr::Instance();
    // 地址栏输入数据
    AddrInputMgr::Instance();
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
    connect(this, &MainWndMgr::inprivateWndCntChanged, window, &MainWindow::onInpWndCntChanged);

    // The Qt::WA_DeleteOnClose attribute must be set,
    // otherwise the resource will not be released
    window->setAttribute(Qt::WA_DeleteOnClose, true);

    // if window destroyed,remove reference in set and map.
    connect(window, &MainWindow::destroyed, this, [=](QObject *){
        windows_.remove(window);
        wnd_map_.remove( wnd_map_.key(window));
        emit inprivateWndCntChanged();

        // if quit application flag is set and window set is empty,quit the application
        if(quit_app_flag_ && windows_.isEmpty())
        {
            qApp->quit();
        }
    });
    // 先改变位置，再记录，不然获取到的记录是空的
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
        QRect targetRect = cfg.bounds_;
        if(cfg.bounds_.isEmpty())
        {
            auto rect = MainWndMgr::Instance().lastWindowGeometry();
            targetRect = QRect{rect.x() + newWndOffsetX,
                                      rect.y() + newWndOffsetY,
                                      rect.width(),
                                      rect.height()};

        }
        window->setGeometry(targetRect);
    }

    //
    static int wnd_index = 0;
    windows_.insert(window);
    wnd_map_.insert(wnd_index++, window);
    if(cfg.is_inprivate_){
        emit inprivateWndCntChanged();
    }

    if(!cfg.initially_hidden_)
    {
        window->show();
    }

    qInfo()<<"\033[32m[Execute Time]"<<__FUNCTION__<<":" << timer.elapsed() << "ms"<<"\033[0m";

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

void MainWndMgr::closeAllInprivate()
{
    auto it = windows_.begin();
    auto end = windows_.end();
    for(; it != end; ++it)
    {
        if( *it && (*it)->isInprivate())
        {
            (*it)->close();
        }
    }
    quit_app_flag_ = true;
}

size_t MainWndMgr::inprivateCount() const
{
    auto it = windows_.begin();
    auto end = windows_.end();
    size_t count = 0;
    for(; it != end; ++it)
    {
        if( *it && (*it)->isInprivate())
        {
            ++count;
        }
    }
    return count;
}
