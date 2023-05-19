#include "cef_client_handler.h"
#include "message_loop/main_message_loop.h"

#include <include/base/cef_callback.h>
#include <include/cef_browser.h>
#include <include/cef_frame.h>
#include <include/cef_parser.h>
#include <include/cef_ssl_status.h>
#include <include/cef_x509_certificate.h>
#include <include/wrapper/cef_closure_task.h>
#include <include/wrapper/cef_helpers.h>

#include <QtDebug>
#include <QApplication>
#include "dialogs/alertdialog.h"
#include "utils/util_qt.h"
#include "globaldef.h"

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
    CEF_REQUIRE_UI_THREAD()
    qInfo()<<__FUNCTION__<<"browser closing ";

    NotifyBrowserClosing(browser);

    // Return false to allow the close. For windowed browsers this will result in the OS close
    // event being sent.
    // and return true to pevent sending close signal to the toplevel window
#ifdef Q_OS_LINUX
    return false;
#else
    return true;
#endif
}

void CefClientHandler::OnBeforeClose(CefRefPtr<CefBrowser> browser)
{
    CEF_REQUIRE_UI_THREAD()
    gBrowserCount --;
    browser_count_--;
    qInfo()<<__FUNCTION__<<browser.get()<<"closed."<<"browser remain count:"<<gBrowserCount;

    NotifyBrowserBeforeClose(browser);
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

void CefClientHandler::NotifyBrowserBeforeClose(CefRefPtr<CefBrowser> browser)
{
    if (!CURRENTLY_ON_MAIN_THREAD()) {
        // Execute this method on the main thread.
        MAIN_POST_CLOSURE(
                    base::Bind(&CefClientHandler::NotifyBrowserBeforeClose, this, browser));
        return;
    }

    if (delegate_)
        delegate_->onBrowserBeforClose();
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

void CefClientHandler::NotifyFullscreenModeChange(CefRefPtr<CefBrowser> browser,
                                                  bool fullscreen)
{
    if (!CURRENTLY_ON_MAIN_THREAD()) {
        // Execute this method on the main thread.
        MAIN_POST_CLOSURE(
                    base::Bind(&CefClientHandler::NotifyFullscreenModeChange,
                               this,
                               browser,
                               fullscreen));
        return;
    }

    if (delegate_)
        delegate_->onBrowserFullscreenChange(fullscreen);
}

void CefClientHandler::NotifyStatusMessage(const CefString &msg)
{
    if (!CURRENTLY_ON_MAIN_THREAD()) {
        // Execute this method on the main thread.
        MAIN_POST_CLOSURE(
                    base::Bind(&CefClientHandler::NotifyStatusMessage,
                               this,
                               msg));
        return;
    }

    if (delegate_)
        delegate_->onBrowserStatusMessage(msg.ToString());
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
            browser->GetHost()->DownloadImage(icon_urls[i], true, 0, true, new ClientDownloadImageCallback(this));
        }
    }
}

void CefClientHandler::OnFullscreenModeChange(CefRefPtr<CefBrowser> browser,
                                              bool fullscreen)
{
    CEF_REQUIRE_UI_THREAD();

    NotifyFullscreenModeChange(browser, fullscreen);
}

bool CefClientHandler::OnTooltip(CefRefPtr<CefBrowser> browser,
                                 CefString &text)
{

    return false;
}

void CefClientHandler::OnStatusMessage(CefRefPtr<CefBrowser> browser,
                                       const CefString &value)
{
    CEF_REQUIRE_UI_THREAD();

    NotifyStatusMessage(value);
}

bool CefClientHandler::OnConsoleMessage(CefRefPtr<CefBrowser> browser,
                                        cef_log_severity_t level,
                                        const CefString &message,
                                        const CefString &source,
                                        int line)
{
//    qInfo()<<__FUNCTION__<<QString::fromStdString(message);
    return false;
}

bool CefClientHandler::OnAutoResize(CefRefPtr<CefBrowser> browser,
                                    const CefSize &new_size)
{
    qInfo()<<__FUNCTION__<<QSize(new_size.width, new_size.height);
    return false;
}

void CefClientHandler::OnLoadingProgressChange(CefRefPtr<CefBrowser> browser,
                                               double progress)
{
    if(delegate_){
        delegate_->onBrowserLoadingProgressChange(browser, progress);
    }
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
#if 1
    model->Clear();
    if(delegate_){
        delegate_->onBrowserBeforeContextMenu(browser, frame, params, model);
    }
    return;
#endif

    Q_UNUSED(browser);
    Q_UNUSED(frame);
    if ((params->GetTypeFlags() & (CM_TYPEFLAG_PAGE | CM_TYPEFLAG_FRAME)) != 0)
    {
        // 在 Page 上
        if ( params->GetTypeFlags() == CM_TYPEFLAG_PAGE )
        {
            model->InsertItemAt(2, MENU_ID_RELOAD, CefString(QObject::tr("Refresh").toStdString()));
            model->InsertItemAt(3, MENU_ID_RELOAD_NOCACHE,
                                CefString(QObject::tr("Reload ignore cache").toStdString()));
            model->InsertItemAt(5, MENU_ID_SAVE_AS,
                                CefString(QObject::tr("Save as").toStdString()));
            model->InsertSeparatorAt(7);
            model->AddItem(MENU_ID_USER_INSPECT_ELEMENT,
                            CefString(QObject::tr("Inspect element").toStdString()));
        }

        // 在有子frame参与的page上
        if ( params->GetTypeFlags() == (CM_TYPEFLAG_PAGE | CM_TYPEFLAG_FRAME) )
        {
            model->InsertItemAt(2, MENU_ID_RELOAD, CefString(QObject::tr("Refresh").toStdString()));
            model->InsertItemAt(3, MENU_ID_RELOAD_NOCACHE,
                                CefString(QObject::tr("Reload ignore cache").toStdString()));
            model->InsertItemAt(5, MENU_ID_SAVE_AS,
                                CefString(QObject::tr("Save as").toStdString()));
            model->InsertSeparatorAt(7);
            model->AddItem(MENU_ID_USER_FRAME_VIEW_SOURCE,
                            CefString(QObject::tr("View frame source").toStdString()));
            model->AddItem(MENU_ID_USER_FRAME_REFRESH,
                            CefString(QObject::tr("Refresh frame").toStdString()));
            model->AddItem(MENU_ID_USER_INSPECT_ELEMENT,
                            CefString(QObject::tr("Inspect element").toStdString()));
        }

        // 在某个链接地址上
        if( params->GetTypeFlags() == (CM_TYPEFLAG_PAGE | CM_TYPEFLAG_LINK) )
        {
            if(model->Clear()){
                model->AddItem(MENU_ID_USER_OPEN_LINK_PAGE, CefString(QObject::tr("Open the link in a new tab").toStdString()));
                model->AddItem(MENU_ID_USER_OPEN_LINK_WINDOW, CefString(QObject::tr("Open the link in a new window").toStdString()));
                model->AddItem(MENU_ID_USER_OPEN_LINK_INPRIVATE,
                               CefString(QObject::tr("Open the link in private window").toStdString()));
                model->AddSeparator();
                model->AddItem(MENU_ID_USER_COPY_LINK_URL, CefString(QObject::tr("Copy link address").toStdString()));
                model->AddSeparator();
                model->AddItem(MENU_ID_USER_INSPECT_ELEMENT,
                                CefString(QObject::tr("Inspect element").toStdString()));
            }
        }
        // 输入框
        if( params->GetTypeFlags() == (CM_TYPEFLAG_PAGE | CM_TYPEFLAG_EDITABLE) )
        {
            model->InsertItemAt(0, MENU_ID_USER_EMOJI, CefString(QObject::tr("emoji").toStdString()));
        }

        // 选中非链接地址的内容
        if( params->GetTypeFlags() == (CM_TYPEFLAG_PAGE | CM_TYPEFLAG_SELECTION) )
        {
            model->AddItem(MENU_ID_USER_SEARCH_SELECTED, CefString(QObject::tr("Searh in web").toStdString()));
            model->AddSeparator();
            model->AddItem(MENU_ID_USER_INSPECT_ELEMENT,
                            CefString(QObject::tr("Inspect element").toStdString()));
        }

        // 图片上
        if( params->GetTypeFlags() == (CM_TYPEFLAG_PAGE | CM_TYPEFLAG_MEDIA) )
        {
            if(model->Clear()){
                model->AddItem(MENU_ID_USER_OPEN_IMAGE_PAGE,
                               CefString(QObject::tr("Open image in new page").toStdString()));
                model->AddItem(MENU_ID_USER_SAVE_IMAGE_AS,
                               CefString(QObject::tr("Save image as").toStdString()));
                model->AddItem(MENU_ID_USER_COPY_IMAGE,
                               CefString(QObject::tr("Copy image").toStdString()));
                model->AddItem(MENU_ID_USER_COPY_IMAGE_LIINK,
                               CefString(QObject::tr("Copy image url").toStdString()));
                model->AddSeparator();
                model->AddItem(MENU_ID_USER_INSPECT_ELEMENT,
                                CefString(QObject::tr("Inspect element").toStdString()));
            }
        }
        // 既是图片又是链接又在Page上
        if( params->GetTypeFlags() == (CM_TYPEFLAG_PAGE | CM_TYPEFLAG_MEDIA | CM_TYPEFLAG_LINK) )
        {
            if(model->Clear()){
                model->AddItem(MENU_ID_USER_OPEN_LINK_PAGE, CefString(QObject::tr("Open the link in a new tab").toStdString()));
                model->AddItem(MENU_ID_USER_OPEN_LINK_WINDOW, CefString(QObject::tr("Open the link in a new window").toStdString()));
                model->AddItem(MENU_ID_USER_OPEN_LINK_INPRIVATE,
                               CefString(QObject::tr("Open the link in private window").toStdString()));
                model->AddSeparator();
                model->AddItem(MENU_ID_USER_COPY_LINK_URL, CefString(QObject::tr("Copy link address").toStdString()));
                model->AddSeparator();
                model->AddItem(MENU_ID_USER_OPEN_IMAGE_PAGE,
                               CefString(QObject::tr("Open image in new page").toStdString()));
                model->AddItem(MENU_ID_USER_SAVE_IMAGE_AS,
                               CefString(QObject::tr("Save image as").toStdString()));
                model->AddItem(MENU_ID_USER_COPY_IMAGE,
                               CefString(QObject::tr("Copy image").toStdString()));
                model->AddItem(MENU_ID_USER_COPY_IMAGE_LIINK,
                               CefString(QObject::tr("Copy image url").toStdString()));
                model->AddSeparator();
                model->AddItem(MENU_ID_USER_INSPECT_ELEMENT,
                                CefString(QObject::tr("Inspect element").toStdString()));
            }
        }
        // 在Page上选中内容，选中的内容是链接地址
        if( params->GetTypeFlags() == (CM_TYPEFLAG_PAGE | CM_TYPEFLAG_SELECTION | CM_TYPEFLAG_LINK) )
        {
            model->InsertItemAt(0, MENU_ID_USER_OPEN_LINK_PAGE, CefString(QObject::tr("Open the link in a new tab").toStdString()));
            model->InsertItemAt(1, MENU_ID_USER_OPEN_LINK_WINDOW, CefString(QObject::tr("Open the link in a new window").toStdString()));
            model->InsertItemAt(2, MENU_ID_USER_OPEN_LINK_INPRIVATE,
                           CefString(QObject::tr("Open the link in private window").toStdString()));
            model->InsertSeparatorAt(3);
            model->InsertItemAt(4, MENU_ID_USER_COPY_LINK_URL, CefString(QObject::tr("Copy link address").toStdString()));
            model->InsertSeparatorAt(5);
            // copy
            model->AddItem(MENU_ID_USER_SEARCH_SELECTED, CefString(QObject::tr("Searh in web").toStdString()));
            model->AddSeparator();
            model->AddItem(MENU_ID_USER_INSPECT_ELEMENT,
                            CefString(QObject::tr("Inspect element").toStdString()));
        }
        if( params->GetTypeFlags() == (CM_TYPEFLAG_PAGE | CM_TYPEFLAG_SELECTION | CM_TYPEFLAG_LINK | CM_TYPEFLAG_FRAME) )
        {

        }
        if( params->GetTypeFlags() == (CM_TYPEFLAG_PAGE | CM_TYPEFLAG_MEDIA | CM_TYPEFLAG_LINK | CM_TYPEFLAG_FRAME) )
        {

        }
        if( params->GetTypeFlags() == (CM_TYPEFLAG_PAGE | CM_TYPEFLAG_EDITABLE| CM_TYPEFLAG_SELECTION ) )
        {

        }

        qInfo()<<__FUNCTION__<<":";
        for(int i = 0; i< model->GetCount(); i++)
        {
            if(model->GetCommandIdAt(i) == -1){
                qInfo()<<"----------------";
            }else{
                qInfo()<<QString::fromStdString(model->GetLabelAt(i));
            }
        }

        // virtual bool SetAccelerator(int command_id, int key_code, bool shift_pressed, bool ctrl_pressed, bool alt_pressed) = 0;
        model->SetAccelerator(MENU_ID_BACK, 0x25, false, false, true); // 设置加速键，atl+左箭头
        model->SetAccelerator(MENU_ID_FORWARD, 0x27, false, false, true); // 设置加速键，atl+右箭头
        model->SetAccelerator(MENU_ID_RELOAD, 0x52, false, true, false); // 设置加速键，Ctrl + R,
        model->SetAccelerator(MENU_ID_PRINT, 0x50, false, true, false); // 设置加速键，Ctrl + P
        model->SetAccelerator(MENU_ID_VIEW_SOURCE, 0x55, false, true, false); // 设置加速键，Ctrl + U
        model->SetAccelerator(MENU_ID_USER_INSPECT_ELEMENT, 0x49, true, true, false); // Ctrl + Shift + I
    }
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
    CEF_REQUIRE_UI_THREAD();
    Q_UNUSED(browser);
    if (delegate_)
        delegate_->onBrowserLoadingStateChange(isLoading,
                                               canGoBack,
                                               canGoForward);
}

void CefClientHandler::OnLoadStart(CefRefPtr<CefBrowser> browser,
                                   CefRefPtr<CefFrame> frame,
                                   CefLoadHandler::TransitionType transition_type)
{
    CEF_REQUIRE_UI_THREAD();
    Q_UNUSED(browser);
    Q_UNUSED(frame);

    if(delegate_){
        delegate_->onBrowerLoadStart(transition_type);
    }
}

void CefClientHandler::OnLoadEnd(CefRefPtr<CefBrowser> browser,
                                 CefRefPtr<CefFrame> frame,
                                 int httpStatusCode)
{
    CEF_REQUIRE_UI_THREAD();
    Q_UNUSED(browser);
    Q_UNUSED(frame);

    if(delegate_){
        delegate_->onBrowerLoadEnd(httpStatusCode);
    }

    auto cefRect2QRect = [](const CefRect &rect) -> QRect
    {
        QRect ret;
        ret.setX(rect.x);
        ret.setY(rect.y);
        ret.setWidth(rect.width);
        ret.setHeight(rect.height);
        return ret;
    };
}

void CefClientHandler::OnLoadError(CefRefPtr<CefBrowser> browser,
                                   CefRefPtr<CefFrame> frame,
                                   CefLoadHandler::ErrorCode errorCode,
                                   const CefString &errorText,
                                   const CefString &failedUrl)
{
    CEF_REQUIRE_UI_THREAD();
    Q_UNUSED(browser);
    Q_UNUSED(frame);
    qInfo()<<__FUNCTION__<<"errorCode:"<<errorCode<<"errorText:"<<QString::fromStdString(errorText)
          <<"failedUrl:"<<QString::fromStdString(failedUrl);
}

void CefClientHandler::OnTakeFocus(CefRefPtr<CefBrowser> browser, bool next)
{
    CEF_REQUIRE_UI_THREAD();
    Q_UNUSED(browser);
    qInfo()<<__FUNCTION__<<next;
}

bool CefClientHandler::OnSetFocus(CefRefPtr<CefBrowser> browser,
                                  CefFocusHandler::FocusSource source)
{
    CEF_REQUIRE_UI_THREAD();
    Q_UNUSED(browser);

    return false;
}

void CefClientHandler::OnGotFocus(CefRefPtr<CefBrowser> browser)
{
    if(delegate_){
        delegate_->onBrowserGotFocus(browser);
    }
}

bool CefClientHandler::OnPreKeyEvent(CefRefPtr<CefBrowser> browser,
                                     const CefKeyEvent &event,
                                     CefEventHandle os_event,
                                     bool *is_keyboard_shortcut)
{
    CEF_REQUIRE_UI_THREAD();

    if(delegate_){
        return delegate_->onBrowserPreKeyEvent(event, os_event, is_keyboard_shortcut);
    }
    return false;

    Q_UNUSED(browser);
}

bool CefClientHandler::OnKeyEvent(CefRefPtr<CefBrowser> browser,
                                  const CefKeyEvent &event,
                                  CefEventHandle os_event)
{
    CEF_REQUIRE_UI_THREAD();

    if(delegate_){
        return delegate_->onBrowserKeyEvent(event, os_event);
    }
    return false;
    Q_UNUSED(browser);
}

void CefClientHandler::ShowDevTools(CefRefPtr<CefBrowser> browser,
                                    const CefPoint &inspect_element_at)
{
    if (!CefCurrentlyOn(TID_UI)) {
      // Execute this method on the UI thread.
      CefPostTask(TID_UI, base::Bind(&CefClientHandler::ShowDevTools,
                                     this, browser,
                                     inspect_element_at));
      return;
    }

    CefWindowInfo windowInfo;
    CefRefPtr<CefClient> client;
    CefBrowserSettings settings;

    //    MainContext::Get()->PopulateBrowserSettings(&settings);

    CefRefPtr<CefBrowserHost> host = browser->GetHost();
    // Test if the DevTools browser already exists.
    bool has_devtools = host->HasDevTools();
    if (!has_devtools) {
        // Create a new RootWindow for the DevTools browser that will be created
        // by ShowDevTools().
        NotifyCreateDevTool(windowInfo, client, settings);

        has_devtools = true;
    }

    if (has_devtools) {
      // Create the DevTools browser if it doesn't already exist.
      // Otherwise, focus the existing DevTools browser and inspect the element
      // at |inspect_element_at| if non-empty.
      host->ShowDevTools(windowInfo, client, settings, inspect_element_at);
    }
}

void CefClientHandler::CloseDevTools(CefRefPtr<CefBrowser> browser)
{
    browser->GetHost()->CloseDevTools();
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

void CefClientHandler::NotifyCreateDevTool(CefWindowInfo &windowInfo,
                                           CefRefPtr<CefClient> &client,
                                           CefBrowserSettings &settings)
{
    CEF_REQUIRE_UI_THREAD();
    delegate_->onBrowserDeveTools(windowInfo,
                                  client,
                                  settings);
}
