#include "browser_window.h"
#include "message_loop/main_message_loop.h"

#include <include/wrapper/cef_helpers.h>

#include <QtDebug>
#include <QThread>
#include <QScreen>
#include <QApplication>


BrowserWindow::BrowserWindow(Delegate *delagate, const std::string &startup_url)
    : delegate_(delagate)
    , is_closing_(false)
{
    DCHECK(delegate_);
    client_handler_ = new CefClientHandler(this, startup_url);
}

BrowserWindow::~BrowserWindow()
{
    qInfo()<<__FUNCTION__;
}

bool BrowserWindow::CreateBrowser(ClientWindowHandle parent_handle,
                                  const CefRect &rect,
                                  const CefBrowserSettings &settings,
                                  CefRefPtr<CefDictionaryValue> extra_info,
                                  CefRefPtr<CefRequestContext> request_context)
{
    REQUIRE_MAIN_THREAD();

    CefWindowInfo window_info;
    RECT wnd_rect = {rect.x, rect.y, rect.x + rect.width, rect.y + rect.height};
    window_info.SetAsChild(parent_handle, wnd_rect);

    return CefBrowserHost::CreateBrowser(window_info, client_handler_,
                                         client_handler_->startup_url(), settings,
                                         extra_info, request_context);
}

void BrowserWindow::GetPopupConfig(ClientWindowHandle temp_handle,
                                   CefWindowInfo &windowInfo,
                                   CefRefPtr<CefClient> &client,
                                   CefBrowserSettings &settings)
{
    CEF_REQUIRE_UI_THREAD();
    client = client_handler_;

    auto screen = qApp->primaryScreen();
    auto size = screen->availableSize();

    // The window will be properly sized after the browser is created.
#if defined(OS_WIN)
    RECT rect{0,0,size.width(),size.height()};
    windowInfo.SetAsChild(temp_handle, rect);
#elif defined(OS_LINUX)
    CefRect rect(0, 0, size.width(), size.height());
    windowInfo.SetAsChild(temp_handle, rect);
#endif
}

void BrowserWindow::ShowPopup(ClientWindowHandle parent_handle,
                              int x,
                              int y,
                              size_t width,
                              size_t height)
{
    REQUIRE_MAIN_THREAD();
    Q_UNUSED(parent_handle);
    Q_UNUSED(x);
    Q_UNUSED(y);
    Q_UNUSED(width);
    Q_UNUSED(height);
}

CefRefPtr<CefBrowser> BrowserWindow::GetBrowser() const
{
    REQUIRE_MAIN_THREAD();
    return browser_;
}

bool BrowserWindow::IsClosing() const
{
    REQUIRE_MAIN_THREAD();
    return is_closing_;
}

void BrowserWindow::onBrowserComfirmClose()
{
    client_handler_->DetachDelegate();
}

void BrowserWindow::onBrowserForgroundTab(CefWindowInfo &windowInfo,
                                              CefRefPtr<CefClient> &client,
                                              CefBrowserSettings &settings)
{
    REQUIRE_MAIN_THREAD();
    delegate_->onBrowserWindowNewForgroundPage(windowInfo,
                                               client,
                                               settings);
}

void BrowserWindow::onBrowserPopupWnd(const CefPopupFeatures &popupFeatures,
                                      CefWindowInfo &windowInfo,
                                      CefRefPtr<CefClient> &client,
                                      CefBrowserSettings &settings)
{
    REQUIRE_MAIN_THREAD();
    delegate_->onBrowserWndPopupWnd(popupFeatures,
                                    windowInfo,
                                    client,
                                    settings);
}

void BrowserWindow::OnBrowserCreated(CefRefPtr<CefBrowser> browser)
{
    REQUIRE_MAIN_THREAD();
    DCHECK(!browser_);

    browser_ = browser;

    delegate_->OnBrowserCreated();
}

void BrowserWindow::OnBrowserClosing(CefRefPtr<CefBrowser> browser)
{
    REQUIRE_MAIN_THREAD();
    DCHECK_EQ(browser->GetIdentifier(), browser_->GetIdentifier());
    // 修改标志
    is_closing_ = true;

    // 通知到Qt的窗口
    delegate_->OnBrowserWindowClosing();
}

void BrowserWindow::onBrowserAddressChange(const std::string &url)
{
    REQUIRE_MAIN_THREAD();

    delegate_->onBrowserWindowAddressChange(url);
}

void BrowserWindow::onBrowserTitleChange(const std::string &title)
{
     REQUIRE_MAIN_THREAD();

     delegate_->onBrowserWindowTitleChange(title);
}

void BrowserWindow::onBrowserStatusMessage(const std::string &msg)
{
    REQUIRE_MAIN_THREAD();

    delegate_->onBrowserWindowStatusMessage(msg);
}

void BrowserWindow::onBrowserFaviconChange(CefRefPtr<CefImage> image,
                                           const std::string &url)
{
    REQUIRE_MAIN_THREAD();
    delegate_->onBrowserWindowFaviconChange(image, url);
}

void BrowserWindow::onBrowerLoadStart(CefLoadHandler::TransitionType transition_type)
{
    REQUIRE_MAIN_THREAD();
    delegate_->onBrowerWindowLoadStart(transition_type);
}

void BrowserWindow::onBrowerLoadEnd(int httpStatusCode)
{
    REQUIRE_MAIN_THREAD();
    delegate_->onBrowerWindowLoadEnd(httpStatusCode);
}

void BrowserWindow::onBrowserLoadingStateChange(bool isLoading,
                                                bool canGoBack,
                                                bool canGoForward)
{
    REQUIRE_MAIN_THREAD();

    delegate_->onBrowserWindowLoadingStateChange(isLoading,canGoBack,canGoForward);
}

void BrowserWindow::onBrowserGotFocus(CefRefPtr<CefBrowser> browser)
{
    REQUIRE_MAIN_THREAD();
    delegate_->OnBrowserGotFocus();
}
