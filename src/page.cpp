#include "page.h"
#include "popups/SiteInfoPopup.h"
#include "popups/ZoomPopup.h"

#include <QtDebug>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDockWidget>
#include <QFrame>
#include <QVariant>
#include <QUrl>
#include <QStyle>
#include <QTimer>

#include "managers/AppCfgManager.h"
#include "managers/CefManager.h"

Page::Page(const QString &url, QWidget *parent)
    : QMainWindow(parent)
    , main_layout_(new QVBoxLayout)
    , dock_dev_tool_(new QDockWidget)
    , site_info_popup_(new SiteInfoPopup(this))
    , zoom_popup_(new ZoomPopup(this))
    , zoom_bar_timer_(new QTimer(this))
{
    setMinimumHeight(100);
    if(!centralWidget()){
        setCentralWidget(new QWidget);
    }
    browser_widget_ = new CefQWidget(url, this);

    main_layout_->addWidget(browser_widget_);
    centralWidget()->setLayout(main_layout_);

    main_layout_->setContentsMargins(0,0,0,0);
    main_layout_->setSpacing(0);

    initBrowser();
    initOthers();
}

Page::Page(CefQWidget*browser, QWidget *parent)
    : QMainWindow(parent)
    , main_layout_(new QVBoxLayout)
    , browser_widget_(browser)
    , dock_dev_tool_(new QDockWidget)
    , site_info_popup_(new SiteInfoPopup(this))
    , zoom_popup_(new ZoomPopup(this))
    , zoom_bar_timer_(new QTimer(this))
{
    setMinimumHeight(100);
    if(!centralWidget()){
        setCentralWidget(new QWidget);
    }
    main_layout_->addWidget(browser_widget_);
    centralWidget()->setLayout(main_layout_);

    main_layout_->setContentsMargins(0,0,0,0);
    main_layout_->setSpacing(0);

    initBrowser();
    initOthers();
}

Page::~Page()
{

}

bool Page::eventFilter(QObject *obj, QEvent *ev)
{
    if(obj == dock_dev_tool_){
        if(ev->type() == QEvent::Close){
            auto window = dock_dev_tool_->widget();
            dock_dev_tool_->setWidget(nullptr);

            if(window){
                window->deleteLater();
                window = nullptr;
            }
            auto geo = dock_dev_tool_->saveGeometry();
            AppCfgMgr::setDevToolGeometry(geo);
            ev->accept();
        }
    }
    return QMainWindow::eventFilter(obj, ev);
}

CefQWidget* Page::getBrowserWidget()
{
    return browser_widget_;
}

QString Page::url() const
{
    return edited_flag_ ? edited_txt_ : url_;
}

void Page::setEditedText(const QString &txt)
{
    if(url_.compare(txt, Qt::CaseInsensitive) == 0 || txt.isEmpty()){
        edited_flag_ = false;
        return;
    }
    edited_flag_ = true;
    edited_txt_ = txt;
}

void Page::showSiteInfomation(const QPoint &pos)
{
    /*设置必要的信息*/
    site_info_popup_->setDomain(QUrl(url_).host());
    site_info_popup_->setLevel(SecurityLevel::Https);
    site_info_popup_->move(pos);
    site_info_popup_->show();
}

void Page::showZoomBar(const QPoint &pos)
{
    auto pos1 = pos;
    pos1.rx() -= zoom_popup_->width();
    zoom_popup_->move(pos1);
    auto zoomLevel = browser_widget_->ZoomLevel();
    zoom_popup_->setZoomLevelStr(CefManager::Instance().zoom_map.value(static_cast<int>(zoomLevel)));
    zoom_popup_->show();
}

void Page::openDevTool()
{
    auto cef_qwidget = qobject_cast<CefQWidget *>(dock_dev_tool_->widget());
    if(cef_qwidget && cef_qwidget->isDevTool())
    {
        dock_dev_tool_->close();
    }else{
        browser_widget_->ShowDevTool(QPoint());
    }
}

void Page::closeEvent(QCloseEvent *event)
{
    browser_widget_->close();
    QMainWindow::closeEvent(event);
}

void Page::initBrowser()
{
    connect(browser_widget_, &CefQWidget::browserNeedSize, [this]()
    {
        emit pageCmd(PageCmd::NeedSize, QVariant());
    });
    connect(browser_widget_, &CefQWidget::browserCreated, [this]()
    {
        emit pageCmd(PageCmd::Created, QVariant());
    });
    connect(browser_widget_, &CefQWidget::browserClosing, [this]()
    {
        emit pageCmd(PageCmd::Closing, QVariant());
    });
    connect(browser_widget_, &CefQWidget::browserNewForgroundPage, [this](CefQWidget *browser){
        Page *page = new Page(browser);
        emit newPage(page);
    });
    connect(browser_widget_, &CefQWidget::browserDevTool, this, &Page::onBrowserDevTool);

    connect(browser_widget_, &CefQWidget::browserAddressChange, [this](const QString &url)
    {
        setEditedText("");
        url_ = url;
        emit pageCmd(PageCmd::Address, url);
    });
    connect(browser_widget_, &CefQWidget::browserTitleChange, [this](const QString &title)
    {
        title_ = title;
        emit pageCmd(PageCmd::Title, title);
    });
    connect(browser_widget_, &CefQWidget::browserFullScnChange, [this](bool fullscreen)
    {
        emit pageCmd(PageCmd::FullScreen, fullscreen);
    });
    connect(browser_widget_, &CefQWidget::browserStatusMessage, [this](const QString &msg)
    {
        emit pageCmd(PageCmd::StatusMessage, msg);
    });
    connect(browser_widget_, &CefQWidget::browserLoadStart, [this](CefLoadHandler::TransitionType transition_type)
    {
        setMinimumSize(QSize(0,0));
        emit pageCmd(PageCmd::LoadStart, (int)transition_type);
    });
    connect(browser_widget_, &CefQWidget::browserLoadEnd, [this](int httpStatusCode)
    {
//        const QStringList customSchemes{QString("about"),"chrome"};
//        auto parts = url_.split(":");
//        if(parts.count()  > 0){
//            auto scheme = parts.at(0).toLower();
//            if(customSchemes.contains(scheme)){
//                emit pageCmd(PageCmd::Favicon, style()->standardPixmap(QStyle::SP_MessageBoxInformation));
//            }
//        }

//        if(url_.startsWith("file://",Qt::CaseInsensitive)){
//            emit pageCmd(PageCmd::Favicon, style()->standardPixmap(QStyle::SP_FileIcon));
//        }
        setMinimumSize(QSize(0,0));
        emit pageCmd(PageCmd::LoadEnd, httpStatusCode);
    });
    connect(browser_widget_, &CefQWidget::browserLoadingProgress, [this](double progress)
    {
        emit pageCmd(PageCmd::LoadingProgress, progress);
    });
    connect(browser_widget_, &CefQWidget::browserLoadingStateChange, [this](bool a, bool b, bool c)
    {
        setMinimumSize(QSize(0,0));

        isLoading_ = a;
        canGoBack_ = b;
        canGoForward_ = c;
        emit pageCmd(PageCmd::LoadingState, QVariant());
    });
    connect(browser_widget_, &CefQWidget::browserFocusChange, [this](bool focus)
    {
        emit pageCmd(PageCmd::FocusChange, focus);
    });
    connect(browser_widget_, &CefQWidget::browserFaviconChange, [this](const QPixmap &pix)
    {
        favicon_ = pix;
        emit pageCmd(PageCmd::Favicon, pix);
    });

    connect(browser_widget_, &CefQWidget::browserShortcut,
            this, &Page::browserShortcut);
}

void Page::initOthers()
{
    dock_dev_tool_->installEventFilter(this);
    connect(dock_dev_tool_, &QDockWidget::topLevelChanged,
            this, &Page::onDockDevToolTopLevelChanged);
    connect(dock_dev_tool_, &QDockWidget::dockLocationChanged, this, &Page::onDockDevToolLocChanged);

    connect(zoom_popup_, &ZoomPopup::zoomOut, this, [this](){
        emit pageCmd(PageCmd::ZoomOut, "");
    });
    connect(zoom_popup_, &ZoomPopup::zoomIn, this, [this](){
        emit pageCmd(PageCmd::ZoomIn, "");
    });
    connect(zoom_popup_, &ZoomPopup::zoomReset, this, [this](){
        emit pageCmd(PageCmd::ZoomReset, "");
    });


    connect(site_info_popup_, &SiteInfoPopup::openUrl, this,[this](const QUrl &url)
    {
        emit pageCmd(PageCmd::OpenUrl, url);
    });

    connect(zoom_bar_timer_, &QTimer::timeout,this, &Page::onZoomBarTimer);
    zoom_bar_timer_->start(2000);
}

void Page::onBrowserDevTool(CefQWidget *devTool)
{
    qInfo()<<__FUNCTION__<<devTool;
    connect(devTool, &CefQWidget::devToolShortcut, this, &Page::onDevToolShortcut);

    dock_dev_tool_->setAllowedAreas(Qt::AllDockWidgetAreas);
    dock_dev_tool_->setFeatures(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetFloatable);
    dock_dev_tool_->setWidget(devTool);
    dock_dev_tool_->setWindowTitle(tr("Developer Tools - ") + QUrl(url_).toDisplayString());

    addDockWidget(Qt::BottomDockWidgetArea, dock_dev_tool_);
    dock_dev_tool_->show();
}

void Page::onDockDevToolTopLevelChanged(bool isFloating)
{
    if(!isFloating){
        return ;
    }
    auto geo = AppCfgMgr::devToolGeometry();
    if(!geo.isEmpty()){
        static QWidget w;   // 这就很像工具人
        w.hide();
        if(w.restoreGeometry(geo))
        {
            auto size = w.size();
            dock_dev_tool_->resize(size);
        }else{
           dock_dev_tool_->resize(500,300);
        }
    }
}

void Page::onDockDevToolLocChanged(Qt::DockWidgetArea area)
{
    switch (area) {
    case Qt::LeftDockWidgetArea:
        break;
    case Qt::TopDockWidgetArea:
        break;
    case Qt::RightDockWidgetArea:
        break;
    case Qt::BottomDockWidgetArea:
        break;
    case Qt::NoDockWidgetArea:
        break;
    default:
        break;
    }
}

void Page::onDevToolShortcut(const CefKeyEvent &event, CefEventHandle)
{
    qInfo()<<__FUNCTION__;
    auto sender = QObject::sender();
    auto devTool = qobject_cast<CefQWidget*>(sender);
    // 开发者工具 browser中的快捷键处理，这里按下了 F12，表明关闭开发者工具
    if(event.modifiers == EVENTFLAG_NONE
            && event.windows_key_code == VK_F12
            && event.type == KEYEVENT_RAWKEYDOWN)
    {
        if(devTool && devTool == dock_dev_tool_->widget())
        {
            dock_dev_tool_->close();
        }
    }

    // Ctrl + -(- 位于 主键盘 按键 0 右侧)
    // 缩小
    if(event.modifiers == EVENTFLAG_CONTROL_DOWN
            && event.windows_key_code == VK_OEM_MINUS)
    {
        if(devTool && devTool == dock_dev_tool_->widget())
        {
            devTool->ZoomOut();
        }
    }

    // Ctrl + -(- 位于 小键盘 )
    // 缩小
    if(event.modifiers == (EVENTFLAG_CONTROL_DOWN | EVENTFLAG_IS_KEY_PAD)
            && event.windows_key_code == VK_SUBTRACT ){
        if(devTool && devTool == dock_dev_tool_->widget())
        {
            devTool->ZoomOut();
        }
    }
    /* from WinUser.h
     * VK_0 - VK_9 are the same as ASCII '0' - '9' (0x30 - 0x39)
     * 0x3A - 0x40 : unassigned
     * VK_A - VK_Z are the same as ASCII 'A' - 'Z' (0x41 - 0x5A)
     */
    // Ctrl + 0(0 位于 主键盘)
    // 恢复缩放比例
    if(event.modifiers == EVENTFLAG_CONTROL_DOWN
            && event.windows_key_code == '0'){
        if(devTool && devTool == dock_dev_tool_->widget())
        {
            devTool->ZoomReset();
        }
    }
    // Ctrl + 0(0 位于 小键盘 )
    // 恢复缩放比例
    if(event.modifiers == (EVENTFLAG_CONTROL_DOWN | EVENTFLAG_IS_KEY_PAD)
            && event.windows_key_code == VK_NUMPAD0)
    {
        if(devTool && devTool == dock_dev_tool_->widget())
        {
            devTool->ZoomReset();
        }
    }

    // Ctrl + +(+ 位于 backspace 左侧)
    // 放大
    if( event.modifiers == EVENTFLAG_CONTROL_DOWN
            && event.windows_key_code == VK_OEM_PLUS)
    {
        if(devTool && devTool == dock_dev_tool_->widget())
        {
            devTool->ZoomIn();
        }
    }
    // Ctrl + +(+ 位于 小键盘 )
    // 放大
    if(event.modifiers == (EVENTFLAG_CONTROL_DOWN | EVENTFLAG_IS_KEY_PAD)
            && event.windows_key_code == VK_ADD){
        if(devTool && devTool == dock_dev_tool_->widget())
        {
            devTool->ZoomIn();
        }
    }
}

void Page::onZoomBarTimer()
{
    auto zoomLevel = browser_widget_->ZoomLevel();
    if(zoomLevel == 0.0){
        if( !zoom_popup_->rect().contains( zoom_popup_->mapFromGlobal(QCursor::pos()) ) )
        {
            zoom_popup_->hide();
        }
    }
}
