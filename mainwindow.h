#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>
#include <QFrame>

#include "browser/cef_client_handler.h"
#include "managers/MainWindowManager.h"
#include "globaldef.h"

class QVBoxLayout;
class QHBoxLayout;
class QToolButton;
class TabPagesBar;
class NaviBar;
class BookmarkBar;
class NotificationBar;
class QStackedWidget;
class QStatusBar;
class Page;
class Tab_Thumbnail_Widget;
class HistoryPopup;
class UserInfoPopup;
class AppCfgWidget;
class QPropertyAnimation;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:

    MainWindow(const MainWindowConfig &cfg, QWidget *parent = nullptr);
    ~MainWindow();

    int addNewPage(const QString &url, bool switchTo = false);
    int addNewPage(Page *page);
signals:
    void windowStateChanged(Qt::WindowStates state, const QVariant &data);
    void historyPopupVisibleChange(bool visible);
    void userInfoPopupVisibleChange(bool visible);
#ifdef Q_OS_WIN
    void dwmColorChanged();
#endif
public:
    // QObject interface
    bool event(QEvent *e) override;
    bool eventFilter(QObject *obj, QEvent *event) override;
protected:
    bool nativeEvent(const QByteArray &eventType, void *message, long *result) override;
    void closeEvent(QCloseEvent *evnet) override;
    void changeEvent(QEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
private:
    MainWindowConfig created_cfg_{false, false, false, QRect(), ""};

    /*参见：QLayout::setMenuBar(QWidget *menuBar)，*/
    QWidget *widget_north_ = nullptr;
    QVBoxLayout *widget_north_layout_ = nullptr;
    QMenuBar *menu_bar_ = nullptr;      /*MacOS需要菜单栏*/
    TabPagesBar *tab_bar_ = nullptr;   /*标签栏*/
    NaviBar *navi_bar_ = nullptr; /*导航栏*/
    BookmarkBar *bookmark_bar_ = nullptr; /*书签栏*/
    NotificationBar *notify_bar_ = nullptr; /*通知栏*/

    QVBoxLayout *central_widget_layout_ = nullptr;
    QHBoxLayout *central_area_layout_ = nullptr;

    QWidget *widget_west_ = nullptr;
    QWidget *widget_south_ = nullptr;
    QWidget *widget_east_ = nullptr;

    QStackedWidget *stack_browsers_ = nullptr;    /*浏览器窗口栈*/
    Tab_Thumbnail_Widget *tab_thumbnail_ = nullptr; /*标签页预览窗口*/
    QPropertyAnimation *tab_thumbnail_anime_ = nullptr;   /* tab预览窗口移动动画*/

    HistoryPopup *history_popup_ = nullptr; /*历史记录 popup*/
    UserInfoPopup *userinfo_popup_ = nullptr; /*用户信息 popup*/
    AppCfgWidget *app_cfg_widget_ = nullptr;

    bool window_closing_ = false;  /*窗口是否正在关闭*/

private:
    void initUi();
    void setAppearance();

    void initSignalSlot();
    void initPage(Page *page);
    Page *GetActivePage();
    Page *GetPage(int index);
    void onStatusMessage(const QString &msg);

private slots:
    void onTabBarCurrentChanged(int index);
    void onTabBarCloseRequested(int index);
    void onTabBarTabMoved(int from, int to);
    void onTabbarMenuTriggered(TabBarCmd cmd, const QVariant &para);
    void onNaviBarCmd(NaviBarCmd cmd, const QVariant &para);
    void onPageCmd(PageCmd cmd, const QVariant &para);
    void onShowTabThumnail(const QPoint &g_pos, const int index);
};
#endif // MAINWINDOW_H
