#include "mainwindow.h"

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

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    initUi();
    initSignalSlot();

    addOneBrowserPage("https://www.baidu.com/");
}

MainWindow::~MainWindow()
{

}

int MainWindow::addOneBrowserPage(const QString &url, bool switchTo)
{
    CefQWidget *page = new CefQWidget(url, this);
    auto index = stack_browsers_->addWidget(page);
    tab_bar_->insertTab(index, url);
    initPage(page);

    if(switchTo){
        stack_browsers_->setCurrentIndex(index);
    }
    return index;
    return 0;
}

bool MainWindow::event(QEvent *e)
{
    return QMainWindow::event(e);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    closing_ = true;

    if(stack_browsers_->count() > 0){
        // 一个一个的关闭，等待关闭完
        onTabPageCloseRequested(0);
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
    /*设置centralWidget*/
    if(!centralWidget()) {
        setCentralWidget(new QWidget);
    }

    layout_ = new QVBoxLayout;

    tab_bar_ = new TabBar;
    navi_bar_ = new NaviBar;
    bookmark_bar_ = new BookmarkBar;
    stack_browsers_ = new QStackedWidget;
    status_bar_ = new QStatusBar;

    layout_->addWidget(tab_bar_);
    layout_->addWidget(navi_bar_);
    layout_->addWidget(bookmark_bar_);
    layout_->addWidget(stack_browsers_);

    setStatusBar(status_bar_);

    centralWidget()->setLayout(layout_);
}

void MainWindow::initSignalSlot()
{
//    connect(ui->tabWidget, &QTabWidget::tabCloseRequested,
//            this, &MainWindow::onTabPageCloseRequested);
//    connect(ui->lineEdit, &QLineEdit::returnPressed, [this]()
//    {
//        auto url = UtilQt::check_url(ui->lineEdit->text());
//        auto page = GetActivePage();
//        if(page){
//            page->Navigate(url);
//        }

//    });
}

void MainWindow::initPage(CefQWidget *page)
{
    connect(page, &CefQWidget::browserClosing, [this](CefQWidget *page)
    {
        auto index = stack_browsers_->indexOf(page);
//        stack_browsers_->removeTab(index);
        // 必须要删除才能真正的释放cef的browser
        page->deleteLater();
        // 关掉一个以后，紧接着判断是不是用户在关闭整个窗口
        if(closing_){
            close();
        }
    });

    // 顶层窗口的窗口状态改变应该通知到page去，
    connect(this, &MainWindow::windowStateChanged, page, &CefQWidget::onTopLevelWindowStateChanged);

//    connect(page, &CefQWidget::browserAddressChange, [this](const QString &address)
//    {
//        ui->lineEdit->setText(QUrl(address).toDisplayString());
//    });

    connect(page, &CefQWidget::browserTitleChange, [this, page](const QString &title)
    {
        auto index = stack_browsers_->indexOf(page);
        tab_bar_->setTabText(index, title);

    });
    connect(page, &CefQWidget::browserNewForgroundPage, [this](CefQWidget *newPage)
    {
//        auto index = ui->tabWidget->addTab(newPage, "");
//        ui->tabWidget->setCurrentIndex(index);
//        initPage(newPage);
    });

}

CefQWidget *MainWindow::GetActivePage()
{
    auto widget = stack_browsers_->currentWidget();
    if(widget){
        return qobject_cast<CefQWidget *>(widget);
    }
    return nullptr;
}

void MainWindow::onTabPageCloseRequested(int index)
{
    if(stack_browsers_->count() == 1){
        closing_ = true;
    }
    auto page = stack_browsers_->widget(index);
    if(page){
        page->close();
    }
}
