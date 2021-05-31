#include "mainwindow.h"

#include "page.h"
#include "cef_qwidget.h"
#include "browser/cef_client_handler.h"
#include "utils/util_qt.h"
#include "utils/util_win.h"
#include "toolbars/TabPagesBar.h"
#include "toolbars/NavigateBar.h"
#include "toolbars/BookmarkBar.h"
#include "toolbars/NotificationBar.h"
#include "widgets/TabThumbnailWidget.h"
#include "widgets/appconfigwidget.h"
#include "managers/AppCfgManager.h"
#include "managers/CefManager.h"

#include "popups/HistoryPopup.h"
#include "popups/InprivatePopup.h"
#include "popups/UserInfoPopup.h"
#include "popups/ZoomPopup.h"

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

#ifdef Q_OS_WIN
#include <Windows.h>
#include <QtWin>
#pragma comment(lib, "Gdi32.lib")
#endif

InprivatePopup* MainWindow::gInprivatePopup = nullptr;
ZoomPopup* MainWindow::gZoomPopup = nullptr;

MainWindow::MainWindow(const MainWindowConfig &cfg, QWidget *parent)
    : QMainWindow(parent)
    , created_cfg_(cfg)
{
    //    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowSystemMenuHint);
    initQtShortcut();
    initUi();
    setAppearance();
    initSignalSlot();

    auto url = cfg.url_;
    if(url.isEmpty()){
        url = AppCfgMgr::homePageUrl();
        if(url.isEmpty()){
            url = "https://cn.bing.com/";
        }
    }
    AddNewPage(url);
}

MainWindow::~MainWindow()
{
    qInfo()<<__FUNCTION__;
}

int MainWindow::AddNewPage(const QString &url, bool switchTo)
{
    auto url1 = url;
    if(url1.isEmpty()){
        url1 = "https://cn.bing.com/";
    }
    Page *page = new Page(url1, this);
    initPage(page);
    auto index = stack_browsers_->addWidget(page);
    tab_bar_->insertTab(index, url);

    if(switchTo){
        tab_bar_->setCurrentIndex(index);
    }
    return index;
}

int MainWindow::AddNewPage(Page *page)
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

void MainWindow::updateInprivateCount()
{
    if(!gInprivatePopup){
        gInprivatePopup = new InprivatePopup;
    }
    auto cnt = MainWndMgr::Instance().inprivateCount();
    gInprivatePopup->setHintText(tr(" %1 inprivate window%2 opened")
                                 .arg(cnt).arg(cnt > 1 ? "s" : ""));
}

bool MainWindow::event(QEvent *e)
{
    return QMainWindow::event(e);
}
bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    auto type = event->type();
    if(obj == history_popup_){
        if(type == QEvent::Show || type == QEvent::Hide){
            emit historyPopupVisibleChange(history_popup_->isVisible());
        }
    }else if(obj == userinfo_popup_){
        if(type == QEvent::Show || type == QEvent::Hide){
            emit userInfoPopupVisibleChange(userinfo_popup_->isVisible());
        }
    }
    return QMainWindow::eventFilter(obj, event);
}

void MainWindow::onInpWndCntChanged()
{
    if(created_cfg_.is_inprivate_){
        navi_bar_->inpWndCntChanged();
    }
}

bool MainWindow::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
#ifdef Q_OS_WIN
    //Workaround for known bug -> check Qt forum : https://forum.qt.io/topic/93141/qtablewidget-itemselectionchanged/13
#if (QT_VERSION == QT_VERSION_CHECK(5, 11, 1))
    MSG* msg = *reinterpret_cast<MSG**>(message);
#else
    MSG* msg = reinterpret_cast<MSG*>(message);
#endif

#define LOG_MACRO(x) #x
    switch (msg->message)
    {
    case WM_DWMCOLORIZATIONCOLORCHANGED:
        emit dwmColorChanged();
        return false;
    case WM_DPICHANGED:
        return false;
    case WM_NCPAINT:

        return false;
    case WM_NCRBUTTONDOWN:
        qInfo()<<__FUNCTION__<<LOG_MACRO(WM_NCRBUTTONDOWN);
        qInfo()<<msg->wParam;
        return false;
    case WM_NCRBUTTONUP:
        qInfo()<<__FUNCTION__<<LOG_MACRO(WM_NCRBUTTONUP);
        return false;
    default:
        break;
    }
    return QMainWindow::nativeEvent(eventType, message, result);
#endif
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
    QMainWindow::mousePressEvent(event);
}

void MainWindow::mouseDoubleClickEvent(QMouseEvent *event)
{
    QMainWindow::mouseDoubleClickEvent(event);
}

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

    tab_bar_ = new TabPagesBar(created_cfg_.is_inprivate_, this);
    navi_bar_ = new NaviBar;
    navi_bar_->setInprivate(created_cfg_.is_inprivate_);
    bookmark_bar_ = new BookmarkBar;
    notify_bar_ = new NotificationBar;

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
    widget_east_->setObjectName("mainwindow_east_widget");

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

    history_popup_ = new HistoryPopup(this);
    history_popup_->resize(360, 600);
    history_popup_->installEventFilter(this);

    userinfo_popup_ = new UserInfoPopup(this);
    userinfo_popup_->resize(320, 360);
    userinfo_popup_->installEventFilter(this);

    app_cfg_widget_ = new AppCfgWidget(this);

    notify_bar_->hide();
#if 0
    widget_west_->setMinimumWidth(70);
    widget_east_->setMinimumWidth(70);
    widget_south_->setMinimumHeight(50);
#endif
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
    connect(tab_bar_, &TabPagesBar::currentChanged, this, &MainWindow::onTabBarCurrentChanged);
    connect(tab_bar_, &TabPagesBar::tabCloseRequested,this, &MainWindow::onTabBarCloseRequested);
    connect(tab_bar_, &TabPagesBar::tabMoved, this, &MainWindow::onTabBarTabMoved);
    connect(tab_bar_, &TabPagesBar::showPreview, this, &MainWindow::onShowTabThumnail);
    connect(tab_bar_, &TabPagesBar::addPage, [this]()
    {
        AddNewPage("about:version", true);
    });
    connect(tab_bar_, &TabPagesBar::showDockPage, [this]()
    {
        AddNewPage("about:version", true);
    });
    connect(tab_bar_, &TabPagesBar::testBtnClicked, [this]()
    {
        auto visible = stack_browsers_->isVisible();
        stack_browsers_->setVisible(!visible);
    });
    connect(tab_bar_, &TabPagesBar::tabbarMenuTriggered, this, &MainWindow::onTabBarMenuTriggered);
#ifdef Q_OS_WIN
    connect(this, &MainWindow::dwmColorChanged, tab_bar_, &TabPagesBar::onDwmColorChanged);
#endif
    connect(navi_bar_, &NaviBar::naviBarCmd, this, &MainWindow::onNaviBarCmd);
    connect(this, &MainWindow::historyPopupVisibleChange, navi_bar_, &NaviBar::onHistoryPopupVisibleChange);
    connect(this, &MainWindow::userInfoPopupVisibleChange, navi_bar_, &NaviBar::onUserInfoPopupVisibleChange);
}

void MainWindow::initPage(Page *page)
{
    connect(page, &Page::pageCmd, this, &MainWindow::onPageCmd);
    connect(page, &Page::newPage, [this](Page *page){AddNewPage(page);});
    // 顶层窗口的窗口状态改变应该通知到page去，
    connect(this, &MainWindow::windowStateChanged, [page](Qt::WindowStates state, const QVariant &data)
    {
        page->getBrowserWidget()->onTopLevelWindowStateChanged(state, data);
    });

    connect(page, &Page::browserShortcut, this, &MainWindow::onBrowserShortcut);
}

Page *MainWindow::CurrentPage()
{
    auto widget = stack_browsers_->currentWidget();
    if(widget){
        return qobject_cast<Page *>(widget);
    }
    return nullptr;
}

Page *MainWindow::GetPage(int index)
{
    return qobject_cast<Page*>(stack_browsers_->widget(index));
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

    int x = this->x();
    int y = this->y() + this->height();
    QToolTip::showText(QPoint(x, y), tempMsg, this);
}

void MainWindow::pageZoomLevelChanged()
{
    auto page = CurrentPage();
    if(!page){
        return;
    }
    auto zoomLevel = page->getBrowserWidget()->ZoomLevel();
    navi_bar_->setZoomLevelValue(zoomLevel);

    if(!gZoomPopup){
        gZoomPopup = new ZoomPopup;
    }
    gZoomPopup->setZoomLevelStr(CefManager::Instance().zoom_map.value(static_cast<int>(zoomLevel)));
    auto pos = navi_bar_->zoomBtnPos();
    pos.rx() += 10;
    pos.rx() -= gZoomPopup->width();
    pos.ry() += 2;
    gZoomPopup->move(pos);
    gZoomPopup->setVisible(zoomLevel != 0.0);
}

void MainWindow::onTabBarCurrentChanged(int index)
{
    // 控制stacked切换到对应的窗口
    if (index < stack_browsers_->count() && index >= 0)
        stack_browsers_->setCurrentIndex(index);

    // 地址栏需要改变
    auto page = GetPage(index);
    if(page){
        navi_bar_->setAddress(page->url());
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
    auto page = stack_browsers_->widget(index);
    if(page){
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
    {
        if(page){
            auto rect = para.toRect();
            page->showSiteInfomation(rect);
        }
    }
        break;
    case NaviBarCmd::ShowZoomBar:
        pageZoomLevelChanged();
        break;
    case NaviBarCmd::Favorite:
    {
        auto pos = para.toPoint();
        pos.ry() += 2;
        pos.rx() -= history_popup_->width();
        pos.rx() += history_popup_->shadowRightWidth();
        history_popup_->move(pos);
        history_popup_->show();
    }
        break;
    case NaviBarCmd::History:
        onShowHistory();
        break;
    case NaviBarCmd::Download:
    {
        auto pos = para.toPoint();
        pos.ry() += 2;
        pos.rx() -= history_popup_->width();
        pos.rx() += history_popup_->shadowRightWidth();
        history_popup_->move(pos);
        history_popup_->show();
    }
        break;
    case NaviBarCmd::Inprivate:
        onShowInprivate();
        break;
    case NaviBarCmd::User:
    {
        auto pos = para.toPoint();
        pos.ry() += 2;
        pos.rx() -= userinfo_popup_->width();
        pos.rx() += userinfo_popup_->shadowRightWidth();
        userinfo_popup_->move(pos);
        userinfo_popup_->show();
    }
        break;
    case NaviBarCmd::NewTabPage:
        AddNewPage(para.toString(), true);
        break;
    case NaviBarCmd::NewWindow:
        MainWndMgr::Instance().createWindow(MainWndCfg());
        break;
    case NaviBarCmd::NewInprivateWindow:
        MainWndMgr::Instance().createWindow(MainWndCfg(true));
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
    {
        app_cfg_widget_->show();
    }
        break;
    case NaviBarCmd::About:
        AddNewPage("https://gitee.com/slamdd/yi-guai", true);
    case NaviBarCmd::Feedback:
        AddNewPage("https://gitee.com/slamdd/yi-guai", true);
    case  NaviBarCmd::Like:
        AddNewPage("https://gitee.com/slamdd/yi-guai", true);
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
    Page *page = nullptr;
    if(sender){
        page = qobject_cast<Page *>(sender);
    }else{
        qInfo()<<__FUNCTION__<<"fatal error!";
        return;
    }

    switch(cmd){
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
            navi_bar_->setAddress(QUrl(page->url()).toDisplayString());
        }
    }
        break;
    case PageCmd::Title:
    {
        auto index = stack_browsers_->indexOf(page);
        tab_bar_->setTabText(index, para.toString());
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

    }
        break;
    case PageCmd::LoadEnd:
    {

    }
        break;
    case PageCmd::LoadingState:
    {
        if(page && page == CurrentPage()){
            QUrl url(para.toString());
            navi_bar_->setLoadingState(page->isLoading(),page->canGoBack(), page->canGoForward());
        }
    }
        break;
    case PageCmd::FocusChange:
    {
        auto focus = para.toBool();
        navi_bar_->setFocus(focus);
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

void MainWindow::onBrowserShortcut(const CefKeyEvent &event,
                                   CefEventHandle os_event)
{
    Q_UNUSED(os_event);

    // F11
    // 改变浏览器的全屏模式
    if(event.modifiers == EVENTFLAG_NONE
            && event.windows_key_code == VK_F11){
        onFullScreen();
    }

    // F12
    // 打开/关闭开发者工具
    if(event.modifiers == EVENTFLAG_NONE
            && event.windows_key_code == VK_F12){
        onDevTool();
    }

    // Ctrl + -(- 位于 数字键盘 0 右侧)
    // 缩小
    if(event.modifiers == EVENTFLAG_CONTROL_DOWN
            && event.windows_key_code == VK_OEM_MINUS)
    {
        onZoomOut();
    }
    // Ctrl + -(- 位于 小键盘 )
    // 缩小
    if(event.modifiers == (EVENTFLAG_CONTROL_DOWN | EVENTFLAG_IS_KEY_PAD)
            && event.windows_key_code == VK_SUBTRACT ){
        onZoomOut();
    }
    /* from WinUser.h
     * VK_0 - VK_9 are the same as ASCII '0' - '9' (0x30 - 0x39)
     * 0x3A - 0x40 : unassigned
     * VK_A - VK_Z are the same as ASCII 'A' - 'Z' (0x41 - 0x5A)
     */
    // Ctrl + 0(0 位于 数字键盘)
    // 恢复缩放比例
    if(event.modifiers == EVENTFLAG_CONTROL_DOWN
            && event.windows_key_code == '0'){
        onZoomReset();
    }

    // Ctrl + 0(0 位于 小键盘 )
    // 恢复缩放比例
    if(event.modifiers == (EVENTFLAG_CONTROL_DOWN | EVENTFLAG_IS_KEY_PAD)
            && event.windows_key_code == VK_NUMPAD0)
    {
        onZoomReset();
    }

    // Ctrl + +(+ 位于 backspace 左侧)
    // 放大
    if( event.modifiers == EVENTFLAG_CONTROL_DOWN
            && event.windows_key_code == VK_OEM_PLUS)
    {
        onZoomIn();
    }
    // Ctrl + +(+ 位于 小键盘 )
    // 放大
    if(event.modifiers == (EVENTFLAG_CONTROL_DOWN | EVENTFLAG_IS_KEY_PAD)
            && event.windows_key_code == VK_ADD){
        onZoomIn();
    }

    // Ctrl + P
    // 打印
    if(event.modifiers == EVENTFLAG_CONTROL_DOWN
            && event.windows_key_code == 'P'){
        onPrint();
    }

    // Ctrl + T
    // 新建标签页
    if(event.modifiers == EVENTFLAG_CONTROL_DOWN
            && event.windows_key_code == 'T'){
        AddNewPage("", true);
    }

    // Ctrl + N
    // 新建浏览器窗口
    if(event.modifiers == EVENTFLAG_CONTROL_DOWN
            && event.windows_key_code == 'N'){
        MainWndMgr::Instance().createWindow(MainWndCfg());
    }

    // Ctrl + Shift + N
    // 新建InPrivate浏览器窗口
    if(event.modifiers == (EVENTFLAG_CONTROL_DOWN | EVENTFLAG_SHIFT_DOWN)
            && event.windows_key_code == 'N'){
        MainWndMgr::Instance().createWindow(MainWndCfg(true));
    }

    // Ctrl + H
    // 查看历史记录
    if(event.modifiers == EVENTFLAG_CONTROL_DOWN
            && event.windows_key_code == 'H'){
        onShowHistory();
    }
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

void MainWindow::onZoomOut()
{
    auto page = CurrentPage();
    if(page){
        page->getBrowserWidget()->ZoomOut();
    }
    pageZoomLevelChanged();
}

void MainWindow::onZoomReset()
{
    auto page = CurrentPage();
    if(page){
        page->getBrowserWidget()->ZoomReset();
    }
    pageZoomLevelChanged();
}

void MainWindow::onZoomIn()
{
    auto page = CurrentPage();
    if(page){
        page->getBrowserWidget()->ZoomIn();
    }
    pageZoomLevelChanged();
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
    auto pos = navi_bar_->hisrotyBtnPos();
    pos.ry() += 2;
    pos.rx() -= history_popup_->width();
    pos.rx() += history_popup_->shadowRightWidth();
    history_popup_->move(pos);
    history_popup_->setVisible(!history_popup_->isVisible());
}

void MainWindow::onShowInprivate()
{
    if(!gInprivatePopup){
        gInprivatePopup = new InprivatePopup;
    }

    auto pos = navi_bar_->inprivateBtnPos();
    pos.ry() += 2;
    pos.rx() -= gInprivatePopup->width();
    pos.rx() += gInprivatePopup->shadowRightWidth();
    gInprivatePopup->move(pos);
    gInprivatePopup->setVisible(!gInprivatePopup->isVisible());
}

void MainWindow::onPrint()
{
    auto page = CurrentPage();
    if(page){
        page->getBrowserWidget()->Print();
    }
}

void MainWindow::onWindowStateChanged()
{
    emit windowStateChanged(windowState(),
                            stack_browsers_->currentWidget()->size());
    if(windowState() & Qt::WindowFullScreen){
        widget_north_->hide();
    }else{
        widget_north_->show();
    }
}
