#ifndef CEF_CLIENT_HANDLER_H
#define CEF_CLIENT_HANDLER_H
#pragma once

#include <include/cef_client.h>

#include <list>
#include <mutex>

class CefClientHandler
        : public CefClient
        , public CefLifeSpanHandler
        , public CefDisplayHandler
        , public CefJSDialogHandler
        , public CefContextMenuHandler
        , public CefLoadHandler
        , public CefFocusHandler
        , public CefKeyboardHandler
{
public:
    class Delegate{
    public:
        virtual void onBrowserForgroundTab(CefWindowInfo &windowInfo,
                                           CefRefPtr<CefClient> &client,
                                           CefBrowserSettings &settings) = 0;
        virtual void onBrowserPopupWnd(const CefPopupFeatures& popupFeatures,
                                       CefWindowInfo& windowInfo,
                                       CefRefPtr<CefClient>& client,
                                       CefBrowserSettings& settings) = 0;
        virtual void onBrowserDeveTools(CefWindowInfo& windowInfo,
                                        CefRefPtr<CefClient>& client,
                                        CefBrowserSettings& settings) = 0;
        // Called when the browser is created.
        virtual void OnBrowserCreated(CefRefPtr<CefBrowser> browser) = 0;

        // Called when the browser is closing.
        virtual void OnBrowserClosing(CefRefPtr<CefBrowser> browser) = 0;
        virtual void onBrowserAddressChange(const std::string &url) = 0;
        virtual void onBrowserBeforClose(){}

        virtual void onBrowserTitleChange(const std::string &url) = 0;
        virtual void onBrowserFullscreenChange(bool fullscreen) = 0;
        virtual void onBrowserStatusMessage(const std::string &msg) = 0;
        virtual void onBrowserFaviconChange(CefRefPtr<CefImage> image,
                                            const std::string &url) = 0;
        virtual void onBrowserLoadingProgressChange(CefRefPtr<CefBrowser> browser,
                                                    double progress) = 0;

        virtual void onBrowserLoadingStateChange(bool isLoading,
                                                 bool canGoBack,
                                                 bool canGoForward) = 0;
        virtual void onBrowerLoadStart(CefLoadHandler::TransitionType transition_type) = 0;
        virtual void onBrowerLoadEnd(int httpStatusCode) = 0;

        virtual void onBrowserGotFocus(CefRefPtr<CefBrowser> ) {}
        virtual bool onBrowserPreKeyEvent(const CefKeyEvent &event,
                                          CefEventHandle os_event,
                                          bool *is_keyboard_shortcut) = 0;
        virtual bool onBrowserKeyEvent(const CefKeyEvent &event,
                                       CefEventHandle os_event) = 0;
        virtual void onBrowserBeforeContextMenu(CefRefPtr<CefBrowser> browser,
                                                CefRefPtr<CefFrame> frame,
                                                CefRefPtr<CefContextMenuParams> params,
                                                CefRefPtr<CefMenuModel> model) = 0;
    protected:
        virtual ~Delegate() {}
    };

    CefClientHandler(Delegate* delegate, const std::string& startup_url);
    ~CefClientHandler();

    // CefClient interface
    CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() override { return this; }
    CefRefPtr<CefDisplayHandler> GetDisplayHandler() override { return this; }
    CefRefPtr<CefJSDialogHandler> GetJSDialogHandler() override {return this;}
    CefRefPtr<CefContextMenuHandler> GetContextMenuHandler() override {return this;}
    CefRefPtr<CefLoadHandler> GetLoadHandler() override {return this;}
    //    CefRefPtr<CefFocusHandler> GetFocusHandler() override{ return this; }
    CefRefPtr<CefKeyboardHandler> GetKeyboardHandler() override { return this; }

    static int gBrowserCount;

    // CefLifeSpanHandler methods
public:
    bool OnBeforePopup(
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
            bool* no_javascript_access) override;
    void OnAfterCreated(CefRefPtr<CefBrowser> browser) override;
    bool DoClose(CefRefPtr<CefBrowser> browser) override;
    void OnBeforeClose(CefRefPtr<CefBrowser> browser) override;

    // CefDisplayHandler interface
public:
    void OnAddressChange(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, const CefString &url) override;
    void OnTitleChange(CefRefPtr<CefBrowser> browser, const CefString &title) override;
    void OnFaviconURLChange(CefRefPtr<CefBrowser> browser, const std::vector<CefString> &icon_urls) override;
    void OnFullscreenModeChange(CefRefPtr<CefBrowser> browser, bool fullscreen) override;
    bool OnTooltip(CefRefPtr<CefBrowser> browser, CefString &text) override;
    void OnStatusMessage(CefRefPtr<CefBrowser> browser, const CefString &value) override;
    bool OnConsoleMessage(CefRefPtr<CefBrowser> browser, cef_log_severity_t level, const CefString &message, const CefString &source, int line) override;
    bool OnAutoResize(CefRefPtr<CefBrowser> browser, const CefSize &new_size) override;
    void OnLoadingProgressChange(CefRefPtr<CefBrowser> browser, double progress) override;

    // CefJSDialogHandler interface
public:
    bool OnJSDialog(CefRefPtr<CefBrowser> browser,
                    const CefString &origin_url,
                    JSDialogType dialog_type,
                    const CefString &message_text,
                    const CefString &default_prompt_text,
                    CefRefPtr<CefJSDialogCallback> callback,
                    bool &suppress_message) override;
    // CefContextMenuHandler interface
public:
    void OnBeforeContextMenu(CefRefPtr<CefBrowser> browser,
                             CefRefPtr<CefFrame> frame,
                             CefRefPtr<CefContextMenuParams> params,
                             CefRefPtr<CefMenuModel> model) override;
    bool OnContextMenuCommand(CefRefPtr<CefBrowser> browser,
                              CefRefPtr<CefFrame> frame,
                              CefRefPtr<CefContextMenuParams> params,
                              int command_id, EventFlags event_flags) override;
    // CefLoadHandler interface
public:
    void OnLoadingStateChange(CefRefPtr<CefBrowser> browser,
                              bool isLoading,
                              bool canGoBack,
                              bool canGoForward) override;
    void OnLoadStart(CefRefPtr<CefBrowser> browser,
                     CefRefPtr<CefFrame> frame,
                     TransitionType transition_type) override;
    void OnLoadEnd(CefRefPtr<CefBrowser> browser,
                   CefRefPtr<CefFrame> frame,
                   int httpStatusCode) override;
    void OnLoadError(CefRefPtr<CefBrowser> browser,
                     CefRefPtr<CefFrame> frame,
                     ErrorCode errorCode,
                     const CefString &errorText,
                     const CefString &failedUrl) override;

    // CefFocusHandler interface
public:
    void OnTakeFocus(CefRefPtr<CefBrowser> browser, bool next) override;
    bool OnSetFocus(CefRefPtr<CefBrowser> browser, FocusSource source) override;
    void OnGotFocus(CefRefPtr<CefBrowser> browser) override;

    // CefKeyboardHandler interface
public:
    bool OnPreKeyEvent(CefRefPtr<CefBrowser> browser,
                       const CefKeyEvent &event,
                       CefEventHandle os_event,
                       bool *is_keyboard_shortcut) override;
    bool OnKeyEvent(CefRefPtr<CefBrowser> browser,
                    const CefKeyEvent &event,
                    CefEventHandle os_event) override;


    // Returns the number of browsers currently using this handler. Can only be
    // called on the CEF UI thread.
    int GetBrowserCount() const;

    // Show a new DevTools popup window.
    void ShowDevTools(CefRefPtr<CefBrowser> browser,
                      const CefPoint& inspect_element_at);

    // Close the existing DevTools popup window, if any.
    void CloseDevTools(CefRefPtr<CefBrowser> browser);

    // Test if the current site has SSL information available.
    bool HasSSLInformation(CefRefPtr<CefBrowser> browser);

    // Show SSL information for the current site.
    void ShowSSLInformation(CefRefPtr<CefBrowser> browser);

    // Set a string resource for loading via StringResourceProvider.
    void SetStringResource(const std::string& page, const std::string& data);

    // Returns the Delegate.
    Delegate* delegate() const { return delegate_; }

    // Returns the startup URL.
    std::string startup_url() const { return startup_url_; }

private:
    // favicon download help class
    friend class ClientDownloadImageCallback;

    // Execute Delegate notifications on the main thread.
    void NotifyForgroundTab(CefWindowInfo& windowInfo,
                            CefRefPtr<CefClient>& client,
                            CefBrowserSettings& settings);
    void NotifyPopupWindow(const CefPopupFeatures &popupFeatures,
                           CefWindowInfo &windowInfo,
                           CefRefPtr<CefClient> &client,
                           CefBrowserSettings &settings);
    void NotifyCreateDevTool(CefWindowInfo& windowInfo,
                             CefRefPtr<CefClient>& client,
                             CefBrowserSettings& settings);
    void NotifyBrowserCreated(CefRefPtr<CefBrowser> browser);
    void NotifyBrowserBeforeClose(CefRefPtr<CefBrowser> browser);
    void NotifyBrowserClosing(CefRefPtr<CefBrowser> browser);
    void NotifyBrowserClosed(CefRefPtr<CefBrowser> browser);

    void NotifyBrowserAddressChange(CefRefPtr<CefBrowser> browser,
                                    CefRefPtr<CefFrame> frame,
                                    const CefString &url);
    void NotifyBrowserTitleChange(CefRefPtr<CefBrowser> browser,
                                  const CefString &title);
    void NotifyFullscreenModeChange(CefRefPtr<CefBrowser> browser, bool fullscreen);
    void NotifyStatusMessage(const CefString &msg);
    void NotifyBrowserFavicon(CefRefPtr<CefImage> image,
                              const CefString &icon_url);

    // The startup URL.
    const std::string startup_url_;

    // MAIN THREAD MEMBERS
    // The following members will only be accessed on the main thread. This will
    // be the same as the CEF UI thread except when using multi-threaded message
    // loop mode on Windows.

    Delegate* delegate_;
    // The current number of browsers using this handler.
    int browser_count_ = 0;

    // Include the default reference counting implementation.
    IMPLEMENT_REFCOUNTING(CefClientHandler);
    DISALLOW_COPY_AND_ASSIGN(CefClientHandler);
};
#endif // CEFCLIENTHANDLER_H
