#ifndef MAINWINDOWMANAGER_H
#define MAINWINDOWMANAGER_H

#include <QObject>
#include <QRect>
#include <QtDebug>
#include <QSet>

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
    explicit MainWndMgr(QObject *parent = nullptr);
    MainWndMgr(const MainWndMgr& other);
    MainWndMgr& operator=(const MainWndMgr & other);
public:
    static MainWndMgr& Instance();
    ~MainWndMgr();
    void createWindow(const MainWindowConfig &cfg);
    QRect lastWindowGeometry() const;
    void closeAllWindows();
signals:

private:
    QSet<MainWindow *> windows_;
};

#endif // MAINWINDOWMANAGER_H
