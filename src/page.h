#ifndef PAGE_H
#define PAGE_H

#include <QMainWindow>
#include "cef_qwidget.h"
#include "globaldef.h"

class QHBoxLayout;
class QVBoxLayout;
class CefQWidget;
class QDockWidget;
class SiteInfoPopup;
class ZoomPopup;
class QTimer;

///
/// \brief The Page class
/// 每一个独立的标签页
class Page : public QMainWindow
{
    Q_OBJECT
public:
    explicit Page(const QString &url, QWidget *parent = nullptr);
    Page(CefQWidget *browser, QWidget *parent = nullptr);
    ~Page();
    bool eventFilter(QObject *obj, QEvent *ev) override;

    CefQWidget* getBrowserWidget();

    QString url() const;
    QString title() const {return title_;}
    QPixmap favicon() const {return favicon_; }
    bool isLoading() const {return isLoading_;}
    bool canGoBack() const {return canGoBack_;}
    bool canGoForward() const {return canGoForward_;}

    QString editedTxt() const {return edited_txt_;}
    bool edited() const {return edited_flag_;}

    void setEditedText(const QString &txt);
    void showSiteInfomation(const QPoint &pos);
    void showZoomBar(const QPoint &pos);
    void openDevTool();

signals:
    void pageCmd(PageCmd cmd, const QVariant &data);
    void newPage(Page *page);
//    void browserShortcut(const CefKeyEvent &event,
//                         CefEventHandle os_event);
protected:
    void closeEvent(QCloseEvent *event) override;
private:
    QVBoxLayout *main_layout_ = nullptr;   /*整体布局*/
    CefQWidget *browser_widget_ = nullptr;    /*浏览器QWidget*/
    QDockWidget *dock_dev_tool_ = nullptr;    /*开发者工具停靠窗口*/
    SiteInfoPopup *site_info_popup_ = nullptr;  /*站点信息查看*/
    ZoomPopup *zoom_popup_ = nullptr;   /*缩放比例调整 popup*/
    QTimer *zoom_bar_timer_ = nullptr;

    QString url_;
    QString title_;
    QPixmap favicon_;
    bool isLoading_;
    bool canGoBack_;
    bool canGoForward_;

    bool edited_flag_ = false;
    QString edited_txt_;
private:
    void initBrowser();
    void initOthers();
private slots:
    void onBrowserDevTool(CefQWidget *devTool);
    void onDockDevToolTopLevelChanged(bool isFloating);
    void onDockDevToolLocChanged(Qt::DockWidgetArea area);
//    void onDevToolShortcut(const CefKeyEvent &event,
//                           CefEventHandle);

private slots:
    void onZoomBarTimer();
};

#endif // PAGE_H
