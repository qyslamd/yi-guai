#include "page.h"
#include "cef_qwidget.h"
#include "popups/SiteInfoPopup.h"

#include <QtDebug>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDockWidget>
#include <QVariant>
#include <QUrl>
#include <QStyle>

Page::Page(const QString &url, QWidget *parent)
    : QMainWindow(parent)
    , main_layout_(new QVBoxLayout)
    , dock_dev_tool_(new QDockWidget)
    , site_info_popup_(new SiteInfoPopup(this))
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
}

Page::Page(CefQWidget*browser, QWidget *parent)
    : QMainWindow(parent)
    , main_layout_(new QVBoxLayout)
    , browser_widget_(browser)
    , dock_dev_tool_(new QDockWidget)
    , site_info_popup_(new SiteInfoPopup(this))
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
}

CefQWidget* Page::getBrowserWidget()
{
    return browser_widget_;
}

void Page::showSiteInfomation(const QRect &rect)
{
    /*设置必要的信息*/
    site_info_popup_->setTitle(tr("View ") + QUrl(url_).host());

    auto pos = rect.topLeft();
    pos.ry() += rect.height();
    site_info_popup_->move(pos);
    site_info_popup_->show();
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
        url_ = url;
        emit pageCmd(PageCmd::Address, url);
    });
    connect(browser_widget_, &CefQWidget::browserTitleChange, [this](const QString &title)
    {
        title_ = title;
        emit pageCmd(PageCmd::Title, title);
    });
    connect(browser_widget_, &CefQWidget::browserStatusMessage, [this](const QString &msg)
    {
        emit pageCmd(PageCmd::StatusMessage, msg);
    });
    connect(browser_widget_, &CefQWidget::browserLoadStart, [this](CefLoadHandler::TransitionType transition_type)
    {
        emit pageCmd(PageCmd::LoadStart, (int)transition_type);
    });
    connect(browser_widget_, &CefQWidget::browserLoadEnd, [this](int httpStatusCode)
    {
        const QStringList customSchemes{QString("about"),"chrome"};
        auto parts = url_.split(":");
        if(parts.count()  > 0){
            auto scheme = parts.at(0).toLower();
            if(customSchemes.contains(scheme)){
                emit pageCmd(PageCmd::Favicon, style()->standardPixmap(QStyle::SP_MessageBoxInformation));
            }
        }

        if(url_.startsWith("file://",Qt::CaseInsensitive)){
            emit pageCmd(PageCmd::Favicon, style()->standardPixmap(QStyle::SP_FileIcon));
        }
        emit pageCmd(PageCmd::LoadEnd, httpStatusCode);
    });
    connect(browser_widget_, &CefQWidget::browserLoadingStateChange, [this](bool a, bool b, bool c)
    {
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
}
