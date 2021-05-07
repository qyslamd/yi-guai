#include "mainwindow.h"

#include "page.h"
#include "cef_qwidget.h"
#include "browser/cef_client_handler.h"
#include "utils/util_qt.h"
#include "toolbars/tabbar.h"
#include "toolbars/navibar.h"
#include "toolbars/bookmarkbar.h"

#include <QVBoxLayout>
#include <QStackedWidget>
#include <QStatusBar>
#include <QtDebug>
#include <QUrl>
#include <QCloseEvent>
#include <QTimer>
#include <QThread>
#include <QToolButton>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    resize(1100,600);
    initUi();
    setAppearance();
    initSignalSlot();

    addNewPage("https://www.baidu.com/");
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

bool MainWindow::event(QEvent *e)
{
    return QMainWindow::event(e);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    closing_ = true;

    if(stack_browsers_->count() > 0 || tab_bar_->count() > 0){
        // 一个一个的关闭，等待关闭完
        onTabBarCloseRequested(0);
        event->ignore();
        return;
    }else{
        // 真正的关闭
        qInfo()<<__FUNCTION__;
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

void MainWindow::initUi()
{
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
    tab_bar_ = new TabBar;
    btn_add_page_ = new QToolButton;
    navi_bar_ = new NaviBar;
    bookmark_bar_ = new BookmarkBar;
    stack_browsers_ = new QStackedWidget;
    stack_browsers_->setLineWidth(0);

    auto tabbar_layout = new QHBoxLayout;
    tabbar_layout->setContentsMargins(6, 6, 0, 0);
    tabbar_layout->setSpacing(2);
    tabbar_layout->addSpacerItem(new QSpacerItem(6,10,QSizePolicy::Fixed));
    tabbar_layout->addWidget(btn_dock_tabs_);
    tabbar_layout->addWidget(tab_bar_);
    tabbar_layout->addSpacerItem(new QSpacerItem(6,10,QSizePolicy::Fixed));
    tabbar_layout->addWidget(btn_add_page_);
    tabbar_layout->addStretch();
    tabbar_layout->addSpacerItem(new QSpacerItem(180,10,QSizePolicy::Fixed));

    layout_->addLayout(tabbar_layout);
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
    // 顶层窗口的窗口状态改变应该通知到page去，
    connect(this, &MainWindow::windowStateChanged, page, &Page::onTopLevelWindowStateChanged);

}

Page *MainWindow::GetActivePage()
{
    auto widget = stack_browsers_->currentWidget();
    if(widget){
        return qobject_cast<Page *>(widget);
    }
    return nullptr;
}

void MainWindow::onTabBarCurrentChanged(int index)
{
    if (index < stack_browsers_->count() && index >= 0)
        stack_browsers_->setCurrentIndex(index);
}

void MainWindow::onTabBarCloseRequested(int index)
{
    if(stack_browsers_->count() == 1){
        closing_ = true;
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
    if(cmd == NaviBarCmd::Navigate){
        auto url = UtilQt::check_url(para.toString());
        auto page = GetActivePage();
        if(page){
            page->getBrowserWidget()->Navigate(url);
        }
    }
}

void MainWindow::onPageCmd(PageCmd cmd, const QVariant &data)
{
    if(cmd == PageCmd::Closing){
        auto sender = QObject::sender();
        if(sender){
            auto page = qobject_cast<Page *>(sender);
            if(page){
                tab_bar_->removeTab(stack_browsers_->indexOf(page));
                stack_browsers_->removeWidget(page);
                // 必须要删除才能真正的释放cef的browser
                page->deleteLater();

                // 关掉一个以后，紧接着判断是不是用户在关闭整个窗口
                if(closing_){
                    close();
                }
            }
        }
    } else if(cmd == PageCmd::None){

    }

}
