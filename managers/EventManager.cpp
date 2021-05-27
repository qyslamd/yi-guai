#include "EventManager.h"
#include <QtDebug>

EventMgr &EventMgr::instance()
{
    static EventMgr mgr;
    return mgr;
}

EventMgr::~EventMgr()
{
    qInfo()<<__FUNCTION__;
}

EventMgr::EventMgr(QObject *parent) : QObject(parent)
{

}
