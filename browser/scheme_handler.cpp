#include "scheme_handler.h"

#include <QtDebug>
#include <QFile>
#include <QBuffer>
#include <QPixmap>
#include <QUrl>

#include "utils/util_qt.h"

// std::min和VC编译器定义的宏冲突
#ifdef _MSC_VER

#ifdef min
#undef min
#endif

#endif

namespace custom_scheme {

    ClientSchemeHandler::ClientSchemeHandler()
        : offset_(0)
    {

    }

    bool ClientSchemeHandler::Open(CefRefPtr<CefRequest> request,
                              bool &handle_request,
                              CefRefPtr<CefCallback> callback)
    {
        DCHECK(!CefCurrentlyOn(TID_UI) && !CefCurrentlyOn(TID_IO));
        // The request will be continued or canceled based on the return value.
        handle_request = true;

        bool handled = false;

        std::string url = request->GetURL();
        QUrl qUrl(QString::fromStdString(url));
        auto path = qUrl.path();
        qInfo()<<__FUNCTION__<<path;
        if (path.compare( "/", Qt::CaseInsensitive) == 0 || path.isEmpty()) {
            auto content = UtilQt::readFileString(":/htmls/resources/html/HomePage.html");
            data_.append(content.toStdString());

            handled = true;
            // Set the resulting mime type
            mime_type_ = "text/html";
        }else if (strstr(url.c_str(), "logo.png") != nullptr) {
            // Load the response image
            QBuffer buffer;
            QPixmap(":/icons/resources/imgs/liumang.jpg").save(&buffer,"jpg");
            data_.append(buffer.data().data());
            handled = true;
            // Set the resulting mime type
            mime_type_ = "image/jpg";
          }

          return handled;
    }

    void ClientSchemeHandler::GetResponseHeaders(CefRefPtr<CefResponse> response,
                                           int64 &response_length,
                                           CefString &redirectUrl)
    {
        CEF_REQUIRE_IO_THREAD();

        DCHECK(!data_.empty());

        response->SetMimeType(mime_type_);
        response->SetStatus(200);

        // Set the resulting response length
        response_length = data_.length();
    }

    bool ClientSchemeHandler::Read(void *data_out,
                             int bytes_to_read,
                             int &bytes_read,
                             CefRefPtr<CefResourceReadCallback> callback)
    {
        DCHECK(!CefCurrentlyOn(TID_UI) && !CefCurrentlyOn(TID_IO));

        bool has_data = false;
        bytes_read = 0;

        if (offset_ < data_.length()) {
            // Copy the next block of data into the buffer.
            int transfer_size =
                    std::min(bytes_to_read, static_cast<int>(data_.length() - offset_));
            memcpy(data_out, data_.c_str() + offset_, transfer_size);
            offset_ += transfer_size;

            bytes_read = transfer_size;
            has_data = true;
        }

        return has_data;
    }

    void ClientSchemeHandler::Cancel()
    {
        CEF_REQUIRE_IO_THREAD();
    }

    CefRefPtr<CefResourceHandler> ClientSchemeHandlerFactory::Create(CefRefPtr<CefBrowser> browser,
                                                                     CefRefPtr<CefFrame> frame,
                                                                     const CefString &scheme_name,
                                                                     CefRefPtr<CefRequest> request)
    {
        CEF_REQUIRE_IO_THREAD();
        return new ClientSchemeHandler();
    }

    void RegisterSchemeHandlers()
    {
        // para1 is "schem"
        // para2 is "domain"
        CefRegisterSchemeHandlerFactory("yiguai", "tests",
                                          new ClientSchemeHandlerFactory());
    }

}   // namespace custom_scheme
