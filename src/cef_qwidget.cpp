#include "cef_qwidget.h"

#include <QtGlobal>
#include <QApplication>
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
#include <QPainter>
#include <QPainterPath>

#ifdef Q_OS_WIN
#include <Windows.h>
#include <SHlObj.h>
#include <WinUser.h>
#else
#include "utils/windowskeyboardcodes.h"
#include "browser/client_types.h"
#endif

#include <include/base/cef_logging.h>
#include <include/wrapper/cef_helpers.h>

#include "popups/StyledMenu.h"
#include "mainwindow.h"
#include "popup_browser.h"
#include "browser_page.h"
#include "managers/FaviconManager.h"
#include "managers/MainWindowManager.h"
#include "managers/AppCfgManager.h"
#include "managers/HistoryManager.h"
#include "utils/util_qt.h"
#include "dialogs/alertdialog.h"
#include "browser/cef_client_handler.h"
#include "browser/client_types.h"
#include "browser/message_loop/main_message_loop.h"

#ifdef Q_OS_LINUX
#include <gdk/gdk.h>
#include <gdk/gdkx.h>
#include <X11/Xlib.h>
#include <gtk/gtk.h>
#include "utils/util_gtk.h"

#ifdef signals
#undef signals
#endif
#undef Success     // Definition conflicts with cef_message_router.h
#undef RootWindow  // Definition conflicts with root_window.h
#undef Bool // Definition conflicts with X11/Xlib.h

namespace {

::Window GetXWindowForWidget(GtkWidget* widget) {
  ScopedGdkThreadsEnter scoped_gdk_threads;

  // The GTK window must be visible before we can retrieve the XID.
  ::Window xwindow = GDK_WINDOW_XID(gtk_widget_get_window(widget));
  DCHECK(xwindow);
  return xwindow;
}

void SetXWindowVisible(XDisplay* xdisplay, ::Window xwindow, bool visible) {
  CHECK(xdisplay != 0);

  // Retrieve the atoms required by the below XChangeProperty call.
  const char* kAtoms[] = {"_NET_WM_STATE", "ATOM", "_NET_WM_STATE_HIDDEN"};
  Atom atoms[3];
  int result =
      XInternAtoms(xdisplay, const_cast<char**>(kAtoms), 3, false, atoms);
  if (!result)
    NOTREACHED();

  if (!visible) {
    // Set the hidden property state value.
    std::unique_ptr<Atom[]> data(new Atom[1]);
    data[0] = atoms[2];

    XChangeProperty(xdisplay, xwindow,
                    atoms[0],  // name
                    atoms[1],  // type
                    32,        // size in bits of items in 'value'
                    PropModeReplace,
                    reinterpret_cast<const unsigned char*>(data.get()),
                    1);  // num items
  } else {
    // Set an empty array of property state values.
    XChangeProperty(xdisplay, xwindow,
                    atoms[0],  // name
                    atoms[1],  // type
                    32,        // size in bits of items in 'value'
                    PropModeReplace, nullptr,
                    0);  // num items
  }
}

void SetXWindowBounds(XDisplay* xdisplay,
                      ::Window xwindow,
                      int x,
                      int y,
                      size_t width,
                      size_t height) {
    CHECK(xdisplay != 0);
    XWindowChanges changes = {0};
    changes.x = x;
    changes.y = y;
    changes.width = static_cast<int>(width);
    changes.height = static_cast<int>(height);
    XConfigureWindow(xdisplay, xwindow, CWX | CWY | CWHeight | CWWidth, &changes);
}


Display* X11GetDisplay(QWidget* widget)
{
  Q_ASSERT_X(widget, "X11GetDisplay", "Invalid parameter widget");
  if (!widget) {
    qWarning("Invalid parameter widget");
    return nullptr;
  }

  auto platformInterface = QApplication::platformNativeInterface();
  Q_ASSERT_X(platformInterface, "X11GetDisplay", "Failed to get platform native interface");
  if (!platformInterface) {
    qWarning("Failed to get platform native interface");
    return nullptr;
  }

  auto screen = widget->window()->windowHandle()->screen();
  Q_ASSERT_X(screen, "X11GetDisplay", "Failed to get screen");
  if (!screen) {
    qWarning("Failed to get screen");
    return nullptr;
  }

//  return (Display*)platformInterface->nativeResourceForScreen("display", screen);
  return (Display*)screen->handle();
  return nullptr;
}

}  // namespace

#endif  // OS_LINUX


CefQWidget::CefQWidget(const QString &url, QWidget *parent)
    : QWidget(parent)
    , url_(url)
{
    initUi();
#if defined (Q_OS_LINUX)
    CefBrowserSettings browser_settings;
    auto ret = CreateBrowser({x(), y(), window_->size().width(), window_->size().height()},
                  browser_settings,
                  nullptr,
                  nullptr);
    qInfo() << "Create browser:" << ret;
#endif
}

CefQWidget::CefQWidget(CefWindowInfo &windowInfo,
                       CefRefPtr<CefClient> &client,
                       CefBrowserSettings &settings,
                       QWidget *parent)
    : QWidget(parent)
{
    initUi();
    GetPopupConfig( windowInfo, client, settings);
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
    if(browser_){
        browser_->GetMainFrame()->LoadURL(url.toStdString());
    }
}

void CefQWidget::GoBack()
{
    if(browser_){
        if(browser_->CanGoBack()){
            browser_->GoBack();
        }
    }
}

void CefQWidget::GoForward()
{
    if(browser_){
        if(browser_->CanGoForward()){
            browser_->GoForward();
        }
    }
}

void CefQWidget::Refresh()
{
    if(browser_){
        browser_->Reload();
    }
}

void CefQWidget::MuteAudio()
{
    if(browser_){
        auto host = browser_->GetHost();
        bool ismuted = host->IsAudioMuted();
        browser_->GetHost()->SetAudioMuted(!ismuted);
    }
}

void CefQWidget::StopLoading()
{
    if(browser_){
        if(browser_->IsLoading()){
            browser_->StopLoad();
        }
    }
}

void CefQWidget::ZoomOut()
{
    if(browser_){
        auto old = browser_->GetHost()->GetZoomLevel();
        double delta = old <= -7.0 ? 0.0 : -1.0;
        browser_->GetHost()->SetZoomLevel(old + delta);
    }
}

void CefQWidget::ZoomIn()
{
    if(browser_){
        auto old = browser_->GetHost()->GetZoomLevel();
        double delta = old >= 9.0 ? 0.0 : 1.0;
        browser_->GetHost()->SetZoomLevel(old + delta);
    }
}

void CefQWidget::ZoomReset()
{
    if(browser_){
        browser_->GetHost()->SetZoomLevel(0.0);
    }
}

double CefQWidget::ZoomLevel()
{
    if(browser_){
        return browser_->GetHost()->GetZoomLevel();
    }
    return 0.0;
}

void CefQWidget::Print()
{
    if(browser_){
        browser_->GetHost()->Print();
    }
}

void CefQWidget::ShowDevTool(const QPoint &pos)
{
    client_handler_->ShowDevTools(browser_,
                                  CefPoint(pos.x(), pos.y()));
}

void CefQWidget::onBrowserBeforeContextMenu(CefRefPtr<CefBrowser> browser,
                                            CefRefPtr<CefFrame> frame,
                                            CefRefPtr<CefContextMenuParams> params,
                                            CefRefPtr<CefMenuModel> model)
{
    connect(action_back_, &QAction::triggered,[browser](){browser->GoBack();});
    connect(action_forward_, &QAction::triggered,[browser](){browser->CanGoForward();});
    connect(action_reload_, &QAction::triggered,[browser](){browser->Reload();});
    connect(action_reload_ignore_cache_, &QAction::triggered,[browser](){browser->ReloadIgnoreCache();});
//    connect(action_save_as_, &QAction::triggered,[browser](){browser->ReloadIgnoreCache();});
    connect(action_print_, &QAction::triggered,[browser](){browser->GetHost()->Print();});
//    connect(action_view_source_, &QAction::triggered,[browser](){browser->GetHost()->Print();});
    connect(action_inspect_element_, &QAction::triggered,[this](){ShowDevTool(QPoint());});
    connect(action_open_link_, &QAction::triggered,[=]()
    {
        auto link = params->GetLinkUrl();
        CefQWidget *window = new CefQWidget(QString::fromStdString(link));
        emit browserNewForgroundPage(window);
    });


    if ((params->GetTypeFlags() & (CM_TYPEFLAG_PAGE | CM_TYPEFLAG_FRAME)) != 0)
    {
        // 在 Page 上
        if ( params->GetTypeFlags() == CM_TYPEFLAG_PAGE )
        {
            /**************
             *   前进
             *   后退
             *   刷新
             *   忽略缓存刷新
             * -------------
             *   另存为
             *   打印
             * -------------
             *   查看网页源码
             *   检查
             * ************/
            browser_context_menu_->clear();
            browser_context_menu_->addAction(action_back_);
            browser_context_menu_->addAction(action_forward_);
            browser_context_menu_->addAction(action_reload_);
            browser_context_menu_->addAction(action_reload_ignore_cache_);
            browser_context_menu_->addSeparator();
            browser_context_menu_->addAction(action_save_as_);
            browser_context_menu_->addAction(action_print_);
            browser_context_menu_->addSeparator();
            browser_context_menu_->addAction(action_view_source_);
            browser_context_menu_->addAction(action_inspect_element_);

            action_back_->setEnabled(browser->CanGoBack());
            action_forward_->setEnabled(browser->CanGoForward());

            browser_context_menu_->exec(QCursor::pos());
        }

        // 在有子frame参与的page上
        if ( params->GetTypeFlags() == (CM_TYPEFLAG_PAGE | CM_TYPEFLAG_FRAME) )
        {
            browser_context_menu_->clear();
            browser_context_menu_->addAction(action_back_);
            browser_context_menu_->addAction(action_forward_);
            browser_context_menu_->addAction(action_reload_);
            browser_context_menu_->addAction(action_reload_ignore_cache_);
            browser_context_menu_->addSeparator();
            browser_context_menu_->addAction(action_save_as_);
            browser_context_menu_->addAction(action_print_);
            browser_context_menu_->addSeparator();
            browser_context_menu_->addAction(action_view_frame_source_);
            browser_context_menu_->addAction(action_frame_reload_);
            browser_context_menu_->addAction(action_inspect_element_);

            action_back_->setEnabled(browser->CanGoBack());
            action_forward_->setEnabled(browser->CanGoForward());

            browser_context_menu_->exec(QCursor::pos());
        }

        // 在某个链接地址上
        if( params->GetTypeFlags() == (CM_TYPEFLAG_PAGE | CM_TYPEFLAG_LINK) )
        {
            browser_context_menu_->clear();
            browser_context_menu_->addAction(action_open_link_);
            browser_context_menu_->addAction(action_open_link_window_);
            browser_context_menu_->addAction(action_open_link_incognito_);
            browser_context_menu_->addSeparator();
            browser_context_menu_->addAction(action_copy_link_);
            browser_context_menu_->addSeparator();
            browser_context_menu_->addAction(action_inspect_element_);

            browser_context_menu_->exec(QCursor::pos());
        }
        // 输入框
        if( params->GetTypeFlags() == (CM_TYPEFLAG_PAGE | CM_TYPEFLAG_EDITABLE) )
        {
            model->InsertItemAt(0, MENU_ID_USER_EMOJI, CefString(QObject::tr("emoji").toStdString()));
        }

        // 选中非链接地址的内容
        if( params->GetTypeFlags() == (CM_TYPEFLAG_PAGE | CM_TYPEFLAG_SELECTION) )
        {
            model->AddItem(MENU_ID_USER_SEARCH_SELECTED, CefString(QObject::tr("Searh in web").toStdString()));
            model->AddSeparator();
            model->AddItem(MENU_ID_USER_INSPECT_ELEMENT,
                            CefString(QObject::tr("Inspect element").toStdString()));
        }

        // 图片上
        if( params->GetTypeFlags() == (CM_TYPEFLAG_PAGE | CM_TYPEFLAG_MEDIA) )
        {
            if(model->Clear()){
                model->AddItem(MENU_ID_USER_OPEN_IMAGE_PAGE,
                               CefString(QObject::tr("Open image in new page").toStdString()));
                model->AddItem(MENU_ID_USER_SAVE_IMAGE_AS,
                               CefString(QObject::tr("Save image as").toStdString()));
                model->AddItem(MENU_ID_USER_COPY_IMAGE,
                               CefString(QObject::tr("Copy image").toStdString()));
                model->AddItem(MENU_ID_USER_COPY_IMAGE_LIINK,
                               CefString(QObject::tr("Copy image url").toStdString()));
                model->AddSeparator();
                model->AddItem(MENU_ID_USER_INSPECT_ELEMENT,
                                CefString(QObject::tr("Inspect element").toStdString()));
            }
        }
        // 既是图片又是链接又在Page上
        if( params->GetTypeFlags() == (CM_TYPEFLAG_PAGE | CM_TYPEFLAG_MEDIA | CM_TYPEFLAG_LINK) )
        {
            if(model->Clear()){
                model->AddItem(MENU_ID_USER_OPEN_LINK_PAGE, CefString(QObject::tr("Open the link in a new tab").toStdString()));
                model->AddItem(MENU_ID_USER_OPEN_LINK_WINDOW, CefString(QObject::tr("Open the link in a new window").toStdString()));
                model->AddItem(MENU_ID_USER_OPEN_LINK_INPRIVATE,
                               CefString(QObject::tr("Open the link in private window").toStdString()));
                model->AddSeparator();
                model->AddItem(MENU_ID_USER_COPY_LINK_URL, CefString(QObject::tr("Copy link address").toStdString()));
                model->AddSeparator();
                model->AddItem(MENU_ID_USER_OPEN_IMAGE_PAGE,
                               CefString(QObject::tr("Open image in new page").toStdString()));
                model->AddItem(MENU_ID_USER_SAVE_IMAGE_AS,
                               CefString(QObject::tr("Save image as").toStdString()));
                model->AddItem(MENU_ID_USER_COPY_IMAGE,
                               CefString(QObject::tr("Copy image").toStdString()));
                model->AddItem(MENU_ID_USER_COPY_IMAGE_LIINK,
                               CefString(QObject::tr("Copy image url").toStdString()));
                model->AddSeparator();
                model->AddItem(MENU_ID_USER_INSPECT_ELEMENT,
                                CefString(QObject::tr("Inspect element").toStdString()));
            }
        }
        // 在Page上选中内容，选中的内容是链接地址
        if( params->GetTypeFlags() == (CM_TYPEFLAG_PAGE | CM_TYPEFLAG_SELECTION | CM_TYPEFLAG_LINK) )
        {
            model->InsertItemAt(0, MENU_ID_USER_OPEN_LINK_PAGE, CefString(QObject::tr("Open the link in a new tab").toStdString()));
            model->InsertItemAt(1, MENU_ID_USER_OPEN_LINK_WINDOW, CefString(QObject::tr("Open the link in a new window").toStdString()));
            model->InsertItemAt(2, MENU_ID_USER_OPEN_LINK_INPRIVATE,
                           CefString(QObject::tr("Open the link in private window").toStdString()));
            model->InsertSeparatorAt(3);
            model->InsertItemAt(4, MENU_ID_USER_COPY_LINK_URL, CefString(QObject::tr("Copy link address").toStdString()));
            model->InsertSeparatorAt(5);
            // copy
            model->AddItem(MENU_ID_USER_SEARCH_SELECTED, CefString(QObject::tr("Searh in web").toStdString()));
            model->AddSeparator();
            model->AddItem(MENU_ID_USER_INSPECT_ELEMENT,
                            CefString(QObject::tr("Inspect element").toStdString()));
        }
        if( params->GetTypeFlags() == (CM_TYPEFLAG_PAGE | CM_TYPEFLAG_SELECTION | CM_TYPEFLAG_LINK | CM_TYPEFLAG_FRAME) )
        {

        }
        if( params->GetTypeFlags() == (CM_TYPEFLAG_PAGE | CM_TYPEFLAG_MEDIA | CM_TYPEFLAG_LINK | CM_TYPEFLAG_FRAME) )
        {

        }
        if( params->GetTypeFlags() == (CM_TYPEFLAG_PAGE | CM_TYPEFLAG_EDITABLE| CM_TYPEFLAG_SELECTION ) )
        {

        }
    }
}

void CefQWidget::onBrowserForgroundTab(CefWindowInfo &windowInfo,
                                       CefRefPtr<CefClient> &client,
                                       CefBrowserSettings &settings)
{
    CefQWidget *window = new CefQWidget(windowInfo, client, settings);

    emit browserNewForgroundPage(window);
}

void CefQWidget::onBrowserPopupWnd(const CefPopupFeatures &popupFeatures,
                                      CefWindowInfo &windowInfo,
                                      CefRefPtr<CefClient> &client,
                                      CefBrowserSettings &settings)
{
    CefQWidget *window = new CefQWidget(windowInfo, client, settings);

    PopupBrowser *popupBrowser = new PopupBrowser(window);
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
void CefQWidget::onBrowserDeveTools(CefWindowInfo &windowInfo,
                                    CefRefPtr<CefClient> &client,
                                    CefBrowserSettings &settings)

{
    CefQWidget *window = new CefQWidget(windowInfo, client, settings);
    window->is_dev_tool_ = true;
    emit browserDevTool(window);
}

void CefQWidget::OnBrowserCreated(CefRefPtr<CefBrowser> browser)
{
    qInfo() << __FUNCTION__;

    browser_ = browser;
    browser_state_ = Created;
    resizeBrowser();
    emit browserCreated();
}

void CefQWidget::OnBrowserClosing(CefRefPtr<CefBrowser> browser)
{
    REQUIRE_MAIN_THREAD();
    DCHECK_EQ(browser->GetIdentifier(), browser_->GetIdentifier());
    // 修改标志
    is_closing_ = true;

    emit browserClosing();
    close();
}

void CefQWidget::onBrowserAddressChange(const std::string &url)
{
    url_ = QString::fromStdString(url);
    emit browserAddressChange(QString::fromStdString(url));
}

void CefQWidget::onBrowserTitleChange(const std::string &title)
{
    title_ = QString::fromStdString(title);
    emit browserTitleChange(title_);
}

void CefQWidget::onBrowserFullscreenChange(bool fullscreen)
{
    emit browserFullScnChange(fullscreen);
}

void CefQWidget::onBrowserStatusMessage(const std::string &msg)
{
    emit browserStatusMessage(QString::fromStdString(msg));
}

void CefQWidget::onBrowserFaviconChange(CefRefPtr<CefImage> image,
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

void CefQWidget::onBrowerLoadStart(CefLoadHandler::TransitionType transition_type)
{
    emit browserLoadStart(transition_type);
}

void CefQWidget::onBrowerLoadEnd(int httpStatusCode)
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

void CefQWidget::onBrowserLoadingProgressChange(CefRefPtr<CefBrowser> browser,
                                                double progress)
{
    emit browserLoadingProgress(progress);
//    qInfo()<<__FUNCTION__<<progress;
}

void CefQWidget::onBrowserLoadingStateChange(bool isLoading,
                                                   bool canGoBack,
                                                   bool canGoForward)
{
    emit browserLoadingStateChange(isLoading,
                                   canGoBack,
                                   canGoForward);
}

void CefQWidget::onBrowserGotFocus(CefRefPtr<CefBrowser> browser)
{
    emit browserFocusChange(true);
}

bool CefQWidget::onBrowserPreKeyEvent(const CefKeyEvent &event,
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

bool CefQWidget::onBrowserKeyEvent(const CefKeyEvent &event,
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

void CefQWidget::resizeEvent(QResizeEvent *event)
{
#ifdef Q_OS_LINUX
    resizeBrowser(event->size());
#else
    switch(browser_state_){
    case Empty:
    {
        CefRect rect{x(), y(), event->size().width(), event->size().height()};
        CefBrowserSettings browser_settings;
        CreateBrowser(rect,
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
    if (!is_closing_) {
        qInfo()<<__FUNCTION__;
        if(browser_) {
            // Notify the browser window that we would like to close it. This
            // will result in a call to ClientHandler::DoClose() if the
            // JavaScript 'onbeforeunload' event handler allows it.
            browser_->GetHost()->CloseBrowser(false);
        }
        // Cancel the close.
        event->ignore();
    } else {
        if(!this->parentWidget()) {
            if (browser_) {
                qInfo() << __FUNCTION__<<"cef has CefBrowser ref";
            }
        }
    }
}

void CefQWidget::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
}

void CefQWidget::initUi()
{
    client_handler_ = new CefClientHandler(this, url_.toStdString());
    window_ = new QWindow(windowHandle());
    window_->setFlag(Qt::FramelessWindowHint);
    window_->setMinimumSize(QSize(500, 500));

    layout_ = new QVBoxLayout(this);
    layout_->setContentsMargins(0,0,0,0);
    layout_->setSpacing(0);
    qwindow_containter_ = QWidget::createWindowContainer(window_);
    layout_->addWidget(qwindow_containter_);
    setLayout(layout_);

//    initContextMenu();
}

void CefQWidget::initContextMenu()
{
    browser_context_menu_ = new StyledMenu(this);
    action_back_ = new QAction(QIcon(":/icons/resources/imgs/arrow_back_left2_64px.png"), tr("Back"));
    action_back_->setShortcut(QKeySequence(tr("Alt+Left")));

    action_forward_ = new QAction(QIcon(":/icons/resources/imgs/arrow_right2_64px.png"), tr("Forward"));
    action_forward_->setShortcut(QKeySequence(tr("Alt+Right")));

    action_reload_ = new QAction(QIcon(":/icons/resources/imgs/reload_64px.png"), tr("Reload"));
    action_reload_->setShortcut(QKeySequence(tr("Ctrl+R")));

    action_reload_ignore_cache_ = new QAction(QIcon(":/icons/resources/imgs/reload_64px.png"), tr("Reload ignore cache"));
//    action_reload_ignore_cache_->setShortcut(QKeySequence(tr("Ctrl+R")));

    action_frame_reload_ = new QAction(QIcon(":/icons/resources/imgs/reload_64px.png"), tr("Reload frame"));
//    action_reload_->setShortcut(QKeySequence(tr("Ctrl+R")));

    action_save_as_ = new QAction(QIcon(":/icons/resources/imgs/reload_64px.png"), tr("Save as"));
    action_save_as_->setShortcut(QKeySequence(tr("Ctrl+S")));

    action_print_ = new QAction(QIcon(":/icons/resources/imgs/printer_64px.png"), tr("Print"));
    action_print_->setShortcut(QKeySequence(tr("Ctrl+P")));

    action_view_source_ = new QAction(QIcon(""), tr("View source"));
    action_view_source_->setShortcut(QKeySequence(tr("Ctrl+U")));

    action_view_frame_source_ = new QAction(QIcon(""), tr("View frame source"));
//    action_view_frame_source_->setShortcut(QKeySequence(tr("Ctrl+U")));

    action_inspect_element_ = new QAction(QIcon(":/icons/resources/imgs/search2_64px.png"), tr("Inspect element"));
    action_inspect_element_->setShortcut(QKeySequence(tr("Ctrl+Shift+I")));

    action_open_link_ = new QAction(QIcon(""), tr("Open link in new tab page"));
//    action_open_link_->setShortcut(QKeySequence(tr("Ctrl+U")));

    action_open_link_window_ = new QAction(QIcon(""), tr("Open link in new window"));

    action_open_link_incognito_ = new QAction(QIcon(""), tr("Open link in incognito window"));

    action_copy_link_ = new QAction(QIcon(""), tr("Copy link address"));

}

bool CefQWidget::CreateBrowser(const CefRect &rect,
                               const CefBrowserSettings &settings,
                               CefRefPtr<CefDictionaryValue> extra_info,
                               CefRefPtr<CefRequestContext> request_context)
{
    REQUIRE_MAIN_THREAD();
    qInfo() << "CefQWidget::CreateBrowser";

    CefWindowInfo window_info;
#if defined Q_OS_WIN
    RECT wnd_rect = {rect.x, rect.y, rect.x + rect.width, rect.y + rect.height};
#else
    CefRect wnd_rect{rect.x, rect.y, rect.x + rect.width, rect.y + rect.height};
#endif
    window_info.SetAsChild((ClientWindowHandle)window_->winId(), wnd_rect);

    return CefBrowserHost::CreateBrowser(window_info, client_handler_,
                                         client_handler_->startup_url(), settings,
                                         extra_info, request_context);
}

void CefQWidget::GetPopupConfig(CefWindowInfo &windowInfo,
                                CefRefPtr<CefClient> &client,
                                CefBrowserSettings &settings)
{
    CEF_REQUIRE_UI_THREAD()
    client = client_handler_;

    auto screen = qApp->primaryScreen();
    auto size = screen->availableSize();

    // The window will be properly sized after the browser is created.
#if defined(OS_WIN)
    RECT rect{0,0,size.width(),size.height()};
    windowInfo.SetAsChild((ClientWindowHandle)window_->winId(), rect);
#elif defined(OS_LINUX)
    CefRect rect(0, 0, size.width(), size.height());
    windowInfo.SetAsChild((ClientWindowHandle)window_->winId(), rect);
#endif

    Q_UNUSED(settings)
}

void CefQWidget::resizeBrowser(const QSize &size)
{
    QRect rect;
    rect.setX(0);
    rect.setY(0);
    if(size.isEmpty()){
        rect.setWidth(qwindow_containter_->width());
        rect.setHeight(qwindow_containter_->height());
    }else{
        rect.setWidth(size.width());
        rect.setHeight(size.height());
    }
    if(browser_){
        auto windowHandle = browser_->GetHost()->GetWindowHandle();
#if defined(OS_WIN)
//        auto hdwp = BeginDeferWindowPos(1);
//        hdwp = DeferWindowPos(hdwp, windowHandle, HWND_BOTTOM, rect.x(), rect.y(),
//                              rect.width(), rect.height(), SWP_SHOWWINDOW); // SWP_NOZORDER SWP_SHOWWINDOW
//        EndDeferWindowPos(hdwp);
        ::MoveWindow(windowHandle, rect.x(), rect.y(), rect.width(), rect.height(), false);
        ::SetWindowPos(windowHandle, HWND_BOTTOM,rect.x(), rect.y(), rect.width(), rect.height(), SWP_NOACTIVATE);
#elif defined(OS_LINUX)
        ::Window xwindow = windowHandle;
        ::Display* xdisplay = cef_get_xdisplay();
        DCHECK(xwindow);
        DCHECK(xdisplay);

        SetXWindowBounds(xdisplay, xwindow,
                         0,
                         0,
                         static_cast<size_t>(rect.width()),
                         static_cast<size_t>(rect.height()));
        SetXWindowVisible(xdisplay, xwindow, true);
#else
        //TODO:
#endif
    }
}

EmbededWindow::EmbededWindow(QWindow *parent)
    : QWindow(parent)
{
    store_.reset(new QBackingStore(this));
}

bool EmbededWindow::event(QEvent *event)
{
    if (event->type() == QEvent::UpdateRequest) {
        renderNow();
        return true;
    }
    return QWindow::event(event);
}

void EmbededWindow::resizeEvent(QResizeEvent *event)
{
    store_->resize(event->size());
}

void EmbededWindow::exposeEvent(QExposeEvent *event)
{
    if (isExposed()) {
        renderNow();
    }
    QWindow::exposeEvent(event);
}

void EmbededWindow::renderNow()
{
    if(!isExposed()) {
        return;
    }

    QRect rect(0, 0, width(), height());

    store_->beginPaint(rect);
    auto device = store_->paintDevice();
    QPainter p(device);

    p.fillRect(rect, QColor(238,130,238, 200));
    p.end();
    store_->endPaint();
    store_->flush(rect);
}
