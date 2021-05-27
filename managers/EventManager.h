#ifndef EVENTMGR_H
#define EVENTMGR_H

#include <QObject>
#include "globaldef.h"

class EventMgr : public QObject
{
    Q_OBJECT
public:
    static EventMgr& instance();
    ~EventMgr();

signals:
    void tabBarCmd(TabBarCmd cmd, const QVariant &para);
private:
    explicit EventMgr(QObject *parent = nullptr);
    EventMgr& operator=(const EventMgr& other);

signals:

};

#endif // EVENTMGR_H
