#include "MainWindowManager.h"

#include "mainwindow.h"
#include <QApplication>

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
    window->setAttribute(Qt::WA_DeleteOnClose, true);
    connect(window, &MainWindow::destroyed, this, [=](){
        windows_.remove(window);
    });

    windows_.insert(window);
    if(!cfg.bounds.isEmpty())
    {
        window->setGeometry(cfg.bounds);
    }
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
