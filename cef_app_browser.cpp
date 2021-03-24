#include "cef_app_browser.h"

#include "message_loop/main_message_loop_external_pump.h"

CefAppBrowser::CefAppBrowser()
{

}

void CefAppBrowser::OnBeforeCommandLineProcessing(
        const CefString& process_type,
        CefRefPtr<CefCommandLine> command_line)
{

}

void CefAppBrowser::OnScheduleMessagePumpWork(int64 delay)
{
    // Only used when `--external-message-pump` is passed via the command-line.
    client::MainMessageLoopExternalPump* message_pump = client::MainMessageLoopExternalPump::Get();
    if (message_pump)
    {
        message_pump->OnScheduleMessagePumpWork(delay);
    }
}
