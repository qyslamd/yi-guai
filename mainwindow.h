#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>

#include "browser/cef_client_handler.h"

class QVBoxLayout;
class TabBar;
class NaviBar;
class BookmarkBar;
class QStackedWidget;
class QStatusBar;
class CefQWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    int addOneBrowserPage(const QString &url, bool switchTo = false);
    // QObject interface
    bool event(QEvent *e) override;
signals:
    void windowStateChanged(Qt::WindowStates state, const QVariant &data);
protected:
    void closeEvent(QCloseEvent *evnet) override;
    void changeEvent(QEvent *event) override;
private slots:
    void onTabPageCloseRequested(int index);
private:
    QVBoxLayout *layout_;   /*整个窗口的布局*/

    TabBar *tab_bar_ = nullptr;   /*标签栏*/
    NaviBar *navi_bar_ = nullptr; /*导航栏*/
    BookmarkBar *bookmark_bar_ = nullptr; /*书签栏*/
    QStackedWidget *stack_browsers_ = nullptr;    /*浏览器窗口栈*/
    QStatusBar *status_bar_ = nullptr;    /*状态栏*/

    bool closing_ = false;  /*窗口是否正在关闭*/

    void initUi();
    void initSignalSlot();
    void initPage(CefQWidget *page);
    CefQWidget *GetActivePage();
};
#endif // MAINWINDOW_H
