#ifndef MAINWINDOWMANAGER_H
#define MAINWINDOWMANAGER_H

#include <QObject>
#include <QRect>
#include <QtDebug>
#include <QSet>
#include <QSharedPointer>

typedef struct MainWindowConfig{

    // If true the window will be marked as inprivate window.
    bool is_inprivate{false};

    // If true the window will always display above other windows.
    bool always_on_top;

    // If true the window will be created initially hidden.
    bool initially_hidden;

    // intially geometry
    QRect bounds;

    // Initial URL to load.
    QString url{""};
}MainWndCfg;

class MainWindow;
class MainWndMgr : public QObject
{
    Q_OBJECT
public:
    static MainWndMgr& Instance();
    ~MainWndMgr();
    void createWindow(const MainWindowConfig &cfg);
    QRect lastWindowGeometry() const;
    void quitApplication();
signals:

private:
    QSet<MainWindow *> windows_;

    QMap<int, MainWindow *> wnd_map_;

    bool quit_app_flag_ = false;
private:
    explicit MainWndMgr(QObject *parent = nullptr);
    MainWndMgr(const MainWndMgr& other);
    MainWndMgr& operator=(const MainWndMgr & other);
};

#endif // MAINWINDOWMANAGER_H
