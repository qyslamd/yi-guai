#ifndef CEFAPPRENDER_H
#define CEFAPPRENDER_H

#include "client_app.h"

class CefAppRender : public ClientApp, public CefRenderProcessHandler
{
public:
    CefAppRender();

    // CefApp interface
public:
    CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler() override;
    // CefRenderProcessHandler interface
public:
    void OnBrowserCreated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefDictionaryValue> extra_info) override;
    void OnBrowserDestroyed(CefRefPtr<CefBrowser> browser) override;
    void OnContextCreated(CefRefPtr<CefBrowser> browser,
                                    CefRefPtr<CefFrame> frame,
                                    CefRefPtr<CefV8Context> context) override;

private:
    IMPLEMENT_REFCOUNTING(CefAppRender);
    DISALLOW_COPY_AND_ASSIGN(CefAppRender);


};

#endif // CEFAPPRENDER_H
