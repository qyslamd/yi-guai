#ifndef CEF_QWIDGET_H
#define CEF_QWIDGET_H

#include <QWidget>
#include <QWindow>
#include <memory>
#include "browser/browser_window.h"

class QHBoxLayout;

class Page;
class CefQWidget : public QWidget, public BrowserWindow::Delegate
{
    Q_OBJECT
public:
    explicit CefQWidget(const QString &startup_url, QWidget *parent = nullptr);
    CefQWidget(CefWindowInfo &windowInfo, CefRefPtr<CefClient> &client, CefBrowserSettings &settings, QWidget *parent = nullptr);
    ~CefQWidget();

    void Navigate(const QString &url);
    void GoBack();
    void GoForward();
    void Refresh();
    void StopLoading();

signals:
    void browserClosing();
    void browserNewForgroundPage(CefQWidget *newPage);
    void browserAddressChange(const QString &url);
    void browserTitleChange(const QString &title);
    void browserStatusMessage(const QString &msg);
    void browserFaviconChange(const QPixmap &pix);
    void browserLoadStart(CefLoadHandler::TransitionType transition_type);
    void browserLoadEnd(int httpStatusCode);
    void browserLoadingStateChange(bool isLoading, bool canGoBack, bool canGoForward);
    void browserFocusChange(bool getfucos);

public slots:
    void onTopLevelWindowStateChanged(Qt::WindowStates state, const QVariant &data);
    // BrowserWindow::Delegate interface
protected:
    void onBrowserWindowNewForgroundPage(CefWindowInfo &windowInfo, CefRefPtr<CefClient> &client, CefBrowserSettings &settings) override;
    void onBrowserWndPopupWnd(const CefPopupFeatures &popupFeatures, CefWindowInfo &windowInfo,
                              CefRefPtr<CefClient> &client,
                              CefBrowserSettings &settings) override;
    void OnBrowserCreated() override;
    void OnBrowserWindowClosing() override;
    void onBrowserWindowAddressChange(const std::string &url) override;
    void onBrowserWindowTitleChange(const std::string &title) override;
    void onBrowserWindowStatusMessage(const std::string &meg) override;
    void onBrowserWindowFaviconChange(CefRefPtr<CefImage> image, const std::string &url) override;
    void onBrowerWindowLoadStart(CefLoadHandler::TransitionType transition_type) override;
    void onBrowerWindowLoadEnd(int httpStatusCode) override;
    void onBrowserWindowLoadingStateChange(bool isLoading, bool canGoBack, bool canGoForward) override;
    void OnBrowserGotFocus() override;
    // QWidget interface
protected:
    void resizeEvent(QResizeEvent *event) override;
    void closeEvent(QCloseEvent *event) override;

private:
    enum BrowserState{
        Empty,
        Creating,
        Created
    };
    bool newly_created_ = true;
    BrowserState browser_state_ = Empty;

    QString url_;
    QWindow *window_;
    scoped_ptr<BrowserWindow> browser_window_;
    QWidget *qwindow_containter_;
    QHBoxLayout *layout_;

private:
    void initUi();
    void resizeBorser(const QSize &size = QSize());

};

#endif // CEF_QWIDGET_H
