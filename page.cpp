#include "page.h"
#include "cef_qwidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDockWidget>
#include <QVariant>

Page::Page(const QString &startup_url, QWidget *parent)
    : QMainWindow(parent)
    , main_layout_(new QVBoxLayout)
    , dock_dev_tool_(new QDockWidget)
{
    if(!centralWidget()){
        setCentralWidget(new QWidget);
    }
    browser_widget_ = new CefQWidget(startup_url, this);

    main_layout_->addWidget(browser_widget_);
    centralWidget()->setLayout(main_layout_);

    main_layout_->setContentsMargins(0,0,0,0);
    main_layout_->setSpacing(0);

    initBrowser();
}

Page::Page(CefQWidget*browser, QWidget *parent)
    : QMainWindow(parent)
    , main_layout_(new QVBoxLayout)
    , browser_widget_(browser)
    , dock_dev_tool_(new QDockWidget)
{
    if(!centralWidget()){
        setCentralWidget(new QWidget);
    }
    main_layout_->addWidget(browser_widget_);
    centralWidget()->setLayout(main_layout_);

    main_layout_->setContentsMargins(0,0,0,0);
    main_layout_->setSpacing(0);

    initBrowser();
}

CefQWidget* Page::getBrowserWidget()
{
    return browser_widget_;
}
void Page::closeEvent(QCloseEvent *event)
{
    browser_widget_->close();
    QMainWindow::closeEvent(event);
}

void Page::initBrowser()
{
    connect(browser_widget_, &CefQWidget::browserClosing, [this]()
    {
        emit pageCmd(PageCmd::Closing, QVariant());
    });
    connect(browser_widget_, &CefQWidget::browserNewForgroundPage, [this](CefQWidget *browser){
        Page *page = new Page(browser);
        emit newPage(page);
    });
    connect(browser_widget_, &CefQWidget::browserAddressChange, [this](const QString &url)
    {
         emit pageCmd(PageCmd::Address, url);
    });
    connect(browser_widget_, &CefQWidget::browserTitleChange, [this](const QString &title)
    {
         emit pageCmd(PageCmd::Title, title);
    });
    connect(browser_widget_, &CefQWidget::browserLoadingStateChange, [this](bool a, bool b, bool c)
    {
        isLoading_ = a;
        canGoBack_ = b;
        canGoForward_ = c;
        emit pageCmd(PageCmd::LoadingState, QVariant());
    });
}
