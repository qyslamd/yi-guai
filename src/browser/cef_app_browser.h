#ifndef CEFAPPBROWSER_H
#define CEFAPPBROWSER_H

#include <include/cef_app.h>
#include "clientapp.h"

class CefAppBrowser : public ClientApp , public CefBrowserProcessHandler
{
public:
    CefAppBrowser();

    // CefApp methods:
    virtual CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler()
    OVERRIDE {
        return this;
    }
    void OnBeforeCommandLineProcessing(const CefString& process_type,
                                       CefRefPtr<CefCommandLine> command_line)
    override;
    // CefBrowserProcessHandler methods
    void OnScheduleMessagePumpWork(int64 delay) override;

private:
    // Include the default reference counting implementation.
    IMPLEMENT_REFCOUNTING(CefAppBrowser);
};

#endif // CEFAPPBROWSER_H
