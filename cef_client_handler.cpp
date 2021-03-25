#include "cef_client_handler.h"
#include "message_loop/main_message_loop.h"

#include <include/wrapper/cef_closure_task.h>
#include <include/wrapper/cef_helpers.h>

#include <QtDebug>
#include <QDateTime>
#include <QApplication>


int CefClientHandler::total_created_ = 0;

CefClientHandler::CefClientHandler(Delegate *delegate,
                                   const std::string &startup_url)
    : startup_url_(startup_url)
    , delegate_(delegate)
{

}

CefClientHandler::~CefClientHandler()
{
    qInfo()<<__FUNCTION__;
}

void CefClientHandler::DetachDelegate()
{
    if (!CURRENTLY_ON_MAIN_THREAD()) {
        // Execute this method on the main thread.
        MAIN_POST_CLOSURE(base::Bind(&CefClientHandler::DetachDelegate, this));
        return;
    }

    DCHECK(delegate_);
    delegate_ = nullptr;
}

bool CefClientHandler::OnBeforePopup(
        CefRefPtr<CefBrowser> browser,
        CefRefPtr<CefFrame> frame,
        const CefString& target_url,
        const CefString& target_frame_name,
        CefLifeSpanHandler::WindowOpenDisposition target_disposition,
        bool user_gesture,
        const CefPopupFeatures& popupFeatures,
        CefWindowInfo& windowInfo,
        CefRefPtr<CefClient>& client,
        CefBrowserSettings& settings,
        CefRefPtr<CefDictionaryValue>& extra_info,
        bool* no_javascript_access)
{
    CEF_REQUIRE_UI_THREAD();
    // notify to outside
    // return true to cancel create

    /*
     * WOD_UNKNOWN,
      WOD_CURRENT_TAB,
      WOD_SINGLETON_TAB,
      WOD_NEW_FOREGROUND_TAB,
      WOD_NEW_BACKGROUND_TAB,
      WOD_NEW_POPUP,
      WOD_NEW_WINDOW,
      WOD_SAVE_TO_DISK,
      WOD_OFF_THE_RECORD,
      WOD_IGNORE_ACTION
     * */
    if(target_disposition == WOD_NEW_FOREGROUND_TAB)
    {
        NotifyBrowserNewForgroundPage(windowInfo, client, settings);
    }
    return false;
}

void CefClientHandler::OnAfterCreated(CefRefPtr<CefBrowser> browser)
{
    CEF_REQUIRE_UI_THREAD();
    total_created_++;
    browser_count_ += 1;

    qInfo()<<__FUNCTION__<<"Total browser count:"<<total_created_
          <<"handler browser count:"<<browser_count_;


    NotifyBrowserCreated(browser);
}

bool CefClientHandler::DoClose(CefRefPtr<CefBrowser> browser)
{
    CEF_REQUIRE_UI_THREAD();
    qInfo()<<__FUNCTION__<<"browser closing "<<QTime::currentTime();

    NotifyBrowserClosing(browser);

    // Return false to allow the close. For windowed browsers this will result in the OS close
    // event being sent.
    // Return true to pevent send close to Toplevel window
    return true;
}

void CefClientHandler::OnBeforeClose(CefRefPtr<CefBrowser> browser)
{
    CEF_REQUIRE_UI_THREAD();
    total_created_ --;
    browser_count_--;
    qInfo()<<__FUNCTION__<<"Total browser count:"<<total_created_;

    NotifyBrowserClosed(browser);
}

void CefClientHandler::NotifyBrowserCreated(CefRefPtr<CefBrowser> browser)
{
    if (!CURRENTLY_ON_MAIN_THREAD()) {
        // Execute this method on the main thread.
        MAIN_POST_CLOSURE(
                    base::Bind(&CefClientHandler::NotifyBrowserCreated, this, browser));
        return;
    }

    if (delegate_)
        delegate_->OnBrowserCreated(browser);
}

void CefClientHandler::NotifyBrowserClosing(CefRefPtr<CefBrowser> browser)
{
    if (!CURRENTLY_ON_MAIN_THREAD()) {
        // Execute this method on the main thread.
        MAIN_POST_CLOSURE(
                    base::Bind(&CefClientHandler::NotifyBrowserClosing, this, browser));
        return;
    }

    if (delegate_)
        delegate_->OnBrowserClosing(browser);
}

void CefClientHandler::NotifyBrowserClosed(CefRefPtr<CefBrowser> browser)
{
    if (!CURRENTLY_ON_MAIN_THREAD()) {
        // Execute this method on the main thread.
        MAIN_POST_CLOSURE(
                    base::Bind(&CefClientHandler::NotifyBrowserClosed, this, browser));
        return;
    }

//    if (delegate_)
//        delegate_->OnBrowserClosed(browser);
}

void CefClientHandler::NotifyBrowserAddressChange(CefRefPtr<CefBrowser> browser,
                                                  CefRefPtr<CefFrame> frame,
                                                  const CefString &url)
{
    if (!CURRENTLY_ON_MAIN_THREAD()) {
        // Execute this method on the main thread.
        MAIN_POST_CLOSURE(
                    base::Bind(&CefClientHandler::NotifyBrowserAddressChange, this, browser, frame, url));
        return;
    }

    if (delegate_)
        delegate_->onBrowserAddressChange(url);
}

void CefClientHandler::NotifyBroserTitleChange(CefRefPtr<CefBrowser> browser,
                                               const CefString &title)
{
    if (!CURRENTLY_ON_MAIN_THREAD()) {
        // Execute this method on the main thread.
        MAIN_POST_CLOSURE(
                    base::Bind(&CefClientHandler::NotifyBroserTitleChange, this, browser, title));
        return;
    }

    if (delegate_)
        delegate_->onBrowserTitleChange(title.ToString());
}


void CefClientHandler::OnAddressChange(CefRefPtr<CefBrowser> browser,
                                       CefRefPtr<CefFrame> frame,
                                       const CefString &url)
{
    CEF_REQUIRE_UI_THREAD();

    NotifyBrowserAddressChange(browser, frame, url);
}

void CefClientHandler::OnTitleChange(CefRefPtr<CefBrowser> browser,
                                     const CefString &title)
{
    CEF_REQUIRE_UI_THREAD();

    NotifyBroserTitleChange(browser, title);
}

void CefClientHandler::OnFaviconURLChange(CefRefPtr<CefBrowser> browser,
                                          const std::vector<CefString> &icon_urls)
{
}

void CefClientHandler::OnFullscreenModeChange(CefRefPtr<CefBrowser> browser,
                                              bool fullscreen)
{
}

bool CefClientHandler::OnTooltip(CefRefPtr<CefBrowser> browser,
                                 CefString &text)
{
    return false;
}

void CefClientHandler::OnStatusMessage(CefRefPtr<CefBrowser> browser, const CefString &value)
{
}

bool CefClientHandler::OnConsoleMessage(CefRefPtr<CefBrowser> browser, cef_log_severity_t level, const CefString &message, const CefString &source, int line)
{
    return false;
}

bool CefClientHandler::OnAutoResize(CefRefPtr<CefBrowser> browser, const CefSize &new_size)
{
    return false;
}

void CefClientHandler::OnLoadingProgressChange(CefRefPtr<CefBrowser> browser,
                                               double progress)
{

}

void CefClientHandler::NotifyBrowserNewForgroundPage(CefWindowInfo &windowInfo,
                                                     CefRefPtr<CefClient> &client,
                                                     CefBrowserSettings &settings)
{
//    if (!CURRENTLY_ON_MAIN_THREAD()) {
//        // Execute this method on the main thread.
//        MAIN_POST_CLOSURE(
//                    base::Bind(&CefClientHandler::NotifyBrowserNewForgroundPage, this,
//                               windowInfo,
//                               client,
//                               settings,
//                               extra_info));
//        return;
//    }

    if (delegate_)
        delegate_->onBrowserNewForgroundPage(windowInfo,
                                             client,
                                             settings);
}
