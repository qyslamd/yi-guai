#include "browser_page.h"
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
#include <QStackedLayout>
#include <QToolBar>
#include <QToolButton>

#include "managers/AppCfgManager.h"
#include "browser/CefManager.h"
#include "dialogs/pagedialog.h"
#include "toolbars/FindBar.h"

#ifndef Q_OS_WIN
#include "utils/windowskeyboardcodes.h"
#endif

BrowserPage::BrowserPage(const QString &url, QWidget *parent)
    : QMainWindow(parent)
    , main_layout_(new QVBoxLayout)
    , dock_dev_tool_(new QDockWidget)
    , site_info_popup_(new SiteInfoPopup(this))
    , zoom_popup_(new ZoomPopup(this))
    , zoom_bar_timer_(new QTimer(this))
    , find_bar_(new FindBar(this))
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

BrowserPage::BrowserPage(CefQWidget*browser, QWidget *parent)
    : QMainWindow(parent)
    , main_layout_(new QVBoxLayout)
    , browser_widget_(browser)
    , dock_dev_tool_(new QDockWidget)
    , site_info_popup_(new SiteInfoPopup(this))
    , zoom_popup_(new ZoomPopup(this))
    , zoom_bar_timer_(new QTimer(this))
    , find_bar_(new FindBar(this))
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

BrowserPage::~BrowserPage()
{

}

bool BrowserPage::eventFilter(QObject *obj, QEvent *ev)
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

CefQWidget* BrowserPage::getBrowserWidget()
{
    return browser_widget_;
}

QString BrowserPage::url() const
{
    return edited_flag_ ? edited_txt_ : url_;
}

void BrowserPage::setEditedText(const QString &txt)
{
    if(url_.compare(txt, Qt::CaseInsensitive) == 0 || txt.isEmpty()){
        edited_flag_ = false;
        return;
    }
    edited_flag_ = true;
    edited_txt_ = txt;
}

void BrowserPage::showSiteInfomation(const QPoint &pos)
{
    /*设置必要的信息*/
    site_info_popup_->setDomain(QUrl(url_).host());
    site_info_popup_->setLevel(SecurityLevel::Https);
    site_info_popup_->move(pos);
    site_info_popup_->show();
}

void BrowserPage::showZoomBar(const QPoint &pos)
{
    auto pos1 = pos;
    pos1.rx() -= zoom_popup_->width();
    zoom_popup_->move(pos1);
    auto zoomLevel = browser_widget_->ZoomLevel();
    zoom_popup_->setZoomLevelStr(CefManager::Instance().zoom_map.value(static_cast<int>(zoomLevel)));
    zoom_popup_->show();
}

void BrowserPage::openDevTool()
{
    auto cef_qwidget = qobject_cast<CefQWidget *>(dock_dev_tool_->widget());
    if(cef_qwidget && cef_qwidget->isDevTool())
    {
        dock_dev_tool_->close();
    }else{
        browser_widget_->ShowDevTool(QPoint());
    }
}

void BrowserPage::closeEvent(QCloseEvent *event)
{
    browser_widget_->close();
    QMainWindow::closeEvent(event);
}

void BrowserPage::initBrowser()
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
        BrowserPage *page = new BrowserPage(browser);
        emit newPage(page);
    });
    connect(browser_widget_, &CefQWidget::browserDevTool, this, &BrowserPage::onBrowserDevTool);

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

    connect(browser_widget_, &CefQWidget::browserShortcut, this, [=](CefShortcutCmd cmd){
        if(!browser_widget_->isDevTool()){
            emit browserShortcut(cmd);
        }
    });
}

void BrowserPage::initOthers()
{
    dock_dev_tool_->installEventFilter(this);
    connect(dock_dev_tool_, &QDockWidget::topLevelChanged,
            this, &BrowserPage::onDockDevToolTopLevelChanged);
    connect(dock_dev_tool_, &QDockWidget::dockLocationChanged, this, &BrowserPage::onDockDevToolLocChanged);

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

    connect(zoom_bar_timer_, &QTimer::timeout,this, &BrowserPage::onZoomBarTimer);
    zoom_bar_timer_->start(2000);

    find_bar_->hide();
}

void BrowserPage::moveFindBar()
{

}

void BrowserPage::onBrowserDevTool(CefQWidget *devTool)
{
    qInfo()<<__FUNCTION__<<devTool;
    connect(devTool, &CefQWidget::browserShortcut, this, [=](CefShortcutCmd cmd){
        onDevToolShortcut(devTool, cmd);
    });
    dock_dev_tool_->setAllowedAreas(Qt::AllDockWidgetAreas);
    dock_dev_tool_->setFeatures(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetFloatable);
    dock_dev_tool_->setWidget(devTool);
    dock_dev_tool_->setWindowTitle(tr("Developer Tools - ") + QUrl(url_).toDisplayString());

    addDockWidget(Qt::BottomDockWidgetArea, dock_dev_tool_);
    dock_dev_tool_->show();
}

void BrowserPage::onDockDevToolTopLevelChanged(bool isFloating)
{
    if(!isFloating){
        return ;
    }
    auto geo = AppCfgMgr::devToolGeometry();
    if(!geo.isEmpty()){
        /* tool widget, for using restoreGeometry function */
        static QWidget w;
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

void BrowserPage::onDockDevToolLocChanged(Qt::DockWidgetArea area)
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

void BrowserPage::onDevToolShortcut(CefQWidget *devTool, CefShortcutCmd cmd)
{
    qInfo()<<__FUNCTION__;
    switch (cmd){
    case CefShortcutCmd::DevTool:
        // 开发者工具 browser中的快捷键处理，这里按下了 F12，表明关闭开发者工具
        if(devTool && devTool == dock_dev_tool_->widget())
        {
            dock_dev_tool_->close();
        }
        break;
    case CefShortcutCmd::ZoomOut:
        if(devTool && devTool == dock_dev_tool_->widget())
        {
            devTool->ZoomOut();
        }
        break;
    case CefShortcutCmd::ZoomReset:
        if(devTool && devTool == dock_dev_tool_->widget())
        {
            devTool->ZoomReset();
        }
        break;
    case CefShortcutCmd::ZoomIn:
        if(devTool && devTool == dock_dev_tool_->widget())
        {
            devTool->ZoomIn();
        }
        break;
    default:
        break;
    }
}

void BrowserPage::onZoomBarTimer()
{
    auto zoomLevel = browser_widget_->ZoomLevel();
    if(zoomLevel == 0.0){
        if( !zoom_popup_->rect().contains( zoom_popup_->mapFromGlobal(QCursor::pos()) ) )
        {
            zoom_popup_->hide();
        }
    }
}
