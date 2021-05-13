#ifndef BROWSERWINDOW_H
#define BROWSERWINDOW_H

#include "cef_client_handler.h"
#include "client_types.h"

// Represents a native child window hosting a single browser instance. The
// methods of this class must be called on the main thread unless otherwise
// indicated.
class BrowserWindow : public CefClientHandler::Delegate
{
public:
    // This interface is implemented by the owner of the BrowserWindow. The
    // methods of this class will be called on the main thread.
    class Delegate {
    public:
        virtual void onBrowserWindowNewForgroundPage(CefWindowInfo &windowInfo,
                                                     CefRefPtr<CefClient> &client,
                                                     CefBrowserSettings &settings) = 0;
        virtual void onBrowserWndPopupWnd(const CefPopupFeatures &popupFeatures,
                                          CefWindowInfo &windowInfo,
                                          CefRefPtr<CefClient> &client,
                                          CefBrowserSettings &settings) = 0;
        // Called when the browser has been created.
        virtual void OnBrowserCreated() = 0;

        // Called when the BrowserWindow is closing.
        virtual void OnBrowserWindowClosing() {};

        virtual void onBrowserWindowAddressChange(const std::string& url) = 0;
        virtual void onBrowserWindowTitleChange(const std::string& title) = 0;
        virtual void onBrowserWindowStatusMessage(const std::string &meg) = 0;
        virtual void onBrowserWindowFaviconChange(CefRefPtr<CefImage> image,
                                                  const std::string &url) = 0;
        virtual void onBrowerWindowLoadStart(CefLoadHandler::TransitionType transition_type) = 0;
        virtual void onBrowerWindowLoadEnd(int code) = 0;
        virtual void onBrowserWindowLoadingStateChange(bool isLoading,
                                                       bool canGoBack,
                                                       bool canGoForward) = 0;

        virtual void OnBrowserGotFocus() {};
    protected:
        virtual ~Delegate() {}
    };

    // Constructor may be called on any thread.
    // |delegate| must outlive this object.
    explicit BrowserWindow(Delegate* delegate, const std::string &url);

    ~BrowserWindow();
    // Create a new browser and native window.
    bool CreateBrowser(ClientWindowHandle parent_handle,
                       const CefRect& rect,
                       const CefBrowserSettings& settings,
                       CefRefPtr<CefDictionaryValue> extra_info,
                       CefRefPtr<CefRequestContext> request_context);

    // Retrieve the configuration that will be used when creating a popup window.
    // The popup browser will initially be parented to |temp_handle| which should
    // be a pre-existing hidden window. The native window will be created later
    // after the browser has been created. This method will be called on the
    // browser process UI thread.
    void GetPopupConfig(CefWindowHandle temp_handle,
                        CefWindowInfo& windowInfo,
                        CefRefPtr<CefClient>& client,
                        CefBrowserSettings& settings);
    // Show the popup window with correct parent and bounds in parent coordinates.
    void ShowPopup(ClientWindowHandle parent_handle,
                   int x,
                   int y,
                   size_t width,
                   size_t height);

    CefRefPtr<CefBrowser> GetBrowser() const;
    bool IsClosing() const;

    void onBrowserComfirmClose();
protected:
    // Allow deletion via scoped_ptr only.
    friend struct base::DefaultDeleter<BrowserWindow>;

    // ClientHandler::Delegate methods.
    void onBrowserForgroundTab(CefWindowInfo &windowInfo,
                                   CefRefPtr<CefClient> &client,
                                   CefBrowserSettings &settings) override;
    void onBrowserPopupWnd(const CefPopupFeatures& popupFeatures,
                           CefWindowInfo& windowInfo,
                           CefRefPtr<CefClient>& client,
                           CefBrowserSettings& settings) override;
    void OnBrowserCreated(CefRefPtr<CefBrowser> browser) override;
    void OnBrowserClosing(CefRefPtr<CefBrowser> browser) override;
    void onBrowserAddressChange(const std::string &url)  override;
    void onBrowserTitleChange(const std::string &title)  override;
    void onBrowserStatusMessage(const std::string &msg)  override;
    void onBrowserFaviconChange(CefRefPtr<CefImage> image,
                                const std::string &url) override;
    void onBrowerLoadStart(CefLoadHandler::TransitionType transition_type) override;
    void onBrowerLoadEnd(int httpStatusCode) override;
    void onBrowserLoadingStateChange(bool isLoading,
                                     bool canGoBack,
                                     bool canGoForward) override;
    void onBrowserGotFocus(CefRefPtr<CefBrowser> browser) override;

private:
    Delegate* delegate_;
    CefRefPtr<CefBrowser> browser_;
    CefRefPtr<CefClientHandler> client_handler_;
    bool is_closing_ = false;

private:
    DISALLOW_COPY_AND_ASSIGN(BrowserWindow);
};

#endif // BROWSERWINDOW_H
