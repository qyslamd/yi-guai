#include "cef_app_browser.h"

#include "message_loop/main_message_loop_external_pump.h"
#include "client_switches.h"

#include <QtDebug>

CefAppBrowser::CefAppBrowser()
{

}

void CefAppBrowser::OnBeforeCommandLineProcessing(
        const CefString& process_type,
        CefRefPtr<CefCommandLine> command_line)
{
    using namespace client;
    if(!command_line->HasSwitch("enable-print-preview")){
      printf("enable-print-preview\n");
      command_line->AppendSwitch("enable-print-preview");
    }
    if(!command_line->HasSwitch("allow-silent-push")){
      printf("allow-silent-push\n");
      command_line->AppendSwitch("allow-silent-push");
    }

    /// 关闭同源策略
    command_line->AppendSwitch("--disable-web-security");
    command_line->AppendSwitch("--allow-file-access-from-files");

    // 开启摄像头
    command_line->AppendSwitch("enable-media-stream");
    command_line->AppendSwitchWithValue("enable-media-stream", "1");
    command_line->AppendSwitch("enable-speech-input");
    command_line->AppendSwitch("enable-desktop-notifications");

    // 合并渲染进程
    command_line->AppendSwitch("process-per-site");

    // 开启打印预览
    if(!command_line->HasSwitch("enable-print-preview")){
      command_line->AppendSwitch("enable-print-preview");
    }

    // Pass additional command-line flags to the browser process.
    if (process_type.empty()) {
        // Pass additional command-line flags when off-screen rendering is enabled.
        if (command_line->HasSwitch(switches::kOffScreenRenderingEnabled)) {
            // If the PDF extension is enabled then cc Surfaces must be disabled for
            // PDFs to render correctly.
            // See https://bitbucket.org/chromiumembedded/cef/issues/1689 for details.
            if (!command_line->HasSwitch("disable-extensions") &&
                    !command_line->HasSwitch("disable-pdf-extension")) {
                command_line->AppendSwitch("disable-surfaces");
            }

            // Use software rendering and compositing (disable GPU) for increased FPS
            // and decreased CPU usage. This will also disable WebGL so remove these
            // switches if you need that capability.
            // See https://bitbucket.org/chromiumembedded/cef/issues/1257 for details.
            if (!command_line->HasSwitch(switches::kEnableGPU)) {
                command_line->AppendSwitch("disable-gpu");
                command_line->AppendSwitch("disable-gpu-compositing");
            }
        }

        if (command_line->HasSwitch(switches::kUseViews) &&
                !command_line->HasSwitch("top-chrome-md")) {
            // Use non-material mode on all platforms by default. Among other things
            // this causes menu buttons to show hover state. See usage of
            // MaterialDesignController::IsModeMaterial() in Chromium code.
            command_line->AppendSwitchWithValue("top-chrome-md", "non-material");
        }

        if (!command_line->HasSwitch(switches::kCachePath) &&
                !command_line->HasSwitch("disable-gpu-shader-disk-cache")) {
            // Don't create a "GPUCache" directory when cache-path is unspecified.
            command_line->AppendSwitch("disable-gpu-shader-disk-cache");
        }
    }
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
