#ifndef CEFAPPRENDER_H
#define CEFAPPRENDER_H

#include "client_app.h"

class CefAppRender : public ClientApp, public CefRenderProcessHandler
{
public:
    CefAppRender();

    // CefRenderProcessHandler interface
public:
    void OnBrowserCreated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefDictionaryValue> extra_info) override;
    void OnBrowserDestroyed(CefRefPtr<CefBrowser> browser) override;
private:
    IMPLEMENT_REFCOUNTING(CefAppRender);
    DISALLOW_COPY_AND_ASSIGN(CefAppRender);
};

#endif // CEFAPPRENDER_H
