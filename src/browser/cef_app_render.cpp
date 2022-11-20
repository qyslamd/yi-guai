#include "cef_app_render.h"
#include <QtDebug>

#include <iostream>

CefAppRender::CefAppRender()
{

}

void CefAppRender::OnBrowserCreated(CefRefPtr<CefBrowser> browser,
                                    CefRefPtr<CefDictionaryValue> extra_info)
{

}

void CefAppRender::OnBrowserDestroyed(CefRefPtr<CefBrowser> browser)
{

}

void CefAppRender::OnContextCreated(CefRefPtr<CefBrowser> browser,
                                    CefRefPtr<CefFrame> frame,
                                    CefRefPtr<CefV8Context> context)
{
    qInfo()<<__FUNCTION__;

    std::cout<<__FUNCTION__<<std::endl;
}


CefRefPtr<CefRenderProcessHandler> CefAppRender::GetRenderProcessHandler()
{
    return this;
}
