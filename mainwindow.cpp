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

#include "popups/HistoryPopup.h"

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

#ifdef Q_OS_WIN
#include <Windows.h>
#include <QtWin>
#pragma comment(lib, "Gdi32.lib")
#endif

MainWindow::MainWindow(const MainWindowConfig &cfg, QWidget *parent)
    : QMainWindow(parent)
    , created_cfg_(cfg)
{
    //    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowSystemMenuHint);
    initUi();
    setAppearance();
    initSignalSlot();

    auto url = cfg.url;
    if(url.isEmpty()){
        url = AppCfgMgr::homePageUrl();
        if(url.isEmpty()){
            url = "https://cn.bing.com/";
        }
    }
    addNewPage(url);
}

MainWindow::~MainWindow()
{
    qInfo()<<__FUNCTION__;
}

int MainWindow::addNewPage(const QString &url, bool switchTo)
{
    Page *page = new Page(url, this);
    initPage(page);
    auto index = stack_browsers_->addWidget(page);
    tab_bar_->insertTab(index, url);

    if(switchTo){
        tab_bar_->setCurrentIndex(index);
    }
    return index;
}

int MainWindow::addNewPage(Page *page)
{
    initPage(page);
    auto index = stack_browsers_->addWidget(page);
    tab_bar_->insertTab(index, "");
    tab_bar_->setCurrentIndex(index);
    return index;
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
    }
    return QMainWindow::eventFilter(obj, event);
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
        emit windowStateChanged(windowState(),
                                stack_browsers_->currentWidget()->size());
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if(this->isFullScreen()){
        return QMainWindow::mousePressEvent(event);
    }
#ifdef Q_OS_WIN

    QRect dragRect(0, 0, width(), tab_bar_->height());
    if(dragRect.contains(event->pos())){
        if(::ReleaseCapture()){
            SendMessage(HWND(this->winId()), WM_SYSCOMMAND, SC_MOVE + HTCAPTION, 0);
            event->ignore();
        }
    }
#endif
    QMainWindow::mousePressEvent(event);
}

void MainWindow::mouseDoubleClickEvent(QMouseEvent *event)
{
    return QMainWindow::mouseDoubleClickEvent(event);
    if(this->isFullScreen()){
    }
#ifdef Q_OS_WIN

    QRect dragRect(0, 0, width(), tab_bar_->height());
    if(!dragRect.contains(event->pos())){
        return QMainWindow::mouseDoubleClickEvent(event);
    }
    auto state = windowState();
    if(state.testFlag(Qt::WindowNoState)){
        showMaximized();
    }else{
        if(this->isMaximized()){
            showNormal();
        }
    }

#endif
    QMainWindow::mouseDoubleClickEvent(event);
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

    tab_bar_ = new TabPagesBar(created_cfg_.is_inprivate, this);
    navi_bar_ = new NaviBar;
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
    if(created_cfg_.is_inprivate){
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
        addNewPage("about:version", true);
    });
    connect(tab_bar_, &TabPagesBar::showDockPage, [this]()
    {
        addNewPage("about:version", true);
    });
    connect(tab_bar_, &TabPagesBar::testBtnClicked, [this]()
    {
        auto visible = stack_browsers_->isVisible();
        stack_browsers_->setVisible(!visible);
    });
#ifdef Q_OS_WIN
    connect(this, &MainWindow::dwmColorChanged, tab_bar_, &TabPagesBar::onDwmColorChanged);
#endif
    connect(navi_bar_, &NaviBar::naviBarCmd, this, &MainWindow::onNaviBarCmd);
    connect(this, &MainWindow::historyPopupVisibleChange, navi_bar_, &NaviBar::onHistoryPopupVisibleChange);
}

void MainWindow::initPage(Page *page)
{
    connect(page, &Page::pageCmd, this, &MainWindow::onPageCmd);
    connect(page, &Page::newPage, [this](Page *page){addNewPage(page);});
    // 顶层窗口的窗口状态改变应该通知到page去，
    connect(this, &MainWindow::windowStateChanged, [page](Qt::WindowStates state, const QVariant &data)
    {
        page->getBrowserWidget()->onTopLevelWindowStateChanged(state, data);
    });

}

Page *MainWindow::GetActivePage()
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

void MainWindow::onNaviBarCmd(NaviBarCmd cmd, const QVariant &para)
{
    auto page = GetActivePage();
    if(cmd == NaviBarCmd::Navigate)
    {
        auto url = UtilQt::check_url(para.toString());
        url = para.toString();
        if(page){
            page->getBrowserWidget()->Navigate(url);
        }
    } else if(cmd == NaviBarCmd::Back)
    {
        if(page){
            page->getBrowserWidget()->GoBack();
        }
    }else if(cmd == NaviBarCmd::HomePage)
    {
        if(page){
            page->getBrowserWidget()->Navigate(AppCfgMgr::homePageUrl());
        }
    }
    else if(cmd == NaviBarCmd::Forward)
    {
        if(page){
            page->getBrowserWidget()->GoForward();
        }
    }
    else if(cmd == NaviBarCmd::Refresh)
    {
        if(page){
            page->getBrowserWidget()->Refresh();
        }
    }
    else if(cmd == NaviBarCmd::StopLoading)
    {
        if(page){
            page->getBrowserWidget()->StopLoading();
        }
    }
    else if(cmd == NaviBarCmd::ViewSiteInfo)
    {
        if(page){
            auto rect = para.toRect();
            page->showSiteInfomation(rect);
        }
    }
    else if(cmd == NaviBarCmd::History)
    {
        auto pos = para.toPoint();
        pos.ry() += 2;
        pos.rx() -= history_popup_->width();
        pos.rx() += history_popup_->shadowRightWidth();
        history_popup_->move(pos);
        history_popup_->show();
    }
    else if(cmd == NaviBarCmd::NewTabPage){
        auto url = para.toString();
        if(url.isEmpty()){
            url = "https://cn.bing.com/";
        }
        addNewPage(url, true);
    }
    else if(cmd == NaviBarCmd::NewWindow){
        MainWndMgr::Instance().createWindow(MainWndCfg());
    }
    else if(cmd == NaviBarCmd::NewInprivateWindow)
    {
        MainWndCfg cfg{true, false, false, QRect(), ""};
        MainWndMgr::Instance().createWindow(cfg);
    }
    else if(cmd == NaviBarCmd::ZoomOut){
        if(page){
            page->getBrowserWidget()->ZoomOut();
        }
    }
    else if(cmd == NaviBarCmd::ZoomIn){
        if(page){
            page->getBrowserWidget()->ZoomIn();
        }
    }
    else if(cmd == NaviBarCmd::FullScreen){
        //        showFullScreen();
        //        showNormal();
    }
    else if(cmd == NaviBarCmd::Settings) {
        app_cfg_widget_->show();
    }
    else if(cmd == NaviBarCmd::QuitApp) {
        MainWndMgr::Instance().quitApplication();
    }
}

void MainWindow::onPageCmd(PageCmd cmd, const QVariant &para)
{
    auto sender = QObject::sender();
    Page *page = nullptr;
    if(sender){
        page = qobject_cast<Page *>(sender);
    }

    if(cmd == PageCmd::Closing)
    {
        if(page){
            tab_bar_->removeTab(stack_browsers_->indexOf(page));
            stack_browsers_->removeWidget(page);
            // 必须要删除才能真正的释放cef的browser
            page->deleteLater();

            // 关掉一个以后，紧接着判断是不是用户在关闭整个窗口
            if(window_closing_){
                QTimer::singleShot(0, this, &MainWindow::close);
            }
        }

    } else if(cmd == PageCmd::Address)
    {
        if(page && page == GetActivePage()){
            QUrl url(para.toString());
            navi_bar_->setAddress(url.toDisplayString());
        }
    } else if(cmd == PageCmd::Title)
    {
        auto index = stack_browsers_->indexOf(page);
        tab_bar_->setTabText(index, para.toString());
        tab_bar_->update();
    }else if(cmd == PageCmd::StatusMessage)
    {
        onStatusMessage(para.toString());
    }else if(cmd == PageCmd::Favicon)
    {
        auto index = stack_browsers_->indexOf(page);
        tab_bar_->setTabIcon(index, QIcon(para.value<QPixmap>()));
    } else if(cmd == PageCmd::LoadStart)
    {

    } else if(cmd == PageCmd::LoadEnd)
    {

    }else if(cmd == PageCmd::LoadingState)
    {
        if(page && page == GetActivePage()){
            QUrl url(para.toString());
            navi_bar_->setLoadingState(page->isLoading(),page->canGoBack(), page->canGoForward());
        }
    } else if(cmd == PageCmd::FocusChange){
        auto focus = para.toBool();
        navi_bar_->setFocus(focus);
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
