#ifndef MAINWINDOWMANAGER_H
#define MAINWINDOWMANAGER_H

#include <QObject>
#include <QRect>
#include <QtDebug>
#include <QSet>
#include <QSharedPointer>

typedef struct MainWindowConfig{
    MainWindowConfig(bool inprivate,
                     bool always_on_top,
                     bool init_hidden,
                     const QRect &bounds,
                     const QString &url)
        : is_inprivate_(inprivate)
        , always_on_top_(always_on_top)
        , initially_hidden_(init_hidden)
        , bounds_(bounds)
        , url_(url)
    {}

    MainWindowConfig()
        : is_inprivate_(false)
        , always_on_top_(false)
        , initially_hidden_(false)
        , bounds_(QRect())
        , url_("")
    {}
    MainWindowConfig(const QString &url)
        : is_inprivate_(false)
        , always_on_top_(false)
        , initially_hidden_(false)
        , bounds_(QRect())
        , url_(url)
    {}
    MainWindowConfig(bool is_inprivate)
        : is_inprivate_(is_inprivate)
        , always_on_top_(false)
        , initially_hidden_(false)
        , bounds_(QRect())
        , url_("")
    {}
    MainWindowConfig(bool is_inprivate, const QString &url)
        : is_inprivate_(is_inprivate)
        , always_on_top_(false)
        , initially_hidden_(false)
        , bounds_(QRect())
        , url_(url)
    {}

    // If true the window will be marked as inprivate window.
    bool is_inprivate_;

    // If true the window will always display above other windows.
    bool always_on_top_;

    // If true the window will be created initially hidden.
    bool initially_hidden_;

    // intially geometry
    QRect bounds_;

    // Initial URL to load.
    QString url_;
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
    void closeAllInprivate();
    size_t inprivateCount() const;

    static int newWndOffsetX;
    static int newWndOffsetY;
signals:
    void inprivateWndCntChanged();
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
