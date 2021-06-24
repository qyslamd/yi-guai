#ifndef CEF_QWIDGET_H
#define CEF_QWIDGET_H

#include <QWidget>
#include <QWindow>
#include <memory>
#include "browser/browser_window.h"
#include "globaldef.h"

class QHBoxLayout;
class QVBoxLayout;

class Page;
class CefQWidget : public QWidget, public BrowserWindow::Delegate
{
    Q_OBJECT
public:
    explicit CefQWidget(const QString &url, QWidget *parent = nullptr);
    CefQWidget(CefWindowInfo &windowInfo, CefRefPtr<CefClient> &client, CefBrowserSettings &settings, QWidget *parent = nullptr);
    ~CefQWidget();

    // QWidget interface
    QSize sizeHint() const override;

    void Navigate(const QString &url);
    void GoBack();
    void GoForward();
    void Refresh();
    void MuteAudio();
    void StopLoading();
    void ZoomOut();
    void ZoomIn();
    void ZoomReset();
    double ZoomLevel();
    void Print();
    void ShowDevTool(const QPoint &pos);

    CefWindowHandle BrowserWindowHandle();
    bool isDevTool() const { return is_dev_tool_; }

signals:
    void browserNeedSize();
    void browserCreated();
    void browserClosing();
    void browserNewForgroundPage(CefQWidget *newPage);
    void browserAddressChange(const QString &url);
    void browserTitleChange(const QString &title);
    void browserFullScnChange(bool fullscreen);
    void browserStatusMessage(const QString &msg);
    void browserFaviconChange(const QPixmap &pix);
    void browserLoadStart(CefLoadHandler::TransitionType transition_type);
    void browserLoadEnd(int httpStatusCode);
    void browserLoadingProgress(double progress);
    void browserLoadingStateChange(bool isLoading, bool canGoBack, bool canGoForward);
    void browserFocusChange(bool getfucos);
    void browserDevTool(CefQWidget *devTool);
    void browserShortcut(const CefKeyEvent &event,
                         CefEventHandle os_event);
    void devToolShortcut(const CefKeyEvent &event,
                         CefEventHandle os_event);

public slots:
    void onTopLevelWindowStateChanged(Qt::WindowStates state, const QVariant &data);
    // BrowserWindow::Delegate interface
protected:
    void onBrowserWndNewForgroundPage(CefWindowInfo &windowInfo,
                                         CefRefPtr<CefClient> &client,
                                         CefBrowserSettings &settings) override;
    void onBrowserWndPopupWnd(const CefPopupFeatures &popupFeatures,
                              CefWindowInfo &windowInfo,
                              CefRefPtr<CefClient> &client,
                              CefBrowserSettings &settings) override;
    void onBrowserWndDevTools(CefWindowInfo& windowInfo,
                                  CefRefPtr<CefClient>& client,
                                  CefBrowserSettings& settings) override;
    void OnBrowserCreated(CefRefPtr<CefBrowser> browser) override;
    void OnBrowserWindowClosing() override;
    void onBrowserWindowAddressChange(const std::string &url) override;
    void onBrowserWindowTitleChange(const std::string &title) override;
    void onBrowserWndFullscreenChange(bool fullscreen) override;
    void onBrowserWindowStatusMessage(const std::string &meg) override;
    void onBrowserWindowFaviconChange(CefRefPtr<CefImage> image, const std::string &url) override;
    void onBrowerWindowLoadStart(CefLoadHandler::TransitionType transition_type) override;
    void onBrowerWindowLoadEnd(int httpStatusCode) override;
    void onBrowserWndLoadingProgressChange(double progress) override;
    void onBrowserWindowLoadingStateChange(bool isLoading, bool canGoBack, bool canGoForward) override;
    void OnBrowserGotFocus() override;
    // browser keyboard event
    bool onBrowserWndPreKeyEvent(const CefKeyEvent &event,
                                 CefEventHandle os_event,
                                 bool *is_keyboard_shortcut) override;
    bool onBrowserWndKeyEvent(const CefKeyEvent &event,
                              CefEventHandle os_event) override;

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
    bool is_dev_tool_ = false;
    BrowserState browser_state_ = Empty;
    CefRefPtr<CefBrowser> browser_;

    QString url_;
    QString title_;
    QWindow *window_;
    scoped_ptr<BrowserWindow> browser_window_;
    QWidget *qwindow_containter_;
    QVBoxLayout *layout_;

private:
    void initUi();
    void resizeBorser(const QSize &size = QSize());
    void dealCefKeyEvent(const CefKeyEvent &event,
                         CefEventHandle os_event,
                         bool *is_keyboard_shortcut,
                         bool isPre = true);
};

#endif // CEF_QWIDGET_H
