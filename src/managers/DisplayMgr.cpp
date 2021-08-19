#include "DisplayMgr.h"

#include <QtDebug>
#include <QMutexLocker>
#include <QApplication>
#include <QScreen>

DisplayManager *DisplayManager::GetInstance()
{
    if(gInst == nullptr){
        QMutexLocker locker(&gMutex);
        if(gInst == nullptr){
            gInst = new DisplayManager;
        }
    }
    static DisplayManager::Gc gc;
    return gInst;
}

DisplayManager::~DisplayManager()
{
    qInfo()<<__FUNCTION__;
}

DisplayManager* DisplayManager::gInst = nullptr;
QMutex DisplayManager::gMutex;

DisplayManager::DisplayManager(QObject *parent) : QObject(parent)
{
    connect(qApp, &QApplication::screenAdded, [this](QScreen *screen)
    {

    });
}
