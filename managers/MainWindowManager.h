#ifndef MAINWINDOWMANAGER_H
#define MAINWINDOWMANAGER_H

#include <QObject>
#include <QtDebug>
#include <QSet>

class MainWindow;
class MainWndMgr : public QObject
{
    Q_OBJECT
    explicit MainWndMgr(QObject *parent = nullptr);
    MainWndMgr(const MainWndMgr& other);
    MainWndMgr& operator=(const MainWndMgr & other);
public:
    static MainWndMgr& Instance();
    ~MainWndMgr();
    void createWindow();
    QRect lastWindowGeometry() const;
signals:
private:
    QSet<MainWindow *> windows_;

    void closeAllWindows();

};

#endif // MAINWINDOWMANAGER_H
