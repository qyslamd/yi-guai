#ifndef SCHEME_HANDLER_H
#define SCHEME_HANDLER_H

#include <include/cef_browser.h>
#include <include/cef_callback.h>
#include <include/cef_frame.h>
#include <include/cef_request.h>
#include <include/cef_resource_handler.h>
#include <include/cef_response.h>
#include <include/cef_scheme.h>
#include <include/wrapper/cef_helpers.h>

namespace custom_scheme {
    class ClientSchemeHandler : public CefResourceHandler
    {
    public:
        ClientSchemeHandler();

        // CefResourceHandler interface
    public:
        bool Open(CefRefPtr<CefRequest> request, bool &handle_request, CefRefPtr<CefCallback> callback) override;
        void GetResponseHeaders(CefRefPtr<CefResponse> response, int64 &response_length, CefString &redirectUrl) override;
        bool Read(void *data_out, int bytes_to_read, int &bytes_read, CefRefPtr<CefResourceReadCallback> callback) override;
        void Cancel() override;
    private:
        std::string data_;
        std::string mime_type_;
        size_t offset_;

        IMPLEMENT_REFCOUNTING(ClientSchemeHandler);
         DISALLOW_COPY_AND_ASSIGN(ClientSchemeHandler);
    };

    // Implementation of the factory for for creating schema handlers.
    class ClientSchemeHandlerFactory : public CefSchemeHandlerFactory {
     public:
        ClientSchemeHandlerFactory(){};
        // Return a new scheme handler instance to handle the request.
        CefRefPtr<CefResourceHandler> Create(CefRefPtr<CefBrowser> browser,
                                             CefRefPtr<CefFrame> frame,
                                             const CefString& scheme_name,
                                             CefRefPtr<CefRequest> request) override;
    private:
        IMPLEMENT_REFCOUNTING(ClientSchemeHandlerFactory);
        DISALLOW_COPY_AND_ASSIGN(ClientSchemeHandlerFactory);
    };

    void RegisterSchemeHandlers();

}   // namespace custom_scheme


#endif // SCHEME_HANDLER_H
