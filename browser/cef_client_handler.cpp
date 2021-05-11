#include "cef_client_handler.h"
#include "message_loop/main_message_loop.h"

#include <include/wrapper/cef_closure_task.h>
#include <include/wrapper/cef_helpers.h>

#include <QtDebug>
#include <QApplication>
#include "dialogs/alertdialog.h"
#include "utils/util_qt.h"


int CefClientHandler::gBrowserCount = 0;

class ClientDownloadImageCallback : public CefDownloadImageCallback {
public:
    explicit ClientDownloadImageCallback(CefRefPtr<CefClientHandler> clientHandler)
        : clientHandler_(clientHandler) {}

    void OnDownloadImageFinished(const CefString& image_url,
                                 int http_status_code,
                                 CefRefPtr<CefImage> image) override {
        if (image)
            clientHandler_->NotifyBrowserFavicon(image, image_url);
        Q_UNUSED(http_status_code);
    }

private:
    CefRefPtr<CefClientHandler> clientHandler_;

    IMPLEMENT_REFCOUNTING(ClientDownloadImageCallback);
    DISALLOW_COPY_AND_ASSIGN(ClientDownloadImageCallback);
};

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
    // return true to cancel the creation

    //参数释义：
    // browser : 发出popup请求的浏览器对象
    // frame : 发出popup请求的那个frame
    // target_url : 要加载的目标url
    // target_frame_name : 要加载的frame_name
    // target_disposition : 显示方式
    //

    // notify to outside
    switch(target_disposition){
    case WOD_UNKNOWN:
    {
      qInfo()<<__FUNCTION__<<"WOD_UNKNOWN";
    }break;
    case WOD_CURRENT_TAB:
      qInfo()<<__FUNCTION__<<"WOD_CURRENT_TAB";
      break;
    case WOD_SINGLETON_TAB:
      qInfo()<<__FUNCTION__<<"WOD_SINGLETON_TAB";
      break;
    case WOD_NEW_FOREGROUND_TAB:
    {
      qInfo()<<__FUNCTION__<<"WOD_NEW_FOREGROUND_TAB";
      NotifyForgroundTab(windowInfo,
                           client,
                           settings);
      // Allow creation, but should be used as a child window of a window
      return false;
    }break;
    case WOD_NEW_BACKGROUND_TAB:
      qInfo()<<__FUNCTION__<<"WOD_NEW_BACKGROUND_TAB";
      break;
    case WOD_NEW_POPUP:{
      qInfo()<<__FUNCTION__<<"WOD_NEW_POPUP";
      NotifyPopupWindow(popupFeatures,windowInfo,client,settings);
      return false;
    }break;
    case WOD_NEW_WINDOW:
      qInfo()<<__FUNCTION__<<"WOD_NEW_WINDOW";
      break;
    case WOD_SAVE_TO_DISK:
      qInfo()<<__FUNCTION__<<"WOD_SAVE_TO_DISK";
      break;
    case WOD_OFF_THE_RECORD:
      qInfo()<<__FUNCTION__<<"WOD_OFF_THE_RECORD";
      break;
    case WOD_IGNORE_ACTION:
      qInfo()<<__FUNCTION__<<"WOD_IGNORE_ACTION";
      break;
    }

    // return true to cancel create
    return true;
}

void CefClientHandler::OnAfterCreated(CefRefPtr<CefBrowser> browser)
{
    CEF_REQUIRE_UI_THREAD();
    gBrowserCount++;
    browser_count_ += 1;

    qInfo()<<__FUNCTION__<<"Total browser count:"<<gBrowserCount
          <<"handler browser count:"<<browser_count_;


    NotifyBrowserCreated(browser);
}

bool CefClientHandler::DoClose(CefRefPtr<CefBrowser> browser)
{
    CEF_REQUIRE_UI_THREAD();
    qInfo()<<__FUNCTION__<<"browser closing ";

    NotifyBrowserClosing(browser);

    // Return false to allow the close. For windowed browsers this will result in the OS close
    // event being sent.
    // and return true to pevent sending close signal to the toplevel window
    return true;
}

void CefClientHandler::OnBeforeClose(CefRefPtr<CefBrowser> browser)
{
    CEF_REQUIRE_UI_THREAD();
    gBrowserCount --;
    browser_count_--;
    qInfo()<<__FUNCTION__<<"Total browser count:"<<gBrowserCount;
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

void CefClientHandler::NotifyBrowserTitleChange(CefRefPtr<CefBrowser> browser,
                                               const CefString &title)
{
    if (!CURRENTLY_ON_MAIN_THREAD()) {
        // Execute this method on the main thread.
        MAIN_POST_CLOSURE(
                    base::Bind(&CefClientHandler::NotifyBrowserTitleChange, this, browser, title));
        return;
    }

    if (delegate_)
        delegate_->onBrowserTitleChange(title.ToString());
}

void CefClientHandler::NotifyBrowserFavicon(CefRefPtr<CefImage> image,
                                           const CefString &icon_url)
{
    if (!CURRENTLY_ON_MAIN_THREAD()) {
        // Execute this method on the main thread.
        MAIN_POST_CLOSURE(
                    base::Bind(&CefClientHandler::NotifyBrowserFavicon,
                               this,
                               image,
                               icon_url));
        return;
    }

    if (delegate_)
        delegate_->onBrowserFaviconChange(image, icon_url);
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

    NotifyBrowserTitleChange(browser, title);
}

void CefClientHandler::OnFaviconURLChange(CefRefPtr<CefBrowser> browser,
                                          const std::vector<CefString> &icon_urls)
{
    CEF_REQUIRE_UI_THREAD();
    if ( !icon_urls.empty() )
    {
        for( size_t i = 0; i<icon_urls.size(); i++ )
        {
            std::string fileExtension = UtilQt::GetFileExtension(icon_urls[i]);
            browser->GetHost()->DownloadImage(icon_urls[i], true, 48, true, new ClientDownloadImageCallback(this));
            //            if(fileExtension.compare("ico")==0||fileExtension.compare("ICO")==0)
            //            {
            //                 break;
            //            }
        }
    }
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

void CefClientHandler::OnStatusMessage(CefRefPtr<CefBrowser> browser,
                                       const CefString &value)
{

}

bool CefClientHandler::OnConsoleMessage(CefRefPtr<CefBrowser> browser,
                                        cef_log_severity_t level,
                                        const CefString &message,
                                        const CefString &source,
                                        int line)
{
    return false;
}

bool CefClientHandler::OnAutoResize(CefRefPtr<CefBrowser> browser,
                                    const CefSize &new_size)
{
    return false;
}

void CefClientHandler::OnLoadingProgressChange(CefRefPtr<CefBrowser> browser,
                                               double progress)
{

}

bool CefClientHandler::OnJSDialog(CefRefPtr<CefBrowser> browser,
                                  const CefString &origin_url,
                                  CefJSDialogHandler::JSDialogType dialog_type,
                                  const CefString &message_text,
                                  const CefString &default_prompt_text,
                                  CefRefPtr<CefJSDialogCallback> callback,
                                  bool &suppress_message)
{
    //普通提示框 询问框
    if(dialog_type == JSDIALOGTYPE_ALERT){
//        AlertDialog dlg(QString::fromStdWString(origin_url),
//                        QString::fromStdWString(message_text));
//        dlg.exec();
        auto hwnd = browser->GetHost()->GetWindowHandle();
        ::MessageBox(hwnd, message_text.ToWString().c_str(), L"AAA", MB_OK);
        suppress_message = true;
        return false;
    }else if(dialog_type == JSDIALOGTYPE_CONFIRM){

    }else if(dialog_type == JSDIALOGTYPE_PROMPT){

    }
    return false;
}

void CefClientHandler::OnBeforeContextMenu(CefRefPtr<CefBrowser> browser,
                                           CefRefPtr<CefFrame> frame,
                                           CefRefPtr<CefContextMenuParams> params,
                                           CefRefPtr<CefMenuModel> model)
{

}

bool CefClientHandler::OnContextMenuCommand(CefRefPtr<CefBrowser> browser,
                                            CefRefPtr<CefFrame> frame,
                                            CefRefPtr<CefContextMenuParams> params,
                                            int command_id,
                                            CefContextMenuHandler::EventFlags event_flags)
{
    return false;
}

void CefClientHandler::OnLoadingStateChange(CefRefPtr<CefBrowser> browser,
                                            bool isLoading,
                                            bool canGoBack,
                                            bool canGoForward)
{
    if (delegate_)
        delegate_->onBrowserLoadingStateChange(isLoading,
                                               canGoBack,
                                               canGoForward);
}

void CefClientHandler::OnLoadStart(CefRefPtr<CefBrowser> browser,
                                   CefRefPtr<CefFrame> frame,
                                   CefLoadHandler::TransitionType transition_type)
{

}

void CefClientHandler::OnLoadEnd(CefRefPtr<CefBrowser> browser,
                                 CefRefPtr<CefFrame> frame,
                                 int httpStatusCode)
{

}

void CefClientHandler::OnLoadError(CefRefPtr<CefBrowser> browser,
                                   CefRefPtr<CefFrame> frame,
                                   CefLoadHandler::ErrorCode errorCode,
                                   const CefString &errorText,
                                   const CefString &failedUrl)
{

}

void CefClientHandler::OnTakeFocus(CefRefPtr<CefBrowser> browser, bool next)
{

}

bool CefClientHandler::OnSetFocus(CefRefPtr<CefBrowser> browser,
                                  CefFocusHandler::FocusSource source)
{
    return false;
}

void CefClientHandler::OnGotFocus(CefRefPtr<CefBrowser> browser)
{
    if(delegate_){
        delegate_->onBrowserGotFocus(browser);
    }
}

void CefClientHandler::NotifyForgroundTab(CefWindowInfo &windowInfo,
                                          CefRefPtr<CefClient> &client,
                                          CefBrowserSettings &settings)
{
//    if (!CURRENTLY_ON_MAIN_THREAD()) {
//        // Execute this method on the main thread.
//        MAIN_POST_CLOSURE(
//                    base::Bind(&CefClientHandler::NotifyForgroundTab, this,
//                               windowInfo,
//                               client,
//                               settings));
//        return;
//    }

    if (delegate_)
        delegate_->onBrowserForgroundTab(windowInfo,
                                         client,
                                         settings);
}

void CefClientHandler::NotifyPopupWindow(const CefPopupFeatures &popupFeatures,
                                         CefWindowInfo &windowInfo,
                                         CefRefPtr<CefClient> &client,
                                         CefBrowserSettings &settings)
{
//    if (!CURRENTLY_ON_MAIN_THREAD()) {
//        // Execute this method on the main thread.
//        MAIN_POST_CLOSURE(
//                    base::Bind(&CefClientHandler::NotifyPopupWindow,
//                               this,
//                               popupFeatures,
//                               windowInfo,
//                               client,
//                               settings));
//        return;
//    }

    if (delegate_)
        delegate_->onBrowserPopupWnd(popupFeatures,
                                     windowInfo,
                                     client,
                                     settings);
}
