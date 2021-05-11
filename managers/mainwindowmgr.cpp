#include "mainwindowmgr.h"

#include "mainwindow.h"
#include <QApplication>

MainWindowMgr::MainWindowMgr(QObject *parent)
    : QObject(parent)
{
    connect(qApp, &QApplication::aboutToQuit, this, &MainWindowMgr::closeAllWindows);

}

void MainWindowMgr::createWindow()
{
    MainWindow *window = new MainWindow;
    window->setAttribute(Qt::WA_DeleteOnClose, true);
    connect(window, &MainWindow::destroyed, this, [=](){
        windows_.remove(window);
    });
    windows_.insert(window);
    window->show();
}

QRect MainWindowMgr::lastWindowGeometry() const
{
    auto iter = windows_.end();
    --iter;
    auto window = *iter;
    return window->geometry();
}

void MainWindowMgr::closeAllWindows()
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
