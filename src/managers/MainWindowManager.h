#ifndef MAINWINDOWMANAGER_H
#define MAINWINDOWMANAGER_H

#include <QObject>
#include <QRect>
#include <QtDebug>
#include <QSet>

#include "globaldef.h"

typedef struct MainWindowConfig{
    MainWindowConfig()
        : is_inprivate_(false), always_on_top_(false), initially_hidden_(false)
    {
    }
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

    //
    QList<QString> load_after_created_;
}MainWndCfg;

class MainWindow;
class InprivatePopup;
class AppCfgWidget;
class FullscnHint;
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

    MainWindow *activeWindow();

    static int newWndOffsetX;
    static int newWndOffsetY;
    static InprivatePopup* gInprivatePopup;
    static AppCfgWidget* gAppCfgWidget;
    static FullscnHint* gFullscrnWidget;
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

    void updatePrivateWndCount();
    void relocateWindow(MainWindow *window, const QRect &bounds);
private slots:
    void onBkmkMgrMenuCmd(BookmarkCmd cmd, const QVariant &data);
    void onAppCfgChanged();
    void onWndDestroyed(MainWindow *obj);
};

#endif // MAINWINDOWMANAGER_H
