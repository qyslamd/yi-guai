#ifndef PAGE_H
#define PAGE_H

#include <QMainWindow>
#include "globaldef.h"

class QHBoxLayout;
class QVBoxLayout;
class CefQWidget;
class QDockWidget;

///
/// \brief The Page class
/// 每一个独立的标签页
class Page : public QMainWindow
{
    Q_OBJECT
public:
    explicit Page(const QString &startup_url, QWidget *parent = nullptr);
    Page(CefQWidget *browser, QWidget *parent = nullptr);

    CefQWidget* getBrowserWidget();
    bool IsLoading() const {return isLoading_;}
    bool CanGoBack() const {return canGoBack_;}
    bool CanGoForward() const {return canGoForward_;}
signals:
    void pageCmd(PageCmd cmd, const QVariant &data);
    void newPage(Page *page);
protected:
    void closeEvent(QCloseEvent *event) override;
private:
    QVBoxLayout *main_layout_ = nullptr;   /*整体布局*/
    CefQWidget *browser_widget_ = nullptr;    /*浏览器QWidget*/
    QDockWidget *dock_dev_tool_ = nullptr;    /*开发者工具停靠窗口*/

    QString url_;
    bool isLoading_;
    bool canGoBack_;
    bool canGoForward_;
private:
    void initUi();
    void initUi(const QString &url);
    void initBrowser();
};

#endif // PAGE_H
