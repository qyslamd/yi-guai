#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>

#include "browser/cef_client_handler.h"
#include "globaldef.h"

class QVBoxLayout;
class QHBoxLayout;
class QToolButton;
class TabBar;
class NaviBar;
class BookmarkBar;
class QStackedWidget;
class QStatusBar;
class Page;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    int addNewPage(const QString &url, bool switchTo = false);
    int addNewPage(Page *page);
    // QObject interface
    bool event(QEvent *e) override;
signals:
    void windowStateChanged(Qt::WindowStates state, const QVariant &data);
protected:
    void closeEvent(QCloseEvent *evnet) override;
    void changeEvent(QEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
private:
    QVBoxLayout *layout_;   /*整个窗口的布局*/

    QHBoxLayout* tabbar_layout_;    /*标签栏布局*/
    QToolButton *btn_dock_tabs_ = nullptr;    /*停靠标签按钮*/
    TabBar *tab_bar_ = nullptr;   /*标签栏*/
    QToolButton *btn_add_page_ = nullptr; /*添加新标签按钮*/
    NaviBar *navi_bar_ = nullptr; /*导航栏*/
    BookmarkBar *bookmark_bar_ = nullptr; /*书签栏*/
    QStackedWidget *stack_browsers_ = nullptr;    /*浏览器窗口栈*/

    bool closing_ = false;  /*窗口是否正在关闭*/

private:
    void initUi();
    void setAppearance();

    void initSignalSlot();
    void initPage(Page *page);
    Page *GetActivePage();

private slots:
    void onTabBarCurrentChanged(int index);
    void onTabBarCloseRequested(int index);
    void onTabBarTabMoved(int from, int to);
    void onNaviBarCmd(NaviBarCmd cmd, const QVariant &para);
    void onPageCmd(PageCmd cmd, const QVariant &data);
};
#endif // MAINWINDOW_H
