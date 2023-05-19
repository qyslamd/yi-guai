#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>
#include <QFrame>

#include "browser/cef_client_handler.h"
#include "managers/MainWindowManager.h"
#include "widgets/QtWinFrameless.h"
#include "widgets/QtWinFramelessOld.h"
#include "globaldef.h"

class QFrame;
class QVBoxLayout;
class QHBoxLayout;
class QToolButton;
class TabPageToolBar;
class NavigateToolBar;
class BookmarkBar;
class NotificationBar;
class QStackedWidget;
class QStatusBar;
class BrowserPage;
class Tab_Thumbnail_Widget;

class AddBkmkPopup;
class BookmarkWidget;
class HistoryWidget;
class DownloadWidget;
class PopupGeneral;
class UserInfoPopup;
class InprivatePopup;
class QPropertyAnimation;
class AboutDialog;

class MainWindow : public QtWinFramelessWindow
{
    Q_OBJECT
public:
    explicit MainWindow(const MainWindowConfig &cfg, QWidget *parent = nullptr);
    explicit MainWindow(BrowserPage *page, QWidget *parent = nullptr);
    ~MainWindow();

    int AddNewPage(const QString &url, bool switchTo = false);
    int AddNewPage(BrowserPage *page);
    void NavigateInCurPage(const QString &url);
    bool isInprivate() const {return created_cfg_.is_inprivate_;}
    void updatePreference();
signals:
    void windowStateChanged(Qt::WindowStates state, const QVariant &data);
#ifdef Q_OS_WIN
    void dwmColorChanged();
#endif
public:
    // QObject interface
    bool eventFilter(QObject *obj, QEvent *event) override;
public slots:
    void onInpWndCntChanged();
    void onBookmarkCmd(BookmarkCmd cmd, const QVariant &para);
protected:
    void showEvent(QShowEvent *event) override;
    void closeEvent(QCloseEvent *evnet) override;
    void changeEvent(QEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void moveEvent(QMoveEvent *event) override;
#ifdef Q_OS_WIN
    // QtWinFrameless interface
    bool hitTestCaption(const QPoint &gPos) override;
#endif
private:
    QAction *ac_shortcut_zoomout_;
    QAction *ac_shortcut_resetzoom_;
    QAction *ac_shortcut_zoomin_ ;
    QAction *ac_shortcut_fullscn_;
    QAction *ac_shortcut_devtool_;

    MainWindowConfig created_cfg_;

    /*参见：QLayout::setMenuBar(QWidget *menuBar) */
    QWidget *widget_north_ = nullptr;
    QVBoxLayout *widget_north_layout_ = nullptr;
    QMenuBar *menu_bar_ = nullptr;      /*MacOS需要菜单栏*/
    TabPageToolBar *tab_bar_ = nullptr;   /*标签栏*/
    NavigateToolBar *navi_bar_ = nullptr; /*导航栏*/
    BookmarkBar *bookmark_bar_ = nullptr; /*书签栏*/
    NotificationBar *notify_bar_ = nullptr; /*通知栏*/

    QVBoxLayout *central_widget_layout_ = nullptr;
    QHBoxLayout *central_area_layout_ = nullptr;

    QWidget *widget_west_ = nullptr;
    QWidget *widget_south_ = nullptr;
    QWidget *widget_east_ = nullptr;
    QVBoxLayout *widget_east_layout_ = nullptr;

    QStackedWidget *stack_browsers_ = nullptr;    /*浏览器窗口栈*/
    Tab_Thumbnail_Widget *tab_thumbnail_ = nullptr; /*标签页预览窗口*/
    QPropertyAnimation *tab_thumbnail_anime_ = nullptr;   /* tab预览窗口移动动画*/

    AddBkmkPopup *add_favorite_popup_;    /*添加书签 popup*/

    PopupGeneral *popup_history_ = nullptr;
    HistoryWidget *history_widget_ = nullptr;   /*历史记录widget*/

    PopupGeneral *popup_bookmark_ = nullptr;
    BookmarkWidget *bookmark_widget_ = nullptr;   /*书签 widget*/

    PopupGeneral *popup_download_ = nullptr;
    DownloadWidget *download_widget_ = nullptr;   /*下载 widget*/

    UserInfoPopup *userinfo_popup_ = nullptr; /*用户信息 popup*/

    AboutDialog *about_dialog_ = nullptr;

    bool first_browser_created_ = false; /*首个浏览器创建完成标志*/
    bool window_closing_ = false;   /*窗口正在关闭标志*/
    bool right_closing_ = false;    /*窗口正在关闭右侧标签页标志*/

private:
    void initQtShortcut();
    void initUi();
    void setAppearance();
    void initSignalSlot();

    void initPage(BrowserPage *page);
    BrowserPage *CurrentPage();
    int CurrentPageIndex();
    BrowserPage *GetPage(int index);

private slots:
    void onScreenChanged(QScreen *screen);
    void onTabBarCurrentChanged(int index);
    void onTabBarCloseRequested(int index);
    void onTabBarTabMoved(int from, int to);
    void onTabBarMenuTriggered(TabBarCmd cmd, const QVariant &para);
    void onNaviBarCmd(NaviBarCmd cmd, const QVariant &para);
    void onPageCmd(PageCmd cmd, const QVariant &para);
    void onShowTabThumnail(const QPoint &g_pos, const int index);
    void onBrowserShortcut(CefShortcutCmd cmd);
    void onHistoryWidgetCmd(HistoryCmd cmd, const QVariant &para);
    void onPinOrCloseHistoryWidget();
    void onPinOrCloseBookmarkWidget();

private:
    void onGoBack();
    void onGoForward();
    void onHomepage();
    void onRefresh();
    void onAddFavorite();
    void onZoomOut();
    void onZoomReset();
    void onZoomIn();
    void pageZoomChanged();
    void onFullScreen();
    void onDevTool();
    void onShowHistory();
    void onShowBookmark();
    void onShowDownload();
    void onShowInprivate();
    void onShowUser();
    void onPrint();
    void onSettings();
    void onStatusMessage(const QString &msg);
    void onWindowStateChanged();
    void onNormalMax();
    void onTabSwitch();
    void addRecently(BrowserPage *page);
};
#endif // MAINWINDOW_H
