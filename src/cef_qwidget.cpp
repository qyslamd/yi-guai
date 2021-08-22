#include "cef_qwidget.h"

#if defined Q_OS_WIN
#include <Windows.h>
#include <SHlObj.h>
#include <WinUser.h>
#else
#include "utils/windowskeyboardcodes.h"
#include "browser/client_types.h"
#endif

#include <QtDebug>
#include <QThread>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QCloseEvent>
#include <QTimer>
#include <QDateTime>
#include <QStyle>
#include <QStandardPaths>
#include <QStyle>
#include <QDir>
#include <QMessageBox>
#include <QDateTime>
#include <QPixmap>
#include <QBuffer>
#include <QScreen>
#include "popups/StyledMenu.h"

#include <include/base/cef_logging.h>
#include "mainwindow.h"
#include "popup.h"
#include "page.h"

#include "managers/FaviconManager.h"
#include "managers/MainWindowManager.h"
#include "managers/AppCfgManager.h"
#include "managers/HistoryManager.h"
#include "utils/util_qt.h"
#include "dialogs/alertdialog.h"


#ifdef OS_LINUX
#include <X11/Xlib.h>
#undef Success     // Definition conflicts with cef_message_router.h
#undef RootWindow  // Definition conflicts with root_window.h
#undef Bool // Definition conflicts with X11/Xlib.h

namespace  {
void SetXWindowBounds(::Window xwindow,
                      int x,
                      int y,
                      size_t width,
                      size_t height) {
    ::Display* xdisplay = cef_get_xdisplay();
    XWindowChanges changes = {0};
    changes.x = x;
    changes.y = y;
    changes.width = static_cast<int>(width);
    changes.height = static_cast<int>(height);
    XConfigureWindow(xdisplay, xwindow, CWX | CWY | CWHeight | CWWidth, &changes);
}

//::Window GetXWindowForWidget(GtkWidget* widget) {
//  ScopedGdkThreadsEnter scoped_gdk_threads;

//  // The GTK window must be visible before we can retrieve the XID.
//  ::Window xwindow = GDK_WINDOW_XID(gtk_widget_get_window(widget));
//  DCHECK(xwindow);
//  return xwindow;
//}

}  // namespace

#endif  // OS_LINUX


CefQWidget::CefQWidget(const QString &url, QWidget *parent)
    : QWidget(parent)
    , qwindow_containter_(nullptr)
    , layout_(new QVBoxLayout(this))
{
    window_ = new QWindow(windowHandle());
    browser_window_.reset(new BrowserWindow(this, url.toStdString()));
#if defined (Q_OS_LINUX)
    CefRect rect{x(), y(), window_->size().width(), window_->size().height()};
    CefBrowserSettings browser_settings;
    browser_window_->CreateBrowser(window_->winId(),
                                   rect,
                                   browser_settings,
                                   nullptr,
                                   nullptr);
#endif
    initUi();
}

CefQWidget::CefQWidget(CefWindowInfo &windowInfo,
                       CefRefPtr<CefClient> &client,
                       CefBrowserSettings &settings,
                       QWidget *parent)
    : QWidget(parent)
    , qwindow_containter_(nullptr)
    , layout_(new QVBoxLayout(this))
{
    window_ = new QWindow(windowHandle());
#ifdef Q_OS_WIN
    window_->setFlag(Qt::FramelessWindowHint);
#endif
    browser_window_.reset(new BrowserWindow(this, ""));
    browser_window_->GetPopupConfig((ClientWindowHandle)window_->winId(),
                                    windowInfo, client, settings);
    initUi();
    browser_state_ = Creating;
}

CefQWidget::~CefQWidget()
{
    qInfo()<<__FUNCTION__;
}

QSize CefQWidget::sizeHint() const
{
    return QSize(300, 300);
}

void CefQWidget::Navigate(const QString &url)
{
    auto browser = browser_window_->GetBrowser();
    if(browser){
        browser->GetMainFrame()->LoadURL(url.toStdString());
    }
}

void CefQWidget::GoBack()
{
    auto browser = browser_window_->GetBrowser();
    if(browser){
        if(browser->CanGoBack()){
            browser->GoBack();
        }
    }
}

void CefQWidget::GoForward()
{
    auto browser = browser_window_->GetBrowser();
    if(browser){
        if(browser->CanGoForward()){
            browser->GoForward();
        }
    }
}

void CefQWidget::Refresh()
{
    qInfo()<<__FUNCTION__;
    auto browser = browser_window_->GetBrowser();
    if(browser){
        browser->Reload();
    }
}

void CefQWidget::MuteAudio()
{
    auto browser = browser_window_->GetBrowser();
    if(browser){
        auto host = browser->GetHost();
        bool ismuted = host->IsAudioMuted();
        browser->GetHost()->SetAudioMuted(!ismuted);
    }
}

void CefQWidget::StopLoading()
{
    auto browser = browser_window_->GetBrowser();
    if(browser){
        if(browser->IsLoading()){
            browser->StopLoad();
        }
    }
}

void CefQWidget::ZoomOut()
{
    auto browser = browser_window_->GetBrowser();
    if(browser){
        auto old = browser->GetHost()->GetZoomLevel();
        double delta = old <= -7.0 ? 0.0 : -1.0;
        browser->GetHost()->SetZoomLevel(old + delta);
    }
}

void CefQWidget::ZoomIn()
{
    auto browser = browser_window_->GetBrowser();
    if(browser){
        auto old = browser->GetHost()->GetZoomLevel();
        double delta = old >= 9.0 ? 0.0 : 1.0;
        browser->GetHost()->SetZoomLevel(old + delta);
    }
}

void CefQWidget::ZoomReset()
{
    auto browser = browser_window_->GetBrowser();
    if(browser){
        browser->GetHost()->SetZoomLevel(0.0);
    }
}

double CefQWidget::ZoomLevel()
{
    auto browser = browser_window_->GetBrowser();
    if(browser){
        return browser->GetHost()->GetZoomLevel();
    }
    return 0.0;
}

void CefQWidget::Print()
{
    auto browser = browser_window_->GetBrowser();
    if(browser){
        browser->GetHost()->Print();
    }
}

void CefQWidget::ShowDevTool(const QPoint &pos)
{
    browser_window_->GetHandler()->ShowDevTools(browser_window_->GetBrowser(),
                                                CefPoint(pos.x(), pos.y()));
}

CefWindowHandle CefQWidget::BrowserWindowHandle()
{
    auto browser = browser_window_->GetBrowser();
    if(browser){
        return browser->GetHost()->GetWindowHandle();
    }
    return 0;
}

void CefQWidget::onTopLevelWindowStateChanged(Qt::WindowStates state,
                                              const QVariant &data)
{
    //    resizeBorser(data.toSize());
}

void CefQWidget::onBrowserBeforeContextMenu(CefRefPtr<CefBrowser> browser,
                                            CefRefPtr<CefFrame> frame,
                                            CefRefPtr<CefContextMenuParams> params,
                                            CefRefPtr<CefMenuModel> model)
{
    StyledMenu menu;

    menu.exec(QCursor::pos());
}

void CefQWidget::onBrowserWndNewForgroundPage(CefWindowInfo &windowInfo,
                                                 CefRefPtr<CefClient> &client,
                                                 CefBrowserSettings &settings)
{
    CefQWidget *window = new CefQWidget(windowInfo, client, settings);

    emit browserNewForgroundPage(window);
}

void CefQWidget::onBrowserWndPopupWnd(const CefPopupFeatures &popupFeatures,
                                      CefWindowInfo &windowInfo,
                                      CefRefPtr<CefClient> &client,
                                      CefBrowserSettings &settings)
{
    CefQWidget *window = new CefQWidget(windowInfo, client, settings);

    Popup *popupBrowser = new Popup(window);
    popupBrowser->setAttribute(Qt::WA_DeleteOnClose);

    /* CefPopupFeatures
  int x;
  int xSet;
  int y;
  int ySet;
  int width;
  int widthSet;
  int height;
  int heightSet;

  int menuBarVisible;
  int statusBarVisible;
  int toolBarVisible;
  int scrollbarsVisible;
*/

    auto rect = MainWndMgr::Instance().lastWindowGeometry();

    int x = popupFeatures.x;
    int y = popupFeatures.y;
    int w = popupFeatures.width;
    int h = popupFeatures.height;

   if(x == 0 && y == 0 && w == 0 && h == 0){
       x = rect.x() + MainWndMgr::newWndOffsetX;
       y = rect.y() + MainWndMgr::newWndOffsetY;
       w = rect.width();
       h = rect.height();
   }else{
       x = x < 0 ? 0 : x;
       y = y < 0 ? 0 : y;
       w = w <= 150 ? rect.width() : w;
       h = h <= 150 ? rect.height() : h;
   }

    popupBrowser->setGeometry(x, y, w, h);
    popupBrowser->show();
}

void CefQWidget::onBrowserWndDevTools(CefWindowInfo &windowInfo,
                                          CefRefPtr<CefClient> &client,
                                          CefBrowserSettings &settings)
{
    CefQWidget *window = new CefQWidget(windowInfo, client, settings);
    window->is_dev_tool_ = true;
    emit browserDevTool(window);
}

void CefQWidget::OnBrowserCreated(CefRefPtr<CefBrowser> browser)
{
    browser_ = browser;
    browser_state_ = Created;
    resizeBrowser();
    emit browserCreated();
}

void CefQWidget::OnBrowserWindowClosing()
{
    emit browserClosing();

    close();
}

void CefQWidget::onBrowserWindowAddressChange(const std::string &url)
{
    url_ = QString::fromStdString(url);
    emit browserAddressChange(QString::fromStdString(url));
}

void CefQWidget::onBrowserWindowTitleChange(const std::string &title)
{
    title_ = QString::fromStdString(title);
    emit browserTitleChange(title_);
}

void CefQWidget::onBrowserWndFullscreenChange(bool fullscreen)
{
    emit browserFullScnChange(fullscreen);
}

void CefQWidget::onBrowserWindowStatusMessage(const std::string &msg)
{
    emit browserStatusMessage(QString::fromStdString(msg));
}

void CefQWidget::onBrowserWindowFaviconChange(CefRefPtr<CefImage> image,
                                      const std::string &url)
{
    if(image == nullptr || image->IsEmpty())
    {
        // 没有图添加图
        auto pix = style()->standardPixmap(QStyle::SP_ComputerIcon);
        QBuffer buffer;
        pix.save(&buffer, "png");
        image->AddPNG(1.0, buffer.data().data(), buffer.data().size());
    }
    auto cacheDir = UtilQt::appDataPath();
    QDir dir(cacheDir);
    if(!dir.exists()){
        dir.mkpath(dir.path());
    }
    const char * sudDir = "iconCache";
    if(!dir.cd(sudDir)){
        dir.mkdir(sudDir);
    }
    dir.cd(sudDir);

    auto file_path = UtilQt::GetFileNameFromURL(QString::fromStdString(url));
    file_path = dir.absoluteFilePath(file_path);
    QString subFix = QFileInfo(QString::fromStdString(url)).suffix();
    int width = image->GetWidth();
    int height = image->GetHeight();
    QPixmap pixmap;

    // 有些臃肿，故意如此
    if(subFix.compare("svg", Qt::CaseInsensitive) == 0)
    {
        CefRefPtr<CefBinaryValue> value = image->GetAsPNG(1.0, true,width, height);
        uchar * buffer = (uchar*)malloc(value->GetSize());
        memset(buffer, 0, value->GetSize());
        value->GetData(buffer, value->GetSize(), 0);

        pixmap.loadFromData(buffer,value->GetSize());
        free(buffer);
    }
    else if(subFix.compare("ico", Qt::CaseInsensitive) == 0)
    {
        CefRefPtr<CefBinaryValue> value = image->GetAsPNG(0.0, true, width, height);
        uchar * buffer = (uchar*)malloc(value->GetSize());
        memset(buffer, 0, value->GetSize());
        value->GetData(buffer, value->GetSize(), 0);
        pixmap.loadFromData(buffer,value->GetSize());
        free(buffer);

        if(! QFile(file_path).exists()){
            pixmap.save(file_path);
        }
    }
    else if(subFix.compare("png", Qt::CaseInsensitive) == 0)
    {
        CefRefPtr<CefBinaryValue> value = image->GetAsPNG(0.0, true, width, height);
        uchar * buffer = (uchar*)malloc(value->GetSize());
        memset(buffer, 0, value->GetSize());
        value->GetData(buffer, value->GetSize(), 0);

        pixmap.loadFromData(buffer,value->GetSize());
        free(buffer);

        if(! QFile(file_path).exists()){
            pixmap.save(file_path);
        }
    }
    else {
        pixmap = style()->standardPixmap(QStyle::SP_FileIcon);
    }
    emit browserFaviconChange(pixmap);

    Q_UNUSED(url);
}

void CefQWidget::onBrowerWindowLoadStart(CefLoadHandler::TransitionType transition_type)
{
    emit browserLoadStart(transition_type);
}

void CefQWidget::onBrowerWindowLoadEnd(int httpStatusCode)
{
    if(newly_created_){
        emit browserNeedSize();
        newly_created_ = false;
    }
    HistoryMgr::Instance()->addHistoryRecord(
                History{QString::number(QDateTime::currentMSecsSinceEpoch()),
                        url_,
                        title_,
                        0});
    emit browserLoadEnd(httpStatusCode);
}

void CefQWidget::onBrowserWndLoadingProgressChange(double progress)
{
    emit browserLoadingProgress(progress);
//    qInfo()<<__FUNCTION__<<progress;
}

void CefQWidget::onBrowserWindowLoadingStateChange(bool isLoading,
                                                   bool canGoBack,
                                                   bool canGoForward)
{
    emit browserLoadingStateChange(isLoading,
                                   canGoBack,
                                   canGoForward);
}

void CefQWidget::OnBrowserGotFocus()
{
    emit browserFocusChange(true);
}

bool CefQWidget::onBrowserWndPreKeyEvent(const CefKeyEvent &event,
                                         CefEventHandle os_event,
                                         bool *is_keyboard_shortcut)
{
    if(event.focus_on_editable_field){
        return false;
    }

#if 0
    qInfo()<<__FUNCTION__
          <<"type:"<<event.type
         <<"modifiers:"<<QString::number(event.modifiers,2)
        <<"windows_key_code:"<<QString::number(event.windows_key_code,16).toUpper().prepend("0x")
       <<"native_key_code:"<<QString::number(event.native_key_code,16).toUpper().prepend("0x");
#endif
    // 开发者工具的 browser如果在PreKeyEvent注册了快捷键，那么onKeyEvent就会失效。
    if(!is_dev_tool_){
        dealCefKeyEvent(event, os_event, is_keyboard_shortcut);
    }
    return false;
}

bool CefQWidget::onBrowserWndKeyEvent(const CefKeyEvent &event,
                                      CefEventHandle os_event)
{
    dealCefKeyEvent(event, os_event, nullptr, false);
    return false;
}

void CefQWidget::dealCefKeyEvent(const CefKeyEvent &event,
                                 CefEventHandle os_event,
                                 bool *is_keyboard_shortcut,
                                 bool isPre)
{
    Q_UNUSED(os_event);

    // F5
    // 刷新当前标签页
    if (event.modifiers == EVENTFLAG_NONE
            && event.windows_key_code == VK_F5
            && event.type == KEYEVENT_RAWKEYDOWN)
    {
            if(isPre && is_keyboard_shortcut){
                *is_keyboard_shortcut = true;
            }else{
                emit browserShortcut(CefShortcutCmd::Refresh);
            }
    }

    // F11
    // 改变浏览器的全屏模式
    if (event.modifiers == EVENTFLAG_NONE
            && event.windows_key_code == VK_F11
            && event.type == KEYEVENT_RAWKEYDOWN)
    {
        if(isPre && is_keyboard_shortcut){
            *is_keyboard_shortcut = true;
        }else{
            emit browserShortcut(CefShortcutCmd::Fullscreen);
        }
    }

    // F12
    // 打开/关闭开发者工具
    if (event.modifiers == EVENTFLAG_NONE
            && event.windows_key_code == VK_F12
            && event.type == KEYEVENT_RAWKEYDOWN)
    {
        if(isPre && is_keyboard_shortcut){
            *is_keyboard_shortcut = true;
        }else{
            emit browserShortcut(CefShortcutCmd::DevTool);
        }
    }

    // Ctrl + -(- 位于 主键盘 按键 0 右侧)
    // 缩小
    if (event.modifiers == EVENTFLAG_CONTROL_DOWN
            && event.windows_key_code == VK_OEM_MINUS
            && event.type == KEYEVENT_RAWKEYDOWN)
    {
        if(isPre && is_keyboard_shortcut){
            *is_keyboard_shortcut = true;
        }else{
            emit browserShortcut(CefShortcutCmd::ZoomOut);
        }
    }

    // Ctrl + -(- 位于 小键盘 )
    // 缩小
    if (event.modifiers == (EVENTFLAG_CONTROL_DOWN | EVENTFLAG_IS_KEY_PAD)
            && event.windows_key_code == VK_SUBTRACT
            && event.type == KEYEVENT_RAWKEYDOWN)
    {
        if(isPre && is_keyboard_shortcut){
            *is_keyboard_shortcut = true;
        }else{
            emit browserShortcut(CefShortcutCmd::ZoomOut);
        }
    }

    /* from WinUser.h
     * VK_0 - VK_9 are the same as ASCII '0' - '9' (0x30 - 0x39)
     * 0x3A - 0x40 : unassigned
     * VK_A - VK_Z are the same as ASCII 'A' - 'Z' (0x41 - 0x5A)
     */
    // Ctrl + 0(0 位于 主键盘)
    // 恢复缩放比例
    if (event.modifiers == EVENTFLAG_CONTROL_DOWN
            && event.windows_key_code == '0'
            && event.type == KEYEVENT_RAWKEYDOWN)
    {
        if(isPre && is_keyboard_shortcut){
            *is_keyboard_shortcut = true;
        }else{
            emit browserShortcut(CefShortcutCmd::ZoomReset);
        }
    }

    // Ctrl + 0(0 位于 小键盘 )
    // 恢复缩放比例
    if (event.modifiers == (EVENTFLAG_CONTROL_DOWN | EVENTFLAG_IS_KEY_PAD)
            && event.windows_key_code == VK_NUMPAD0
            && event.type == KEYEVENT_RAWKEYDOWN)
    {
        if(isPre && is_keyboard_shortcut){
            *is_keyboard_shortcut = true;
        }else{
            emit browserShortcut(CefShortcutCmd::ZoomReset);
        }
    }

    // Ctrl + +(+ 位于 backspace 左侧)
    // 放大
    if (event.modifiers == EVENTFLAG_CONTROL_DOWN
            && event.windows_key_code == VK_OEM_PLUS
            && event.type == KEYEVENT_RAWKEYDOWN)
    {
        if(isPre && is_keyboard_shortcut){
            *is_keyboard_shortcut = true;
        }else{
            emit browserShortcut(CefShortcutCmd::ZoomIn);
        }
    }

    // Ctrl + +(+ 位于 小键盘 )
    // 放大
    if (event.modifiers == (EVENTFLAG_CONTROL_DOWN | EVENTFLAG_IS_KEY_PAD)
            && event.windows_key_code == VK_ADD
            && event.type == KEYEVENT_RAWKEYDOWN)
    {
        if(isPre && is_keyboard_shortcut){
            *is_keyboard_shortcut = true;
        }else{
            emit browserShortcut(CefShortcutCmd::ZoomIn);
        }
    }

    // Ctrl + P
    // 打印
    if (event.modifiers == EVENTFLAG_CONTROL_DOWN
            && event.windows_key_code == 'P'
            && event.type == KEYEVENT_RAWKEYDOWN)
    {
        if(isPre && is_keyboard_shortcut){
            *is_keyboard_shortcut = true;
        }else{
            emit browserShortcut(CefShortcutCmd::Print);
        }
    }

    // Ctrl + R
    // 刷新当前标签页
    if (event.modifiers == EVENTFLAG_CONTROL_DOWN
            && event.windows_key_code == 'R'
            && event.type == KEYEVENT_RAWKEYDOWN)
    {
        if(isPre && is_keyboard_shortcut){
            *is_keyboard_shortcut = true;
        }else{
            emit browserShortcut(CefShortcutCmd::Refresh);
        }
    }

    // Ctrl + T
    // 新建标签页
    if (event.modifiers == EVENTFLAG_CONTROL_DOWN
            && event.windows_key_code == 'T'
            && event.type == KEYEVENT_RAWKEYDOWN)
    {
        if(isPre && is_keyboard_shortcut){
            *is_keyboard_shortcut = true;
        }else{
            emit browserShortcut(CefShortcutCmd::NewTab);
        }
    }

    // Ctrl + N
    // 新建浏览器窗口
    if (event.modifiers == EVENTFLAG_CONTROL_DOWN
            && event.windows_key_code == 'N'
            && event.type == KEYEVENT_RAWKEYDOWN)
    {
        if(isPre && is_keyboard_shortcut){
            *is_keyboard_shortcut = true;
        }else{
            emit browserShortcut(CefShortcutCmd::NewWnd);
        }
    }

    // Ctrl + Shift + N
    // 新建InPrivate浏览器窗口
    if (event.modifiers == (EVENTFLAG_CONTROL_DOWN | EVENTFLAG_SHIFT_DOWN)
            && event.windows_key_code == 'N'
            && event.type == KEYEVENT_RAWKEYDOWN)
    {
        if(isPre && is_keyboard_shortcut){
            *is_keyboard_shortcut = true;
        }else{
            emit browserShortcut(CefShortcutCmd::NewPrivateWnd);
        }
    }

    // Ctrl + Shift + I
    // 打开开发者工具
    if (event.modifiers == (EVENTFLAG_CONTROL_DOWN | EVENTFLAG_SHIFT_DOWN)
            && event.windows_key_code == 'I'
            && event.type == KEYEVENT_RAWKEYDOWN)
    {
        if(isPre && is_keyboard_shortcut){
            *is_keyboard_shortcut = true;
        }else{
            emit browserShortcut(CefShortcutCmd::DevTool);
        }
    }

    // Ctrl + H
    // 查看历史记录
    if (event.modifiers == EVENTFLAG_CONTROL_DOWN
            && event.windows_key_code == 'H'
            && event.type == KEYEVENT_RAWKEYDOWN)
    {
        if(isPre && is_keyboard_shortcut){
            *is_keyboard_shortcut = true;
        }else{
            emit browserShortcut(CefShortcutCmd::History);
        }
    }

    // Ctrl + J
    // 查看下载
    if (event.modifiers == EVENTFLAG_CONTROL_DOWN
            && event.windows_key_code == 'J'
            && event.type == KEYEVENT_RAWKEYDOWN)
    {
        if(isPre && is_keyboard_shortcut){
            *is_keyboard_shortcut = true;
        }else{
            emit browserShortcut(CefShortcutCmd::Download);
        }
    }

    // Ctrl + W
    // 关闭当前标签页
    if (event.modifiers == EVENTFLAG_CONTROL_DOWN
            && event.windows_key_code == 'W'
            && event.type == KEYEVENT_RAWKEYDOWN)
    {
        if(isPre && is_keyboard_shortcut){
            *is_keyboard_shortcut = true;
        }else{
            emit browserShortcut(CefShortcutCmd::CloseTab);
        }
    }

    // Ctrl + Tab
    // 标签页切换
    if(event.modifiers == EVENTFLAG_CONTROL_DOWN
            && event.windows_key_code == VK_TAB
            && event.type == KEYEVENT_RAWKEYDOWN)
    {
        if(isPre && is_keyboard_shortcut){
            *is_keyboard_shortcut = true;
        }else{
            emit browserShortcut(CefShortcutCmd::SwitchTab);
        }
    }
    // Alt + <--(左箭头)
    if(event.modifiers == EVENTFLAG_ALT_DOWN
            && event.windows_key_code == VK_LEFT
            && event.type == KEYEVENT_RAWKEYDOWN)
    {
        if(isPre && is_keyboard_shortcut){
            *is_keyboard_shortcut = true;
        }else{
            emit browserShortcut(CefShortcutCmd::NaviBack);
        }
    }
    // Alt + -->(右箭头)
    if(event.modifiers == EVENTFLAG_ALT_DOWN
            && event.windows_key_code == VK_RIGHT
            && event.type == KEYEVENT_RAWKEYDOWN)
    {
        if(isPre && is_keyboard_shortcut){
            *is_keyboard_shortcut = true;
        }else{
            emit browserShortcut(CefShortcutCmd::NaviForward);
        }
    }
}

void CefQWidget::onScreenChanged(QScreen *)
{
    resizeBrowser();
}

void CefQWidget::resizeEvent(QResizeEvent *event)
{
#ifdef Q_OS_LINUX
    resizeBrowser(event->size());
#else
    switch(browser_state_){
    case Empty:
    {
        auto handle = (ClientWindowHandle)window_->winId();
        CefRect rect{x(), y(), event->size().width(), event->size().height()};
        CefBrowserSettings browser_settings;
        browser_window_->CreateBrowser(handle,
                                       rect,
                                       browser_settings,
                                       nullptr,
                                       nullptr);
        browser_state_ = Creating;
    }break;
    case Creating:
        break;
    case Created:
        resizeBrowser(event->size());
        break;
    }
#endif
}

void CefQWidget::closeEvent(QCloseEvent *event)
{
    // 关闭浏览器触发点
    if(browser_window_ && !browser_window_->IsClosing()){
        qInfo()<<__FUNCTION__;
        auto browser = browser_window_->GetBrowser();
        if(browser){
            // Notify the browser window that we would like to close it. This
            // will result in a call to ClientHandler::DoClose() if the
            // JavaScript 'onbeforeunload' event handler allows it.
            browser->GetHost()->CloseBrowser(false);
        }
        // Cancel the close.
        event->ignore();
    }
}

void CefQWidget::initUi()
{
    if(!qwindow_containter_){
        qwindow_containter_ = QWidget::createWindowContainer(window_);
    }

    layout_->setContentsMargins(0,0,0,0);
    layout_->setSpacing(0);
    layout_->addWidget(qwindow_containter_);
    setLayout(layout_);

    connect(window_, &QWindow::screenChanged, this, &CefQWidget::onScreenChanged);
}

void CefQWidget::resizeBrowser(const QSize &size)
{
    QRect rect;
    if(size.isEmpty()){
        rect = layout_->geometry();
    }else{
        rect.setX(0);
        rect.setY(0);
        rect.setWidth(size.width());
        rect.setHeight(size.height());
    }
    auto browser = browser_window_->GetBrowser();
    if(browser){
        auto windowHandle = browser->GetHost()->GetWindowHandle();
#if defined(OS_WIN)
//        auto hdwp = BeginDeferWindowPos(1);
//        hdwp = DeferWindowPos(hdwp, windowHandle, HWND_BOTTOM, rect.x(), rect.y(),
//                              rect.width(), rect.height(), SWP_SHOWWINDOW); // SWP_NOZORDER SWP_SHOWWINDOW
//        EndDeferWindowPos(hdwp);
        ::MoveWindow(windowHandle, rect.x(), rect.y(), rect.width(), rect.height(), false);
        ::SetWindowPos(windowHandle, HWND_BOTTOM,rect.x(), rect.y(), rect.width(), rect.height(), SWP_NOACTIVATE);
#elif defined(OS_LINUX)
        ::Window xwindow = windowHandle;
        SetXWindowBounds(xwindow, 0, 0,
                         static_cast<size_t>(rect.width()),
                         static_cast<size_t>(rect.height()));
#else
#endif
    }
}
