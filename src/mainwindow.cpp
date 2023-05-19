#include "mainwindow.h"

#include "browser_page.h"
#include "cef_qwidget.h"
#include "browser/cef_client_handler.h"
#include "utils/util_qt.h"
#include "utils/util_win.h"
#include "toolbars/TabPageBar.h"
#include "toolbars/NavigationBar.h"
#include "toolbars/BookmarkBar.h"
#include "toolbars/NotificationBar.h"

#include "widgets/TabThumbnailWidget.h"
#include "widgets/AppConfigWidget.h"
#include "widgets/FullscnHint.h"
#include "widgets/HistoryWidget.h"
#include "widgets/BookmarkWidget.h"
#include "widgets/DownloadWidget.h"

#include "managers/BookmarkManager.h"
#include "managers/AppCfgManager.h"
#include "browser/CefManager.h"

#include "popups/AddBookmarkPopup.h"
#include "popups/PopupGeneral.h"
#include "popups/InprivatePopup.h"
#include "popups/UserInfoPopup.h"

#include "dialogs/aboutdialog.h"

#include <QAction>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QStatusBar>
#include <QtDebug>
#include <QUrl>
#include <QCloseEvent>
#include <QTimer>
#include <QThread>
#include <QToolButton>
#include <QApplication>
#include <QFile>
#include <QDialog>
#include <QPropertyAnimation>
#include <QToolTip>
#include <QLabel>
#include <QMessageBox>
#include <QPainter>
#include <QStyle>
#include <QDateTime>
#include <QLocale>
#include <QStandardItemModel>
#include <QScreen>
#include <QDesktopWidget>

#if defined(Q_OS_WIN)
#include <Windows.h>
#include <QtWin>
#pragma comment(lib, "Gdi32.lib")
#else
#include "utils/windowskeyboardcodes.h"
#endif

MainWindow::MainWindow(const MainWindowConfig &cfg, QWidget *parent)
    : QtWinFramelessWindow(parent)
    , created_cfg_(cfg)
{
    initQtShortcut();
    initUi();
    setAppearance();
    initSignalSlot();
    auto url = cfg.url_;
    if(url.isEmpty()){
        url = AppCfgMgr::homePageUrl();
        if(url.isEmpty()){
            url = AppCfgMgr::gDefautlUrl;
        }
    }
    // 如果立即创建浏览器的话，此时窗口的大小是不确定的
    QTimer::singleShot(0, this, [=]{
        AddNewPage(url);
    });
}

MainWindow::MainWindow(BrowserPage *page, QWidget *parent)
    : QtWinFramelessWindow(parent)
{
    initQtShortcut();
    initUi();
    setAppearance();
    initSignalSlot();

    QTimer::singleShot(0, this, [=]{
        AddNewPage(page);
    });
}

MainWindow::~MainWindow()
{
    qInfo()<<__FUNCTION__;
}

int MainWindow::AddNewPage(const QString &url, bool switchTo)
{
    auto url1 = url;
    if(url1.isEmpty()){
        url1 = AppCfgMgr::gDefautlUrl;
    }
    BrowserPage *page = new BrowserPage(url1, this);
    // stackedWidget 有bug,page的大小很小，这里限定一下，等到加载结束的时候
    // 设置最小大小为默认值
    page->setMinimumSize(stack_browsers_->size());
    initPage(page);
    auto index = stack_browsers_->addWidget(page);
    tab_bar_->insertTab(index, url);

    if(switchTo){
        tab_bar_->setCurrentIndex(index);
    }
    return index;
}

int MainWindow::AddNewPage(BrowserPage *page)
{
    initPage(page);
    auto index = stack_browsers_->addWidget(page);
    tab_bar_->insertTab(index, "");
    tab_bar_->setCurrentIndex(index);
    return index;
}

void MainWindow::NavigateInCurPage(const QString &url)
{
    auto page = CurrentPage();
    auto url1 = UtilQt::check_url(url);
    url1 = url;
    if(page){
        page->getBrowserWidget()->Navigate(url1);
    }
}

void MainWindow::updatePreference()
{
    navi_bar_->updatePreference();
    bookmark_bar_->setVisible(AppCfgMgr::bookmarkBarVisible());
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    auto type = event->type();
    if(obj == bookmark_widget_){
        if(type == QEvent::Show || type == QEvent::Hide){
           navi_bar_->onToolWndVisibleChanged(ToolWndType::Favorite,
                                              bookmark_widget_->isVisible());
        }
    }else if(obj == history_widget_){
        if(type == QEvent::Show || type == QEvent::Hide){
            navi_bar_->onToolWndVisibleChanged(ToolWndType::History,
                                               history_widget_->isVisible());
        }
    }else if(obj == download_widget_){
        if(type == QEvent::Show || type == QEvent::Hide){
            navi_bar_->onToolWndVisibleChanged(ToolWndType::Download,
                                               download_widget_->isVisible());
        }
    }else if(obj == userinfo_popup_){
        if(type == QEvent::Show || type == QEvent::Hide){
            navi_bar_->onToolWndVisibleChanged(ToolWndType::UserInfo,
                                               userinfo_popup_->isVisible());
        }
    }
    return QtWinFramelessWindow::eventFilter(obj, event);
}

void MainWindow::onInpWndCntChanged()
{
    if(created_cfg_.is_inprivate_){
        navi_bar_->inpWndCntChanged();
    }
}

void MainWindow::showEvent(QShowEvent *event)
{
    QtWinFramelessWindow::showEvent(event);
    static bool firstShown = true;
    if(firstShown){
        qInfo()<<"\033[34m[Thread]"<<__FUNCTION__<<QThread::currentThreadId()<<"\033[0m";
        firstShown = false;
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    window_closing_ = true;

    if(stack_browsers_->count() > 0 || tab_bar_->count() > 0){
        // 一个一个的关闭，等待关闭完，最后一个Page关闭后会再次触发 close
        onTabBarCloseRequested(0);
        event->ignore();
        return;
    }else
    {
        // 真正的关闭
        AppCfgMgr::setWindowGeometry(saveGeometry());
        event->accept();
    }
}

void MainWindow::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::WindowStateChange)
    {
        onWindowStateChanged();
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    return QtWinFramelessWindow::mousePressEvent(event);
#ifdef Q_OS_WIN
    QRect dragRecet(tab_bar_->rect().x(),
                   tab_bar_->rect().y(),
                   tab_bar_->rect().width() - tab_bar_->reservedWidth(),
                   tab_bar_->rect().height());
    if(dragRecet.contains(event->pos()) && event->button() == Qt::LeftButton)
    {
        if(::ReleaseCapture()){
            SendMessage(HWND(this->winId()), WM_SYSCOMMAND, SC_MOVE + HTCAPTION, 0);
            event->ignore();
        }
    }
#endif
    QtWinFramelessWindow::mousePressEvent(event);
}

void MainWindow::mouseDoubleClickEvent(QMouseEvent *event)
{
    return QtWinFramelessWindow::mouseDoubleClickEvent(event);
#ifdef Q_OS_WIN
    QRect dragRecet(tab_bar_->rect().x(),
                   tab_bar_->rect().y(),
                   tab_bar_->rect().width() - tab_bar_->reservedWidth(),
                   tab_bar_->rect().height());
    if(dragRecet.contains(event->pos()) && event->button() == Qt::LeftButton)
    {
        QTimer::singleShot(0, this, &MainWindow::onNormalMax);
    }
#endif
    QtWinFramelessWindow::mouseDoubleClickEvent(event);
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QtWinFramelessWindow::paintEvent(event);
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
}

void MainWindow::moveEvent(QMoveEvent *event)
{
    QtWinFramelessWindow::moveEvent(event);
//    auto screenNum = qApp->desktop()->screenNumber(this);
//    if(screenNum != screen_number_){
//        resize(width() + 1, height());
//        screen_number_ = screenNum;
//    }
}

#ifdef Q_OS_WIN
bool MainWindow::hitTestCaption(const QPoint &gPos)
{
    return tab_bar_->hitTestCaption(gPos);
}
#endif

void MainWindow::initQtShortcut()
{
    // The shortcut is active when its parent widget has focus
    //    ac_shortcut_zoomout_->setShortcutContext(Qt::WidgetShortcut);

    ac_shortcut_zoomout_ = new QAction(this);
    ac_shortcut_zoomout_->setShortcut(QKeySequence::ZoomOut);
    connect(ac_shortcut_zoomout_, &QAction::triggered, this, &MainWindow::onZoomOut);

    // QKeySequence("Ctrl+-")
    //    qInfo()<< QKeySequence::keyBindings(QKeySequence::ZoomOut);

    ac_shortcut_resetzoom_ = new QAction(this);
    ac_shortcut_resetzoom_->setShortcut(QKeySequence(Qt::CTRL, Qt::Key_0));
    connect(ac_shortcut_resetzoom_, &QAction::triggered, this, &MainWindow::onZoomReset);

    ac_shortcut_zoomin_ = new QAction(this);
    ac_shortcut_zoomin_->setShortcut(QKeySequence::ZoomIn);
    connect(ac_shortcut_zoomin_, &QAction::triggered, this, &MainWindow::onZoomIn);

    // QKeySequence("Ctrl++")
    //    qInfo()<< QKeySequence::keyBindings(QKeySequence::ZoomIn);

    ac_shortcut_fullscn_ = new QAction(this);
    ac_shortcut_fullscn_->setShortcut(QKeySequence::FullScreen);
    connect(ac_shortcut_fullscn_, &QAction::triggered, this, &MainWindow::onFullScreen);

    //QKeySequence("F11"), QKeySequence("Alt+Enter")
    //    qInfo()<< QKeySequence::keyBindings(QKeySequence::FullScreen);

    ac_shortcut_devtool_ = new QAction(this);
    ac_shortcut_devtool_->setShortcut(QKeySequence(Qt::Key_F12));
    connect(ac_shortcut_devtool_, &QAction::triggered, this, &MainWindow::onDevTool);

    addAction(ac_shortcut_zoomout_);
    addAction(ac_shortcut_resetzoom_);
    addAction(ac_shortcut_zoomin_);
    addAction(ac_shortcut_fullscn_);
    addAction(ac_shortcut_devtool_);
}

void MainWindow::initUi()
{
    setMinimumSize(200,200);
    if(auto window = this->windowHandle()){
        connect(window, &QWindow::screenChanged, this, &MainWindow::onScreenChanged);
    }
    /*删除QLayout原来的 Menubar */
    auto menuBar = this->layout()->menuBar();
    if(menuBar){
        delete menuBar;
        menuBar = nullptr;
    }

    widget_north_ = new QWidget(this);
    widget_north_->setObjectName("mainwindow_north_widget");

    widget_north_layout_ = new QVBoxLayout(widget_north_);
    widget_north_layout_->setContentsMargins(0,0,0,0);
    widget_north_layout_->setSpacing(0);

    tab_bar_ = new TabPageToolBar(created_cfg_.is_inprivate_, this);

    navi_bar_ = new NavigateToolBar(created_cfg_.is_inprivate_);

    bookmark_bar_ = new BookmarkBar;
    bookmark_bar_->installEventFilter(this);
    bookmark_bar_->setMinimumHeight(32);

    notify_bar_ = new NotificationBar;
    notify_bar_->setMinimumHeight(32);
    notify_bar_->hide();

    widget_north_layout_->addWidget(tab_bar_);
    widget_north_layout_->addWidget(navi_bar_);
    widget_north_layout_->addWidget(bookmark_bar_);
    widget_north_layout_->addWidget(notify_bar_);

    /*设置成自定义的 MenuBar （其实是QWidget*）*/
    layout()->setMenuBar(widget_north_);


    /*设置centralWidget*/
    if(!centralWidget()) {
        setCentralWidget(new QWidget);
    }

    widget_west_ = new QWidget(this);
    widget_west_->setObjectName("mainwindow_west_widget");

    stack_browsers_ = new QStackedWidget;
    stack_browsers_->setLineWidth(0);

    widget_east_ = new QWidget(this);
    widget_east_->setMinimumWidth(300);
    widget_east_->setObjectName("mainwindow_east_widget");
    widget_east_->setFixedWidth(300);
    widget_east_->hide();
    widget_east_layout_ = new QVBoxLayout(widget_east_);
    widget_east_layout_->setContentsMargins(0,0,0,0);
    widget_east_layout_->setSpacing(0);

    /* 左（西）、中、右（东）*/
    central_area_layout_ = new QHBoxLayout;
    central_area_layout_->addWidget(widget_west_);
    central_area_layout_->addWidget(stack_browsers_);
    central_area_layout_->addWidget(widget_east_);

    widget_south_ = new QWidget(this);
    widget_south_->setObjectName("mainwindow_south_widget");

    /*中间区域布局 + 下（南）*/
    central_widget_layout_ = new QVBoxLayout;
    central_widget_layout_->setContentsMargins(0,0,0,0);
    central_widget_layout_->setSpacing(0);
    central_widget_layout_->addLayout(central_area_layout_);
    central_widget_layout_->addWidget(widget_south_);
    centralWidget()->setLayout(central_widget_layout_);

    tab_thumbnail_ = new Tab_Thumbnail_Widget(this);
    tab_thumbnail_anime_ = new QPropertyAnimation(this);
    tab_thumbnail_anime_->setTargetObject(tab_thumbnail_);   // 动画作用的对象
    tab_thumbnail_anime_->setPropertyName("geometry"); // 动画要动的属性
    tab_thumbnail_anime_->setDuration(50);       // 动画持续时间

    add_favorite_popup_ = new AddBkmkPopup(this);
    add_favorite_popup_->resize(340,190);

    popup_history_ = new PopupGeneral(this);
    popup_history_->resize(360,600);

    history_widget_ = new HistoryWidget;
    history_widget_->installEventFilter(this);
    popup_history_->setWidget(history_widget_);

    popup_bookmark_ = new PopupGeneral(this);
    popup_bookmark_->resize(360,600);

    bookmark_widget_ = new BookmarkWidget;
    bookmark_widget_->installEventFilter(this);
    popup_bookmark_->setWidget(bookmark_widget_);

    popup_download_ = new PopupGeneral(this);
    popup_download_->resize(360,600);

    download_widget_ = new DownloadWidget;
    download_widget_->installEventFilter(this);
    popup_download_->setWidget(download_widget_);

    userinfo_popup_ = new UserInfoPopup(this);
    userinfo_popup_->resize(320, 360);
    userinfo_popup_->installEventFilter(this);

    bookmark_bar_->setVisible(AppCfgMgr::Instance().bookmarkBarVisible());
}

void MainWindow::setAppearance()
{
    if(created_cfg_.is_inprivate_){
        QFile file(":/styles/resources/styles/inprivate.qss");
        if(file.open(QIODevice::ReadOnly)){
            auto all = file.readAll();
            setStyleSheet(QString::fromUtf8(all));
        }
    }
}

void MainWindow::initSignalSlot()
{
#ifdef Q_OS_WIN
    connect(tab_bar_, &TabPageToolBar::minBtnClicked, this, &MainWindow::showMinimized);
    connect(tab_bar_, &TabPageToolBar::normalMaxBtnClicked, this, &MainWindow::onNormalMax);
    connect(tab_bar_, &TabPageToolBar::closeBtnClicked, this, &MainWindow::close);
#endif
    connect(tab_bar_, &TabPageToolBar::currentChanged, this, &MainWindow::onTabBarCurrentChanged);
    connect(tab_bar_, &TabPageToolBar::tabCloseRequested,this, &MainWindow::onTabBarCloseRequested);
    connect(tab_bar_, &TabPageToolBar::tabMoved, this, &MainWindow::onTabBarTabMoved);
    connect(tab_bar_, &TabPageToolBar::showPreview, this, &MainWindow::onShowTabThumnail);
    connect(tab_bar_, &TabPageToolBar::addPage, [this]()
    {
        AddNewPage("about:version", true);
    });
    connect(tab_bar_, &TabPageToolBar::showDockPage, [this]()
    {
        // todo:
        qInfo()<<this->objectName();
    });
    connect(tab_bar_, &TabPageToolBar::testBtnClicked, [this]()
    {
        auto visible = stack_browsers_->isVisible();
        stack_browsers_->setVisible(!visible);
    });
    connect(tab_bar_, &TabPageToolBar::tabbarMenuTriggered, this, &MainWindow::onTabBarMenuTriggered);
#ifdef Q_OS_WIN
    connect(this, &MainWindow::dwmColorChanged, tab_bar_, &TabPageToolBar::onDwmColorChanged);
    connect(this, &MainWindow::windowStateChanged, tab_bar_, &TabPageToolBar::onWndStateChanged);
#endif
    connect(navi_bar_, &NavigateToolBar::naviBarCmd, this, &MainWindow::onNaviBarCmd);
    connect(bookmark_bar_, &BookmarkBar::cmdTriggered, this, &MainWindow::onBookmarkCmd);
    connect(bookmark_bar_, &BookmarkBar::appBtnClicked, this, [](const QVariant &data)
    {
        auto pos = data.toPoint();
        CefQWidget *w = new CefQWidget("cn.bing.com");
        w->setAttribute(Qt::WA_DeleteOnClose);
        w->move(pos);
        w->resize(512, 364);
        w->show();
    });
    connect(history_widget_, &HistoryWidget::pinOrCloseClicked, this, &MainWindow::onPinOrCloseHistoryWidget);
    connect(history_widget_, &HistoryWidget::menuCmd, this, &MainWindow::onHistoryWidgetCmd);
    connect(bookmark_widget_, &BookmarkWidget::pinOrCloseClicked, this, &MainWindow::onPinOrCloseBookmarkWidget);
}

void MainWindow::initPage(BrowserPage *page)
{
    connect(page, &BrowserPage::pageCmd, this, &MainWindow::onPageCmd);
    connect(page, &BrowserPage::newPage, [this](BrowserPage *page){AddNewPage(page);});
    connect(page, &BrowserPage::browserShortcut, this, &MainWindow::onBrowserShortcut);
}

BrowserPage *MainWindow::CurrentPage()
{
    auto widget = stack_browsers_->currentWidget();
    if(widget){
        return qobject_cast<BrowserPage *>(widget);
    }
    return nullptr;
}

int MainWindow::CurrentPageIndex()
{
    return tab_bar_->currentIndex();
}

BrowserPage *MainWindow::GetPage(int index)
{
    return qobject_cast<BrowserPage*>(stack_browsers_->widget(index));
}

void MainWindow::onScreenChanged(QScreen *screen)
{
    qInfo()<<__FUNCTION__<<screen;
    QTimer::singleShot(30, [this]()
    {
        static bool ok = false;
        auto delta = ok ? 1 : -1;
        ok = !ok;
        resize(width() + delta,  height());
    });
}

void MainWindow::onStatusMessage(const QString &msg)
{
    // if message is url, decode it
    QUrl url(msg);
    QString tempMsg = url.isValid()
            ? url.toDisplayString()
            : msg;
    // get elide text
    QFontMetrics fontMetrics(QToolTip::font());
    tempMsg = fontMetrics.elidedText(tempMsg, Qt::ElideRight, this->width() / 2);

    auto pos = mapToGlobal(centralWidget()->pos());
    pos.rx() -= contentsMargins().left();
    pos.ry() += centralWidget()->height();
    pos.ry() -= fontMetrics.height() / 2;
    pos.ry() -= style()->pixelMetric(QStyle::PM_TitleBarHeight);
    pos.ry() -= contentsMargins().bottom();
    QToolTip::showText(pos, tempMsg, this);
}

void MainWindow::pageZoomChanged()
{
    auto page = CurrentPage();
    if(!page){
        return;
    }
    auto zoomLevel = page->getBrowserWidget()->ZoomLevel();
    navi_bar_->setZoomLevelValue(zoomLevel);
    page->showZoomBar(navi_bar_->zoomBtnPos());
}

void MainWindow::onTabBarCurrentChanged(int index)
{
    // 控制stacked切换到对应的窗口
    if (index < stack_browsers_->count() && index >= 0)
        stack_browsers_->setCurrentIndex(index);

    // 地址栏需要改变
    auto page = GetPage(index);
    if(page){
        navi_bar_->setAddress(page->url(), page->edited());
        navi_bar_->setLoadingState(page->isLoading(),
                                   page->canGoBack(),
                                   page->canGoForward());
        navi_bar_->setZoomLevelValue(page->getBrowserWidget()->ZoomLevel());
    }
}

void MainWindow::onTabBarCloseRequested(int index)
{
    if(stack_browsers_->count() == 1){
        window_closing_ = true;
    }
    auto page = GetPage(index);
    if(page){
        addRecently(page);
        page->close();
    }
}

void MainWindow::onTabBarTabMoved(int from, int to)
{
    const QSignalBlocker blocker(stack_browsers_);
    QWidget *w = stack_browsers_->widget(from);
    stack_browsers_->removeWidget(w);
    stack_browsers_->insertWidget(to, w);
}

void MainWindow::onTabBarMenuTriggered(TabBarCmd cmd, const QVariant &para)
{
    switch (cmd) {
    case TabBarCmd::NewTabPage:
        onNaviBarCmd(NaviBarCmd::NewTabPage, para);
        break;
    case TabBarCmd::Reload:
        if(auto page = CurrentPage()){
            page->getBrowserWidget()->Refresh();
        }
        break;
    case TabBarCmd::Mute:
        if(auto page = GetPage(para.toInt())){
            page->getBrowserWidget()->MuteAudio();
        }
        break;
    case TabBarCmd::CloseTab:
        onTabBarCloseRequested(para.toInt());
        break;
    case TabBarCmd::CloseRight:
    {
        auto index = para.toInt();
        if(index >= 0 && index + 1 <= tab_bar_->count() - 1){
            right_closing_ = true;
            onTabBarCloseRequested(index + 1);
        }else{
            right_closing_ = false;
        }
    }
        break;
    case TabBarCmd::CloseOther:
    {
        auto index = para.toInt();
        if(index > 0 && index <= tab_bar_->count() - 1){
            tab_bar_->moveTab(index, 0);
        }
        onTabBarMenuTriggered(TabBarCmd::CloseRight, 0);
    }
        break;
    default:
        break;
    }
}

void MainWindow::onNaviBarCmd(NaviBarCmd cmd, const QVariant &para)
{
    auto page = CurrentPage();
    switch (cmd) {
    case NaviBarCmd::Navigate:
        NavigateInCurPage(para.toString());
        break;
    case NaviBarCmd::Back:
        onGoBack();
        break;
    case NaviBarCmd::HomePage:
        onHomepage();
        break;
    case NaviBarCmd::Forward:
        onGoForward();
        break;
    case NaviBarCmd::Refresh:
        onRefresh();
    case NaviBarCmd::StopLoading:
    {
        if(page){
            page->getBrowserWidget()->StopLoading();
        }
    }
        break;
    case NaviBarCmd::ViewSiteInfo:
        if(page){
            page->showSiteInfomation(para.toPoint());
        }
        break;
    case NaviBarCmd::AddFavorite:
        onAddFavorite();
        break;
    case NaviBarCmd::AddressEdited:
        if(page){
            page->setEditedText(para.toString());
        }
        break;
    case NaviBarCmd::ShowZoomBar:
        if(page){
            page->showZoomBar(para.toPoint());
        }
        break;
    case NaviBarCmd::Favorite:
        onShowBookmark();
        break;
    case NaviBarCmd::History:
        onShowHistory();
        break;
    case NaviBarCmd::Download:
        onShowDownload();
        break;
    case NaviBarCmd::Inprivate:
        onShowInprivate();
        break;
    case NaviBarCmd::User:
        onShowUser();
        break;
    case NaviBarCmd::NewTabPage:
        AddNewPage(para.toString(), true);
        break;
    case NaviBarCmd::NewWindow:
        MainWndMgr::Instance().createWindow(MainWndCfg());
        break;
    case NaviBarCmd::NewInprivateWindow:
    {
        MainWndCfg cfg;
        cfg.is_inprivate_ = true;
        MainWndMgr::Instance().createWindow(cfg);
    }
        break;
    case NaviBarCmd::ZoomOut:
        onZoomOut();
        break;
    case NaviBarCmd::ZoomIn:
        onZoomIn();
        break;
    case NaviBarCmd::FullScreen:
        onFullScreen();
        break;
    case NaviBarCmd::Print:
        onPrint();
        break;
    case NaviBarCmd::Capture:
    {

    }
        break;
    case NaviBarCmd::Find:
    {

    }
        break;
    case NaviBarCmd::TaskMgr:
    {

    }
        break;
    case NaviBarCmd::DevTool:
        onDevTool();
        break;
    case NaviBarCmd::Settings:
        onSettings();
        break;
    case NaviBarCmd::About:
    {
        if(!about_dialog_){
            about_dialog_ = new AboutDialog(this);
        }
        auto pos = this->pos();
        pos.rx() += 8;
        about_dialog_->move(pos);
        about_dialog_->resize(size());
        about_dialog_->exec();
    }
        break;
    case NaviBarCmd::Feedback:
        AddNewPage(AppCfgMgr::gProjectUrl, true);
        break;
    case  NaviBarCmd::Like:
        AddNewPage(AppCfgMgr::gProjectUrl, true);
        break;
    case NaviBarCmd::AboutQt:
        QMessageBox::aboutQt(this, tr("About Qt"));
        break;
    case NaviBarCmd::AboutCef:
        AddNewPage("about:version", true);
        break;
    case NaviBarCmd::QuitApp:
        MainWndMgr::Instance().quitApplication();
        break;
    default:
        break;
    }
}

void MainWindow::onPageCmd(PageCmd cmd, const QVariant &para)
{
    auto sender = QObject::sender();
    BrowserPage *page = nullptr;
    if(sender){
        page = qobject_cast<BrowserPage *>(sender);
    }else{
        qInfo()<<__FUNCTION__<<"fatal error!";
        return;
    }

    switch(cmd){
    case PageCmd::NeedSize:
    {
    }
        break;
    case PageCmd::Created:
    {
        qInfo() << __FUNCTION__ << "PageCmd::BrowserCreated";
        if(!first_browser_created_){
            first_browser_created_ = true;
            if(!created_cfg_.load_after_created_.isEmpty()){
                for(auto item : created_cfg_.load_after_created_)
                {
                    AddNewPage(item, false);
                }
            }
        }
    }
        break;
    case PageCmd::Closing:
    {
        if(page){
            int index = stack_browsers_->indexOf(page);
            tab_bar_->removeTab(index);
            stack_browsers_->removeWidget(page);
            // 必须要删除才能真正的释放cef的browser
            page->deleteLater();

            // 关掉一个以后，根据标志位，进行下一步操作
            // 1.是否处于关闭右侧标签的状态
            if(right_closing_){
                QTimer::singleShot(0,[index, this](){
                    onTabBarMenuTriggered(TabBarCmd::CloseRight, index - 1);
                });
            }
            // 2.是否处于用户要求关闭窗口的状态
            if(window_closing_){
                QTimer::singleShot(0, this, &MainWindow::close);
            }
        }
    }
        break;

    case PageCmd::Address:
    {
        if(page && page == CurrentPage()){
            page->setEditedText("");
            navi_bar_->setAddress(QUrl(page->url()).toDisplayString());
        }
    }
        break;
    case PageCmd::Title:
    {
        auto index = stack_browsers_->indexOf(page);
        tab_bar_->setTabText(index, para.toString());
        QString tip = page->title() + "\n" + QUrl(page->url()).host();
        tab_bar_->setTabToolTip(index, tip);
        tab_bar_->update();
    }
        break;
    case PageCmd::FullScreen:
        onFullScreen();
        break;
    case PageCmd::StatusMessage:
    {
        onStatusMessage(para.toString());
    }
        break;
    case PageCmd::Favicon:
    {
        auto index = stack_browsers_->indexOf(page);
        tab_bar_->setTabIcon(index, QIcon(para.value<QPixmap>()));
    }
        break;
    case PageCmd::LoadStart:
    {
        auto index = stack_browsers_->indexOf(page);
        tab_bar_->setTabData(index, 0.0);
    }
        break;
    case PageCmd::LoadEnd:
    {
        if(page){
            auto index = stack_browsers_->indexOf(page);
            static bool ok  = false;
            ok = !ok;
            tab_bar_->setTabHasAudio(index, ok);
        }
    }
        break;
    case PageCmd::LoadingProgress:
    {

    }
    case PageCmd::LoadingState:
    {
        if(page){
            auto index = stack_browsers_->indexOf(page);
            tab_bar_->setTabData(index, page->isLoading());

            if(page == CurrentPage())
            {
                QUrl url(para.toString());
                navi_bar_->setLoadingState(page->isLoading(),
                                           page->canGoBack(),
                                           page->canGoForward());
            }
        }
    }
        break;
    case PageCmd::FocusChange:
    {
        auto focus = para.toBool();
        navi_bar_->setFocus(focus);
    }
        break;
    case PageCmd::ZoomOut:
        onZoomOut();
        break;
    case PageCmd::ZoomIn:
        onZoomIn();
        break;
    case PageCmd::ZoomReset:
        onZoomReset();
        break;
    case PageCmd::OpenUrl:
    {
        auto url = para.toUrl();
        AddNewPage(url.toString(), true);
    }
        break;
    default:
        break;
    }
}

void MainWindow::onShowTabThumnail(const QPoint &g_pos, const int index)
{
    auto w = tab_thumbnail_;
    auto anime = tab_thumbnail_anime_;
    if(index > -1){
        QSize size1(324,250);
        QSize size2;
        auto curIndex = stack_browsers_->currentIndex();
        auto browserWnd = stack_browsers_->widget(index);
        if(browserWnd){
            w->setDescriptionTxt(tab_bar_->tabText(index));

            // 获取缩略图，有两种情况
            // 1.当前索引不是选中的，显示缩略图
            // 2. 当前索引就是选中的，则显示文字
            if(curIndex == index)
            {
                size2 = QSize(324,80);
                w->setImgVisible(false);
            }else{
                size2 = size1;
                auto img = browserWnd->grab();
                w->setThumbnailImg(img);
                w->setImgVisible(true);
            }
        }
        // 算个坐标
        auto pos = QPoint(g_pos.x() - w->width() / 2, g_pos.y());
        if(w->isVisible()){
            anime->setKeyValueAt(0, QRect(w->pos(), size1));
            anime->setKeyValueAt(1, QRect(pos, size2));
            anime->setLoopCount(1);
            anime->start();
        }else{
            w->move(pos);
            w->show();
        }
    }else{
        w->hide();
    }
}

void MainWindow::onBrowserShortcut(CefShortcutCmd cmd)
{
   switch (cmd){
   case CefShortcutCmd::Refresh:
       onRefresh();
       break;
   case CefShortcutCmd::Fullscreen:
       onFullScreen();
       break;
   case CefShortcutCmd::DevTool:
       onDevTool();
       break;
   case CefShortcutCmd::ZoomIn:
       onZoomIn();
       break;
   case CefShortcutCmd::ZoomReset:
       onZoomReset();
       break;
   case CefShortcutCmd::ZoomOut:
        onZoomOut();
       break;
   case CefShortcutCmd::Print:
       onPrint();
       break;
   case CefShortcutCmd::NewTab:
       AddNewPage("", true);
       break;
   case CefShortcutCmd::NewWnd:
        MainWndMgr::Instance().createWindow(MainWndCfg());
       break;
   case CefShortcutCmd::NewPrivateWnd:
   {
       MainWndCfg cfg;
       cfg.is_inprivate_ = true;
       MainWndMgr::Instance().createWindow(cfg);
   }
       break;
   case CefShortcutCmd::History:
       onShowHistory();
       break;
   case CefShortcutCmd::Download:
       onShowDownload();
       break;
   case CefShortcutCmd::CloseTab:
       onTabBarCloseRequested(CurrentPageIndex());
       break;
   case CefShortcutCmd::SwitchTab:
       onTabSwitch();
       break;
   case CefShortcutCmd::NaviBack:
       onGoBack();
       break;
   case CefShortcutCmd::NaviForward:
       onGoForward();
       break;
   }
}

void MainWindow::onHistoryWidgetCmd(HistoryCmd cmd, const QVariant &para)
{
    switch (cmd) {
    case HistoryCmd::Open:
        NavigateInCurPage(para.toString());
        break;
    case HistoryCmd::OpenInNewPage:
        AddNewPage(para.toString(), false);
        break;
    case HistoryCmd::OpenInNewWnd:
    {
        MainWndCfg cfg;
        cfg.url_ = para.toString();
        MainWndMgr::Instance().createWindow(cfg);
    }
        break;
    case HistoryCmd::OpenInInprivate:
    {
        MainWndCfg cfg;
        cfg.is_inprivate_ = true;
        cfg.url_ = para.toString();
        MainWndMgr::Instance().createWindow(cfg);
    }
        break;
    default:
        break;
    }
}

void MainWindow::onPinOrCloseHistoryWidget()
{
    auto parentW = history_widget_->parentWidget();
    if(parentW == popup_history_->contentFrame()){
        // pin to right
        popup_history_->hide();
        widget_east_layout_->addWidget(history_widget_);
        history_widget_->show();
        widget_east_->show();

        history_widget_->onShowModeChanged(ToolWndShowMode::Dock);
    }
    else if(parentW == widget_east_){
        // close
        popup_history_->setWidget(history_widget_);
        widget_east_->hide();
        history_widget_->onShowModeChanged(ToolWndShowMode::Popup);
    }else{
        qInfo()<<__FUNCTION__<<"layout error!";
    }
}

void MainWindow::onBookmarkCmd(BookmarkCmd cmd, const QVariant &para)
{
    switch (cmd) {
    case BookmarkCmd::Open:
        NavigateInCurPage(para.toString());
        break;
    case BookmarkCmd::OpenInNewPage:
    {
        auto list = para.value<QList<QString>>();
        if(!list.isEmpty()){
            for (auto item : list){
                AddNewPage(item, false);
            }
        }
    }
        break;
    case BookmarkCmd::OpenInNewWnd:
    {
        MainWndCfg cfg;
        auto list = para.value<QList<QString>>();
        if(!list.isEmpty()){
            cfg.url_ = list.at(0);
            for (auto item : list){
                cfg.load_after_created_.append(item);
            }
        }
        cfg.load_after_created_.removeFirst();
        MainWndMgr::Instance().createWindow(cfg);
    }
        break;
    case BookmarkCmd::OpenInInprivate:
    {
        MainWndCfg cfg;
        cfg.is_inprivate_ = true;
        auto list = para.value<QList<QString>>();
        if(!list.isEmpty()){
            cfg.url_ = list.at(0);
            for (auto item : list){
                cfg.load_after_created_.append(item);
            }
        }
        cfg.load_after_created_.removeFirst();
        MainWndMgr::Instance().createWindow(cfg);
    }
        break;
    default:
        break;
    }
}

void MainWindow::onPinOrCloseBookmarkWidget()
{

}

void MainWindow::onGoBack()
{
    auto page = CurrentPage();
    if(page){
        page->getBrowserWidget()->GoBack();
    }
}

void MainWindow::onGoForward()
{
    auto page = CurrentPage();
    if(page){
        page->getBrowserWidget()->GoForward();
    }
}

void MainWindow::onHomepage()
{
    auto page = CurrentPage();
    if(page){
        page->getBrowserWidget()->Navigate(AppCfgMgr::homePageUrl());
    }
}

void MainWindow::onRefresh()
{
    auto page = CurrentPage();
    if(page){
        page->getBrowserWidget()->Refresh();
    }
}

void MainWindow::onAddFavorite()
{
//    auto page = CurrentPage();
//    if(!page) return;

//    auto model = BookmarkMgr::Instance()->gBookmarkModel;
//    auto barItem = model->item(0);
//    BookmarkMgr::Instance()->addBookmarkUrl(barItem->index(),
//                                                       page->url(),
//                                                       page->title());
    auto pos = navi_bar_->addBkmkBtnPos();
    pos.ry() += 2;
    pos.rx() -= add_favorite_popup_->width();
    pos.rx() += add_favorite_popup_->shadowRightWidth();
    add_favorite_popup_->move(pos);
    add_favorite_popup_->setVisible(!add_favorite_popup_->isVisible());
}

void MainWindow::onZoomOut()
{
    auto page = CurrentPage();
    if(page){
        page->getBrowserWidget()->ZoomOut();
    }
    pageZoomChanged();
}

void MainWindow::onZoomReset()
{
    auto page = CurrentPage();
    if(page){
        page->getBrowserWidget()->ZoomReset();
    }
    pageZoomChanged();
}

void MainWindow::onZoomIn()
{
    auto page = CurrentPage();
    if(page){
        page->getBrowserWidget()->ZoomIn();
    }
    pageZoomChanged();
}

void MainWindow::onFullScreen()
{
    bool isFull = isFullScreen();
    if(isFull){
        showNormal();
    }else{
        showFullScreen();
    }
}

void MainWindow::onDevTool()
{
    auto page = CurrentPage();
    if(page){
        page->openDevTool();
    }
}

void MainWindow::onShowHistory()
{
    auto parentW = history_widget_->parentWidget();
    if(parentW == popup_history_->contentFrame()){
        auto pos = navi_bar_->historyBtnPos();
        pos.ry() += 2;
        pos.rx() -= popup_history_->width();
        pos.rx() += popup_history_->shadowRightWidth();
        popup_history_->move(pos);
        popup_history_->setVisible(!popup_history_->isVisible());
    }
    else if(parentW == widget_east_){
        history_widget_->onShowModeChanged(ToolWndShowMode::Popup);
        popup_history_->setWidget(history_widget_);
        widget_east_->hide();
    }else{
        qInfo()<<__FUNCTION__<<"layout error!";
    }
}

void MainWindow::onShowBookmark()
{
    auto pos = navi_bar_->bookmarkBtnPos();
    pos.ry() += 2;
    pos.rx() -= popup_bookmark_->width();
    pos.rx() += popup_bookmark_->shadowRightWidth();
    popup_bookmark_->move(pos);
    popup_bookmark_->setVisible(!popup_bookmark_->isVisible());
}

void MainWindow::onShowDownload()
{
    auto pos = navi_bar_->downloadBtnPos();
    pos.ry() += 2;
    pos.rx() -= popup_download_->width();
    pos.rx() += popup_download_->shadowRightWidth();
    popup_download_->move(pos);
    popup_download_->setVisible(!popup_download_->isVisible());
}

void MainWindow::onShowInprivate()
{
    auto wnd = MainWndMgr::gInprivatePopup;

    auto pos = navi_bar_->inprivateBtnPos();
    pos.ry() += 2;
    pos.rx() -= wnd->width();
    pos.rx() += wnd->shadowRightWidth();
    wnd->move(pos);
    wnd->setVisible(!wnd->isVisible());
}

void MainWindow::onShowUser()
{
    auto pos = navi_bar_->userBtnPos();
    pos.ry() += 2;
    pos.rx() -= userinfo_popup_->width();
    pos.rx() += userinfo_popup_->shadowRightWidth();
    userinfo_popup_->move(pos);
    userinfo_popup_->show();
}

void MainWindow::onPrint()
{
    auto page = CurrentPage();
    if(page){
        page->getBrowserWidget()->Print();
    }
}

void MainWindow::onSettings()
{
    MainWndMgr::gAppCfgWidget->show();
}

void MainWindow::onWindowStateChanged()
{
    emit windowStateChanged(windowState(),
                            stack_browsers_->currentWidget()->size());
    if(windowState() & Qt::WindowFullScreen){
        widget_north_->hide();
        auto fullscrnWnd = MainWndMgr::gFullscrnWidget;
        fullscrnWnd->setKeyStr("F11");
        int x = this->x() + (this->width() - fullscrnWnd->width()) / 2;
        int y = this->y() + 50;
        fullscrnWnd->move(x, y);
        fullscrnWnd->show();
        QTimer::singleShot(5000, [=](){
           fullscrnWnd->hide();
        });
    }else{
        MainWndMgr::gFullscrnWidget->hide();
        widget_north_->show();
    }
}

void MainWindow::onNormalMax()
{
    if(this->windowState() == Qt::WindowNoState){
        showMaximized();
    }else{
        if(isMaximized()){
            showNormal();
        }
    }
}

void MainWindow::onTabSwitch()
{
    int index = tab_bar_->currentIndex();
    int count = tab_bar_->count();
    if(1 == count) {return;}

    if(index >= count - 1){
        tab_bar_->setCurrentIndex(0);
    }else{
        tab_bar_->setCurrentIndex(index + 1);
    }
}

void MainWindow::addRecently(BrowserPage *)
{
//    for(auto item : RecentlyHistory){
//        if(item.url == page->url()){
//            return;
//        }
//    }
//    History data{QString::number(QDateTime::currentSecsSinceEpoch()),
//                page->url(),
//                page->title(),
//                0};
//    RecentlyHistory.push(data);
}
