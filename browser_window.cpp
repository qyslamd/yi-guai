#include "browser_window.h"
#include "message_loop/main_message_loop.h"

#include <include/wrapper/cef_helpers.h>

#include <QtDebug>
#include <QThread>


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

void BrowserWindow::CreateBrowser(ClientWindowHandle parent_handle,
                                  const CefRect &rect,
                                  const CefBrowserSettings &settings,
                                  CefRefPtr<CefDictionaryValue> extra_info,
                                  CefRefPtr<CefRequestContext> request_context)
{
    REQUIRE_MAIN_THREAD();

    CefWindowInfo window_info;
    RECT wnd_rect = {rect.x, rect.y, rect.x + rect.width, rect.y + rect.height};
    window_info.SetAsChild(parent_handle, wnd_rect);

    //    if (GetWindowLongPtr(parent_handle, GWL_EXSTYLE) & WS_EX_NOACTIVATE) {
    //      // Don't activate the browser window on creation.
    //      window_info.ex_style |= WS_EX_NOACTIVATE;
    //    }

    if(!CefBrowserHost::CreateBrowser(window_info, client_handler_,
                                  client_handler_->startup_url(), settings,
                                  extra_info, request_context))
    {
       qInfo()<<__FUNCTION__<<"CefBrowserHost::CreateBrowser failed!";
    }else{
        qInfo()<<__FUNCTION__<<"CefBrowserHost::CreateBrowser ok!";
    }
}

void BrowserWindow::GetPopupConfig(ClientWindowHandle temp_handle,
                                   CefWindowInfo &windowInfo,
                                   CefRefPtr<CefClient> &client,
                                   CefBrowserSettings &settings)
{
    CEF_REQUIRE_UI_THREAD();
    qDebug()<<__FUNCTION__;

    client = client_handler_;

    // The window will be properly sized after the browser is created.
#if defined(OS_WIN)
    RECT rect;
    rect.top = 0;
    rect.left = 0;
    rect.bottom = 200;
    rect.right = 200;
    windowInfo.SetAsChild(temp_handle, rect);
#elif defined(OS_LINUX)
    CefRect rect(0, 0, width, height);
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

    //    HWND hwnd = GetWindowHandle();
    //    if (hwnd) {
    //        SetParent(hwnd, parent_handle);
    //        SetWindowPos(hwnd, NULL, x, y, static_cast<int>(width),
    //                     static_cast<int>(height), SWP_NOZORDER | SWP_NOACTIVATE);

    //        const bool no_activate =
    //                GetWindowLongPtr(parent_handle, GWL_EXSTYLE) & WS_EX_NOACTIVATE;
    //        ShowWindow(hwnd, no_activate ? SW_SHOWNOACTIVATE : SW_SHOW);
    //    }
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

void BrowserWindow::onBrowserNewForgroundPage(CefWindowInfo &windowInfo,
                                              CefRefPtr<CefClient> &client,
                                              CefBrowserSettings &settings)
{
    REQUIRE_MAIN_THREAD();
    delegate_->onBrowserWindowNewForgroundPage(windowInfo,
                                               client,
                                               settings);
}

void BrowserWindow::OnBrowserCreated(CefRefPtr<CefBrowser> browser)
{
    REQUIRE_MAIN_THREAD();
    DCHECK(!browser_);

    browser_ = browser;

    delegate_->OnBrowserCreated(browser);
}

void BrowserWindow::OnBrowserClosing(CefRefPtr<CefBrowser> browser)
{
    REQUIRE_MAIN_THREAD();
    DCHECK_EQ(browser->GetIdentifier(), browser_->GetIdentifier());
    is_closing_ = true; // 修改标志，通知到窗口

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
