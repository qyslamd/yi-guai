#include "page.h"
#include "cef_qwidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDockWidget>
#include <QVariant>

Page::Page(const QString &startup_url, QWidget *parent)
    : QMainWindow(parent)
{
    initUi(startup_url);
}

CefQWidget* Page::getBrowserWidget()
{
    return  browser_widget_;
}

void Page::onTopLevelWindowStateChanged(Qt::WindowStates state,
                                        const QVariant &data)
{
    browser_widget_->onTopLevelWindowStateChanged(state, data);
}

void Page::closeEvent(QCloseEvent *event)
{
    browser_widget_->close();
    QMainWindow::closeEvent(event);
}

void Page::initUi(const QString &url)
{
    if(!centralWidget()){
        setCentralWidget(new QWidget);
    }
    main_layout_ = new QVBoxLayout;
    browser_widget_ = new CefQWidget(url, this);
    dock_dev_tool_ = new QDockWidget;

    main_layout_->addWidget(browser_widget_);
    centralWidget()->setLayout(main_layout_);

    main_layout_->setContentsMargins(0,0,0,0);
    main_layout_->setSpacing(0);

    connect(browser_widget_, &CefQWidget::browserClosing, this, &Page::onBrowserClosing);
}

void Page::onBrowserClosing()
{
   emit pageCmd(PageCmd::Closing, QVariant());
}
