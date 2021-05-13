#ifndef PAGE_H
#define PAGE_H

#include <QMainWindow>
#include "globaldef.h"

class QHBoxLayout;
class QVBoxLayout;
class CefQWidget;
class QDockWidget;
class SiteInfoPopup;

///
/// \brief The Page class
/// 每一个独立的标签页
class Page : public QMainWindow
{
    Q_OBJECT
public:
    explicit Page(const QString &url, QWidget *parent = nullptr);
    Page(CefQWidget *browser, QWidget *parent = nullptr);

    CefQWidget* getBrowserWidget();

    QString url() const {return url_;}
    QString title() const {return title_;}
    QPixmap favicon() const {return favicon_; }
    bool isLoading() const {return isLoading_;}
    bool canGoBack() const {return canGoBack_;}
    bool canGoForward() const {return canGoForward_;}


    void showSiteInfomation(const QRect &rect);

signals:
    void pageCmd(PageCmd cmd, const QVariant &data);
    void newPage(Page *page);
protected:
    void closeEvent(QCloseEvent *event) override;
private:
    QVBoxLayout *main_layout_ = nullptr;   /*整体布局*/
    CefQWidget *browser_widget_ = nullptr;    /*浏览器QWidget*/
    QDockWidget *dock_dev_tool_ = nullptr;    /*开发者工具停靠窗口*/
    SiteInfoPopup *site_info_popup_ = nullptr;  /*站点信息查看*/

    QString url_;
    QString title_;
    QPixmap favicon_;
    bool isLoading_;
    bool canGoBack_;
    bool canGoForward_;
private:
    void initBrowser();
};

#endif // PAGE_H
