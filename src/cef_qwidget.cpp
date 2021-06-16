#include "cef_qwidget.h"

#ifdef Q_OS_WIN
#include <Windows.h>
#include <SHlObj.h>
#include <WinUser.h>
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
#include <QPushButton>
#include <QDateTime>
#include <QPixmap>
#include <QBuffer>

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


CefQWidget::CefQWidget(const QString &url, QWidget *parent)
    : QWidget(parent)
    , window_(new QWindow)
    , qwindow_containter_(nullptr)
    , layout_(new QVBoxLayout(this))
{
    newly_created_ = true;
    browser_window_.reset(new BrowserWindow(this, url.toStdString()));
    initUi();
}

CefQWidget::CefQWidget(CefWindowInfo &windowInfo,
                       CefRefPtr<CefClient> &client,
                       CefBrowserSettings &settings,
                       QWidget *parent)
    : QWidget(parent)
    , window_(new QWindow)
    , qwindow_containter_(nullptr)
    , layout_(new QVBoxLayout(this))
{
    newly_created_ = false;
    browser_window_.reset(new BrowserWindow(this, ""));
    initUi();

    auto handle = (HWND)window_->winId();
    browser_window_->GetPopupConfig(handle, windowInfo, client, settings);
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

void CefQWidget::onTopLevelWindowStateChanged(Qt::WindowStates state, const QVariant &data)
{
    resizeBorser(data.toSize());
//    if(state.testFlag(Qt::WindowMaximized) || state.testFlag(Qt::WindowNoState)){
//    }
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
    resizeBorser();
}

void CefQWidget::OnBrowserWindowClosing()
{
    emit browserClosing();
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
        FaviconMgr::Instance().addIconRecord(url_, file_path);
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
        FaviconMgr::Instance().addIconRecord(url_, file_path);
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
    if(!is_dev_tool_){
        History data{QString::number(QDateTime::currentSecsSinceEpoch()),
                    url_,
                    title_,
                    1};
        HistoryMgr::Instance().addHistoryRecord(data);
    }

    emit browserLoadEnd(httpStatusCode);
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
    if(!is_dev_tool_){
        dealCefKeyEvent(event, os_event, is_keyboard_shortcut);
    }
    return false;
}

bool CefQWidget::onBrowserWndKeyEvent(const CefKeyEvent &event,
                                      CefEventHandle os_event)
{
    if(is_dev_tool_){
        emit devToolShortcut(event, os_event);
    }else{
        // return true represent you deal the event, otherwise return false
        dealCefKeyEvent(event, os_event, nullptr, false);
    }

    return false;
}

void CefQWidget::dealCefKeyEvent(const CefKeyEvent &event,
                                 CefEventHandle os_event,
                                 bool *is_keyboard_shortcut,
                                 bool isPre)
{
    bool is_shortcut_and_need_to_be_done = false;
    // F5
    if (event.modifiers == EVENTFLAG_NONE
            && event.windows_key_code == VK_F5
            && event.type == KEYEVENT_RAWKEYDOWN)
    {
        is_shortcut_and_need_to_be_done = true;
    }
    // F11
    if (event.modifiers == EVENTFLAG_NONE
            && event.windows_key_code == VK_F11
            && event.type == KEYEVENT_RAWKEYDOWN)
    {
        is_shortcut_and_need_to_be_done = true;
    }
    // F12
    if (event.modifiers == EVENTFLAG_NONE
            && event.windows_key_code == VK_F12
            && event.type == KEYEVENT_RAWKEYDOWN)
    {
        is_shortcut_and_need_to_be_done = true;
    }
    // Ctrl + -(- 位于 主键盘 0 右侧)
    if (event.modifiers == EVENTFLAG_CONTROL_DOWN
            && event.windows_key_code == VK_OEM_MINUS
            && event.type == KEYEVENT_RAWKEYDOWN)
    {
        is_shortcut_and_need_to_be_done = true;
    }
    // Ctrl + -(- 位于 小键盘 )
    if (event.modifiers == (EVENTFLAG_CONTROL_DOWN | EVENTFLAG_IS_KEY_PAD)
            && event.windows_key_code == VK_SUBTRACT
            && event.type == KEYEVENT_RAWKEYDOWN)
    {
        is_shortcut_and_need_to_be_done = true;
    }
    // Ctrl + 0(0 位于 主键盘)
    if (event.modifiers == EVENTFLAG_CONTROL_DOWN
            && event.windows_key_code == '0'
            && event.type == KEYEVENT_RAWKEYDOWN)
    {
        is_shortcut_and_need_to_be_done = true;
    }
    // Ctrl + 0(0 位于 小键盘 )
    if (event.modifiers == (EVENTFLAG_CONTROL_DOWN | EVENTFLAG_IS_KEY_PAD)
            && event.windows_key_code == VK_NUMPAD0
            && event.type == KEYEVENT_RAWKEYDOWN)
    {
        is_shortcut_and_need_to_be_done = true;
    }
    // Ctrl + +(+ 位于 backspace 左侧)
    if (event.modifiers == EVENTFLAG_CONTROL_DOWN
            && event.windows_key_code == VK_OEM_PLUS
            && event.type == KEYEVENT_RAWKEYDOWN)
    {
        is_shortcut_and_need_to_be_done = true;
    }
    // Ctrl + +(+ 位于 小键盘 )
    if (event.modifiers == (EVENTFLAG_CONTROL_DOWN | EVENTFLAG_IS_KEY_PAD)
            && event.windows_key_code == VK_ADD
            && event.type == KEYEVENT_RAWKEYDOWN)
    {
        is_shortcut_and_need_to_be_done = true;
    }
    // Ctrl + P
    if (event.modifiers == EVENTFLAG_CONTROL_DOWN
            && event.windows_key_code == 'P'
            && event.type == KEYEVENT_RAWKEYDOWN)
    {
        is_shortcut_and_need_to_be_done = true;
    }
    // Ctrl + R
    if (event.modifiers == EVENTFLAG_CONTROL_DOWN
            && event.windows_key_code == 'R'
            && event.type == KEYEVENT_RAWKEYDOWN)
    {
        is_shortcut_and_need_to_be_done = true;
    }
    // Ctrl + T
    if (event.modifiers == EVENTFLAG_CONTROL_DOWN
            && event.windows_key_code == 'T'
            && event.type == KEYEVENT_RAWKEYDOWN)
    {
        is_shortcut_and_need_to_be_done = true;
    }
    // Ctrl + N
    if (event.modifiers == EVENTFLAG_CONTROL_DOWN
            && event.windows_key_code == 'N'
            && event.type == KEYEVENT_RAWKEYDOWN)
    {
        is_shortcut_and_need_to_be_done = true;
    }
    // Ctrl + Shift + N
    if (event.modifiers == (EVENTFLAG_CONTROL_DOWN | EVENTFLAG_SHIFT_DOWN)
            && event.windows_key_code == 'N'
            && event.type == KEYEVENT_RAWKEYDOWN)
    {
        is_shortcut_and_need_to_be_done = true;
    }
    // Ctrl + Shift + I
    if (event.modifiers == (EVENTFLAG_CONTROL_DOWN | EVENTFLAG_SHIFT_DOWN)
            && event.windows_key_code == 'I'
            && event.type == KEYEVENT_RAWKEYDOWN)
    {
        is_shortcut_and_need_to_be_done = true;
    }
    // Ctrl + H
    if (event.modifiers == EVENTFLAG_CONTROL_DOWN
            && event.windows_key_code == 'H'
            && event.type == KEYEVENT_RAWKEYDOWN)
    {
        is_shortcut_and_need_to_be_done = true;
    }
    // Ctrl + J
    if (event.modifiers == EVENTFLAG_CONTROL_DOWN
            && event.windows_key_code == 'J'
            && event.type == KEYEVENT_RAWKEYDOWN)
    {
        is_shortcut_and_need_to_be_done = true;
    }
    // Ctrl + W
    if (event.modifiers == EVENTFLAG_CONTROL_DOWN
            && event.windows_key_code == 'W'
            && event.type == KEYEVENT_RAWKEYDOWN)
    {
        is_shortcut_and_need_to_be_done = true;
    }
    // Ctrl + Tab
    if(event.modifiers == EVENTFLAG_CONTROL_DOWN
            && event.windows_key_code == VK_TAB
            && event.type == KEYEVENT_RAWKEYDOWN)
    {
        is_shortcut_and_need_to_be_done = true;
    }
    // Alt + <--(左箭头)
    if(event.modifiers == EVENTFLAG_ALT_DOWN
            && event.windows_key_code == VK_LEFT
            && event.type == KEYEVENT_RAWKEYDOWN)
    {
        is_shortcut_and_need_to_be_done = true;
    }
    // Alt + -->(右箭头)
    if(event.modifiers == EVENTFLAG_ALT_DOWN
            && event.windows_key_code == VK_RIGHT
            && event.type == KEYEVENT_RAWKEYDOWN)
    {
        is_shortcut_and_need_to_be_done = true;
    }


    // 统一处理，不写多份儿
    if(is_shortcut_and_need_to_be_done)
    {
        if(isPre && is_keyboard_shortcut){
            *is_keyboard_shortcut = true;
        }else{
            emit browserShortcut(event, os_event);
        }
    }
}

void CefQWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    switch(browser_state_){
    case Empty:
    {
        auto handle = (HWND)window_->winId();
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
        resizeBorser();
        break;
    }

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
    window_->setFlag(Qt::FramelessWindowHint, true);

    if(!qwindow_containter_){
        qwindow_containter_ = QWidget::createWindowContainer(window_, this, Qt::Widget);
    }

    layout_->setContentsMargins(0,0,0,0);
    layout_->setSpacing(0);

#if 0
    QPushButton *button = new QPushButton("zoomIn",this);
    connect(button, &QPushButton::clicked, this, [this]()
    {
        browser_->GetHost()->SetZoomLevel(-3.0);
    });
    layout_->addWidget(button);
#endif

    layout_->addWidget(qwindow_containter_);
    setLayout(layout_);
}

void CefQWidget::resizeBorser(const QSize &size)
{
    QRect rect;
    if(size.isEmpty()){
        rect = qwindow_containter_->rect();
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
#elif defined(OS_LINUX)
        // todo:
#else
        ::Window xwindow = windowHandle;
        SetXWindowBounds(xwindow, 0, 0,
                         static_cast<size_t>(size.width()),
                         static_cast<size_t>(size.height()));
#endif
    }
}
