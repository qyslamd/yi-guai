#ifndef CEF_QWIDGET_H
#define CEF_QWIDGET_H

#include "browser/cef_client_handler.h"
#include <QWidget>
#include <QWindow>
#include <QScopedPointer>
#include <QBackingStore>
#include <memory>
#include "globaldef.h"

class QHBoxLayout;
class QVBoxLayout;
class BrowserPage;
class StyledMenu;
class EmbededWindow;
class CefQWidget : public QWidget, public CefClientHandler::Delegate
{
    Q_OBJECT
public:
    explicit CefQWidget(const QString &url, QWidget *parent = nullptr);
    ~CefQWidget() override;

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
    void browserShortcut(CefShortcutCmd cmd);
    void devToolShortcut(CefShortcutCmd cmd);

    // ClientHandler::Delegate methods.
protected:
    void onBrowserBeforeContextMenu(CefRefPtr<CefBrowser> browser,
                                    CefRefPtr<CefFrame> frame,
                                    CefRefPtr<CefContextMenuParams> params,
                                    CefRefPtr<CefMenuModel> model) override;
    void onBrowserForgroundTab(CefWindowInfo &windowInfo,
                               CefRefPtr<CefClient> &client,
                               CefBrowserSettings &settings) override;
    void onBrowserPopupWnd(const CefPopupFeatures &popupFeatures,
                           CefWindowInfo &windowInfo,
                           CefRefPtr<CefClient> &client,
                           CefBrowserSettings &settings) override;
    void onBrowserDeveTools(CefWindowInfo& windowInfo,
                                  CefRefPtr<CefClient>& client,
                                  CefBrowserSettings& settings) override;
    void OnBrowserCreated(CefRefPtr<CefBrowser> browser) override;
    void OnBrowserClosing(CefRefPtr<CefBrowser> browser) override;
    void onBrowserAddressChange(const std::string &url) override;
    void onBrowserTitleChange(const std::string &title) override;
    void onBrowserFullscreenChange(bool fullscreen) override;
    void onBrowserStatusMessage(const std::string &meg) override;
    void onBrowserFaviconChange(CefRefPtr<CefImage> image, const std::string &url) override;
    void onBrowerLoadStart(CefLoadHandler::TransitionType transition_type) override;
    void onBrowerLoadEnd(int httpStatusCode) override;
    void onBrowserLoadingProgressChange(CefRefPtr<CefBrowser> browser,
                                        double progress) override;
    void onBrowserLoadingStateChange(bool isLoading, bool canGoBack, bool canGoForward) override;
    void onBrowserGotFocus(CefRefPtr<CefBrowser> browser) override;
    // browser keyboard event
    bool onBrowserPreKeyEvent(const CefKeyEvent &event,
                              CefEventHandle os_event,
                              bool *is_keyboard_shortcut) override;
    bool onBrowserKeyEvent(const CefKeyEvent &event,
                           CefEventHandle os_event) override;

    // QWidget interface
protected:
    void resizeEvent(QResizeEvent *event) override;
    void closeEvent(QCloseEvent *event) override;
    void showEvent(QShowEvent *event) override;

private:
    void initUi();
    void initContextMenu();
    bool CreateBrowser(const CefRect& rect,
                       const CefBrowserSettings& settings,
                       CefRefPtr<CefDictionaryValue> extra_info,
                       CefRefPtr<CefRequestContext> request_context);
    void resizeBrowser(const QSize &size = QSize());
    void GetPopupConfig(CefWindowInfo &windowInfo,
                        CefRefPtr<CefClient> &client,
                        CefBrowserSettings &settings);
    void dealCefKeyEvent(const CefKeyEvent &event,
                         CefEventHandle os_event,
                         bool *is_keyboard_shortcut,
                         bool isPre = true);
private:
    CefQWidget(CefWindowInfo &windowInfo,
               CefRefPtr<CefClient> &client,
               CefBrowserSettings &settings,
               QWidget *parent = nullptr);
    enum BrowserState{
        Empty,
        Creating,
        Created
    };
    bool is_closing_ = false;
    bool newly_created_ = true;
    bool is_dev_tool_ = false;
    BrowserState browser_state_ = Empty;
    CefRefPtr<CefBrowser> browser_;
    CefRefPtr<CefClientHandler> client_handler_;

    QString url_;
    QString title_;
    QWindow *window_;
    QWidget *qwindow_containter_;
    QVBoxLayout *layout_;

    StyledMenu *browser_context_menu_ = nullptr;
    QAction *action_back_;
    QAction *action_forward_;
    QAction *action_reload_;
    QAction *action_reload_ignore_cache_;
    QAction *action_frame_reload_;
    QAction *action_save_as_;
    QAction *action_print_;
    QAction *action_view_source_;
    QAction *action_view_frame_source_;
    QAction *action_inspect_element_;

    QAction *action_open_link_;
    QAction *action_open_link_window_;
    QAction *action_open_link_incognito_;
    QAction *action_copy_link_;
};


class EmbededWindow : public QWindow
{
    Q_OBJECT
public:
    EmbededWindow(QWindow *parent);

    bool event(QEvent *event) override;
protected:
    void resizeEvent(QResizeEvent *event) override;
    void exposeEvent(QExposeEvent *event) override;
private:
    void renderNow();
private:
    QScopedPointer<QBackingStore> store_;
};

#endif // CEF_QWIDGET_H
