#ifndef DISPLAYMANAGER_H
#define DISPLAYMANAGER_H

#include <QObject>
#include <QMutex>

class QScreen;
class DisplayManager : public QObject
{
    Q_OBJECT
public:
    static DisplayManager* GetInstance();
    ~DisplayManager();
private:
    explicit DisplayManager(QObject *parent = nullptr);
    DisplayManager(DisplayManager&);
    DisplayManager& operator=(const DisplayManager &);
    static DisplayManager *gInst;
    static QMutex gMutex;

    struct Gc{
        ~Gc(){if(gInst){delete gInst;gInst = nullptr;}}
    };

signals:

};

#endif // DISPLAYMANAGER_H
