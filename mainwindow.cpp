#include "mainwindow.h"

#include "page.h"
#include "cef_qwidget.h"
#include "browser/cef_client_handler.h"
#include "utils/util_qt.h"
#include "toolbars/tabbar.h"
#include "toolbars/navibar.h"
#include "toolbars/bookmarkbar.h"

#include "managers/mainwindowmgr.h"
#include "managers/appconfig.h"

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
#include <QScreen>
#include <QDialog>

#ifdef Q_OS_WIN
#include <Windows.h>
#endif

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    initUi();
    setAppearance();
    initSignalSlot();

    QTimer::singleShot(40,[this](){
        addNewPage("https://www.baidu.com/");
    });
}

MainWindow::~MainWindow()
{

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

void MainWindow::closeEvent(QCloseEvent *event)
{
    window_closing_ = true;

    if(stack_browsers_->count() > 0 || tab_bar_->count() > 0){
        // 一个一个的关闭，等待关闭完
        onTabBarCloseRequested(0);
        event->ignore();
        return;
    }else{
        // 真正的关闭
        AppConfig::setWindowGeometry(saveGeometry());
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
    return QMainWindow::mousePressEvent(event);
    if(this->isFullScreen()){
    }
#ifdef Q_OS_WIN
    int h = 0;
    h += layout_->contentsMargins().top();
    h += tabbar_layout_->contentsMargins().top();
    h += tab_bar_->height();

    QRect dragRect(0, 0, width(), h);
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
    int h = 0;
    h += layout_->contentsMargins().top();
    h += tabbar_layout_->contentsMargins().top();
    h += tab_bar_->height();

    QRect dragRect(0, 0, width(), h);
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
    static bool first_window = true;
    if(first_window)
    {   // 首个窗口设置记录的位置和大小
        auto geo = AppConfig::windowGeometry();
        if(geo.isEmpty() || !restoreGeometry(geo)){
            const QRect availableGeometry =  qApp->primaryScreen()->availableGeometry();
            const QSize size = (availableGeometry.size() * 4) / 5;
            resize(size);
            move(availableGeometry.center() - QPoint(size.width(), size.height()) / 2);

        }
        first_window = false;
    }

    QPalette pl = palette();
    pl.setColor(QPalette::Active, QPalette::Window, QColor("#CECECE"));
    pl.setColor(QPalette::Inactive, QPalette::Window, QColor("#E8E8E8"));
    setPalette(pl);

    /*设置centralWidget*/
    if(!centralWidget()) {
        setCentralWidget(new QWidget);
    }

    layout_ = new QVBoxLayout;
    btn_dock_tabs_ = new QToolButton;
    btn_dock_tabs_->setToolTip(tr("open vertical tabs"));
    tab_bar_ = new TabBar;
    btn_add_page_ = new QToolButton;
    btn_add_page_->setToolTip(tr("Add a tab page"));
    navi_bar_ = new NaviBar;
    bookmark_bar_ = new BookmarkBar;
    stack_browsers_ = new QStackedWidget;
    stack_browsers_->setLineWidth(0);

    tabbar_layout_ = new QHBoxLayout;
    tabbar_layout_->setContentsMargins(6, 6, 0, 0);
    tabbar_layout_->setSpacing(2);
    tabbar_layout_->addSpacerItem(new QSpacerItem(6,10,QSizePolicy::Fixed));
    tabbar_layout_->addWidget(btn_dock_tabs_);
    tabbar_layout_->addWidget(tab_bar_);
    tabbar_layout_->addWidget(btn_add_page_);
    tabbar_layout_->addStretch();
    tabbar_layout_->addSpacerItem(new QSpacerItem(180,10,QSizePolicy::Fixed));

    layout_->addLayout(tabbar_layout_);
    layout_->addWidget(navi_bar_);
    layout_->addWidget(bookmark_bar_);
    layout_->addWidget(stack_browsers_);

    centralWidget()->setLayout(layout_);
    layout_->setContentsMargins(0,0,0,0);
    layout_->setSpacing(0);
}

void MainWindow::setAppearance()
{
    QSize iconSize(24,24);
    btn_dock_tabs_->setIconSize(iconSize);
    btn_add_page_->setIconSize(iconSize);
    btn_dock_tabs_->setIcon(QIcon(":/icons/resources/normal_pagelist_hide.png"));
    btn_add_page_->setIcon(QIcon(":/icons/resources/addb_30px.png"));
}

void MainWindow::initSignalSlot()
{
    connect(tab_bar_, &QTabBar::currentChanged, this, &MainWindow::onTabBarCurrentChanged);
    connect(tab_bar_, &QTabBar::tabCloseRequested,this, &MainWindow::onTabBarCloseRequested);
    connect(tab_bar_, &QTabBar::tabMoved, this, &MainWindow::onTabBarTabMoved);

    connect(btn_add_page_, &QToolButton::clicked, [this]()
    {
       addNewPage("about:version", true);
    });
    connect(navi_bar_, &NaviBar::naviBarCmd, this, &MainWindow::onNaviBarCmd);

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

void MainWindow::onTabBarCurrentChanged(int index)
{
    // 控制stacked切换到对应的窗口
    if (index < stack_browsers_->count() && index >= 0)
        stack_browsers_->setCurrentIndex(index);

    // 地址栏需要改变
    auto page = GetPage(index);
    if(page){
        navi_bar_->setAddress(page->GetUrl());
        navi_bar_->setLoadingState(page->IsLoading(),
                                   page->CanGoBack(),
                                   page->CanGoForward());
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
        if(page){
            page->getBrowserWidget()->Navigate(url);
        }
    } else if(cmd == NaviBarCmd::Back)
    {
        if(page){
            page->getBrowserWidget()->GoBack();
        }
    }else if(cmd == NaviBarCmd::Forward)
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
    }else if(cmd == NaviBarCmd::StopLoading)
    {
        if(page){
            page->getBrowserWidget()->StopLoading();
        }
    } else if(cmd == NaviBarCmd::ViewSiteInfo)
    {
        if(page){
            auto rect = para.toRect();
            page->showSiteInfomation(rect);
        }
    }
    else if(cmd == NaviBarCmd::NewTabPage){
        auto url = para.toString();
        if(url.isEmpty()){
            url = "https://cn.bing.com/";
        }
        addNewPage(url, true);
    } else if(cmd == NaviBarCmd::NewWindow){
        MainWindowMgr::instance().createWindow();
    } else if(cmd == NaviBarCmd::NewInprivateWindow) {
        qInfo()<<__FUNCTION__<<"TODO:";
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
    }else if(cmd == PageCmd::Favicon)
    {
        auto index = stack_browsers_->indexOf(page);
        tab_bar_->setTabIcon(index, QIcon(para.value<QPixmap>()));
    } else if(cmd == PageCmd::LoadingState)
    {
        if(page && page == GetActivePage()){
            QUrl url(para.toString());
            navi_bar_->setLoadingState(page->IsLoading(),page->CanGoBack(), page->CanGoForward());
        }
    } else if(cmd == PageCmd::FocusChange){
        auto focus = para.toBool();
        navi_bar_->setFocus(focus);
    }
}
