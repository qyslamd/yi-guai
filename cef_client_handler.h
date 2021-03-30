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
{
public:
    class Delegate{
    public:
        virtual void onBrowserNewForgroundPage(CefWindowInfo &windowInfo,
                                               CefRefPtr<CefClient> &client,
                                               CefBrowserSettings &settings) = 0;
        // Called when the browser is created.
        virtual void OnBrowserCreated(CefRefPtr<CefBrowser> browser) = 0;

        // Called when the browser is closing.
        virtual void OnBrowserClosing(CefRefPtr<CefBrowser> browser) = 0;
        virtual void onBrowserAddressChange(const std::string &url) = 0;
        virtual void onBrowserTitleChange(const std::string &url) = 0;
    protected:
        virtual ~Delegate() {}
    };

    CefClientHandler(Delegate* delegate,
                      const std::string& startup_url);
    ~CefClientHandler();
    // This object may outlive the Delegate object so it's necessary for the
    // Delegate to detach itself before destruction.
    void DetachDelegate();

    // CefClient interface
    CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() OVERRIDE { return this; }
    CefRefPtr<CefDisplayHandler> GetDisplayHandler() OVERRIDE { return this; }

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
            bool* no_javascript_access) OVERRIDE;
    void OnAfterCreated(CefRefPtr<CefBrowser> browser) OVERRIDE;
    bool DoClose(CefRefPtr<CefBrowser> browser) OVERRIDE;
    void OnBeforeClose(CefRefPtr<CefBrowser> browser) OVERRIDE;

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
    // Execute Delegate notifications on the main thread.
    void NotifyBrowserNewForgroundPage(CefWindowInfo& windowInfo,
                                       CefRefPtr<CefClient>& client,
                                       CefBrowserSettings& settings);
    void NotifyBrowserCreated(CefRefPtr<CefBrowser> browser);
    void NotifyBrowserClosing(CefRefPtr<CefBrowser> browser);
    void NotifyBrowserClosed(CefRefPtr<CefBrowser> browser);

    void NotifyBrowserAddressChange(CefRefPtr<CefBrowser> browser,
                                     CefRefPtr<CefFrame> frame,
                                     const CefString &url);
    void NotifyBroserTitleChange(CefRefPtr<CefBrowser> browser,
                                 const CefString &title);

    // The startup URL.
    const std::string startup_url_;

    // MAIN THREAD MEMBERS
    // The following members will only be accessed on the main thread. This will
    // be the same as the CEF UI thread except when using multi-threaded message
    // loop mode on Windows.

    Delegate* delegate_;

    static int total_created_;
    // The current number of browsers using this handler.
    int browser_count_ = 0;

    // Include the default reference counting implementation.
    IMPLEMENT_REFCOUNTING(CefClientHandler);
    DISALLOW_COPY_AND_ASSIGN(CefClientHandler);
};

#endif // CEFCLIENTHANDLER_H
