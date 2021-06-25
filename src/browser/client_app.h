#ifndef CLIENT_APP_H
#define CLIENT_APP_H

#include <vector>

#include <include/cef_app.h>

// Base class for customizing process-type-based behavior.
class ClientApp : public CefApp {
public:
    ClientApp();

    enum ProcessType {
        BrowserProcess,
        RendererProcess,
        ZygoteProcess,
        OtherProcess,
    };

    // Determine the process type based on command-line arguments.
    static ProcessType GetProcessType(CefRefPtr<CefCommandLine> command_line);

private:
    // Registers custom schemes. Implemented by cefclient in
    // client_app_delegates.cc
//    static void RegisterCustomSchemes(CefRawPtr<CefSchemeRegistrar> registrar, std::vector<CefString>& cookiable_schemes);

    // CefApp methods.
//    void OnRegisterCustomSchemes(CefRawPtr<CefSchemeRegistrar> registrar) override;

    DISALLOW_COPY_AND_ASSIGN(ClientApp);
};

#endif // CLIENT_APP_H
