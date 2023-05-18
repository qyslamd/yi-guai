#ifndef BROWSER_PAGE_H
#define BROWSER_PAGE_H

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
class FindBar;

///
/// \brief The BrowserPage class
///
class BrowserPage : public QMainWindow
{
    Q_OBJECT
public:
    explicit BrowserPage(const QString &url, QWidget *parent = nullptr);
    BrowserPage(CefQWidget *browser, QWidget *parent = nullptr);
    ~BrowserPage();
    bool eventFilter(QObject *obj, QEvent *ev) override;

    CefQWidget* getBrowserWidget();

    QString url() const;
    QString title() const {return title_;}
    QPixmap favicon() const {return favicon_; }
    bool isLoading() const {return isLoading_;}
    bool canGoBack() const {return canGoBack_;}
    bool canGoForward() const {return canGoForward_;}
    bool isFinding() const {return isFinding_;}

    QString editedTxt() const {return edited_txt_;}
    bool edited() const {return edited_flag_;}

    void setEditedText(const QString &txt);
    void showSiteInfomation(const QPoint &pos);
    void showZoomBar(const QPoint &pos);
    void openDevTool();

signals:
    void pageCmd(PageCmd cmd, const QVariant &data);
    void newPage(BrowserPage *page);
    void browserShortcut(CefShortcutCmd cmd);
protected:
    void closeEvent(QCloseEvent *event) override;
private:
    QVBoxLayout *main_layout_ = nullptr;
    CefQWidget *browser_widget_ = nullptr;
    QDockWidget *dock_dev_tool_ = nullptr;
    SiteInfoPopup *site_info_popup_ = nullptr;
    ZoomPopup *zoom_popup_ = nullptr;
    QTimer *zoom_bar_timer_ = nullptr;
    FindBar *find_bar_ = nullptr;

    QString url_;
    QString title_;
    QPixmap favicon_;
    bool isLoading_;
    bool canGoBack_;
    bool canGoForward_;
    bool isFinding_ = false;

    bool edited_flag_ = false;
    QString edited_txt_;
private:
    void initBrowser();
    void initOthers();
    void moveFindBar();
private slots:
    void onBrowserDevTool(CefQWidget *devTool);
    void onDockDevToolTopLevelChanged(bool isFloating);
    void onDockDevToolLocChanged(Qt::DockWidgetArea area);
    void onDevToolShortcut(CefQWidget *devTool, CefShortcutCmd cmd);

private slots:
    void onZoomBarTimer();
};

#endif // BROWSER_PAGE_H
