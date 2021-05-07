#include "cef_qwidget.h"

#ifdef Q_OS_WIN
#include <Windows.h>
#endif

#include <QtDebug>
#include <QThread>
#include <QHBoxLayout>
#include <QCloseEvent>
#include <QTimer>
#include <QDateTime>

#include <include/base/cef_logging.h>
#include "mainwindow.h"

CefQWidget::CefQWidget(const QString &startup_url, QWidget *parent)
    : QWidget(parent)
    , window_(new QWindow)
    , qwindow_containter_(nullptr)
    , layout_(new QHBoxLayout(this))
{
    browser_window_.reset(new BrowserWindow(this, startup_url.toStdString()));
    initUi();

    auto handle = (HWND)window_->winId();
    CefRect rect{x(), y(), width(), height()};  // 给个初始范围，不然浏览器创建完成后的移动窗口会出现黑色背景
    CefBrowserSettings browser_settings;
    browser_window_->CreateBrowser(handle,
                                   rect,
                                   browser_settings,
                                   nullptr,
                                   nullptr);
}

CefQWidget::CefQWidget(CefWindowInfo &windowInfo,
                       CefRefPtr<CefClient> &client,
                       CefBrowserSettings &settings,
                       QWidget *parent)
    : QWidget(parent)
    , window_(new QWindow)
    , qwindow_containter_(nullptr)
    , layout_(new QHBoxLayout(this))
{
    browser_window_.reset(new BrowserWindow(this, ""));
    initUi();

    auto handle = (HWND)window_->winId();
    browser_window_->GetPopupConfig(handle, windowInfo, client, settings);
}

CefQWidget::~CefQWidget()
{
    qInfo()<<__FUNCTION__;
}

void CefQWidget::Navigate(const QString &url)
{
    qInfo()<<__FUNCTION__<<url;
    auto browser = browser_window_->GetBrowser();
    if(browser){
        browser->GetMainFrame()->LoadURL(url.toStdString());
    }
}

void CefQWidget::onTopLevelWindowStateChanged(Qt::WindowStates state, const QVariant &data)
{
    if(state.testFlag(Qt::WindowMaximized) || state.testFlag(Qt::WindowNoState)){
        resizeBorser(data.toSize());
    }
}

void CefQWidget::onBrowserWindowNewForgroundPage(CefWindowInfo &windowInfo,
                                                 CefRefPtr<CefClient> &client,
                                                 CefBrowserSettings &settings)
{
    CefQWidget *window = new CefQWidget(windowInfo, client, settings);

    emit browserNewForgroundPage(window);
}

void CefQWidget::OnBrowserCreated()
{
    resizeBorser();
}

void CefQWidget::OnBrowserWindowClosing()
{
    emit browserClosing();
}

void CefQWidget::onBrowserWindowAddressChange(const std::string &url)
{
    emit browserAddressChange(QString::fromStdString(url));
}

void CefQWidget::onBrowserWindowTitleChange(const std::string &title)
{
    emit browserTitleChange(QString::fromStdString(title));
}

void CefQWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    resizeBorser();
}

void CefQWidget::closeEvent(QCloseEvent *event)
{
    // 关闭浏览器触发点
    if(browser_window_ && !browser_window_->IsClosing()){
        qInfo()<<__FUNCTION__;
        auto browser = browser_window_->GetBrowser();
        if(browser){
            // Notify the browser window that we would like to close it. This
            // will result in a call to ClientHandler::DoClose() if the
            // JavaScript 'onbeforeunload' event handler allows it.
            browser->GetHost()->CloseBrowser(false);
        }
        // Cancel the close.
        event->ignore();
    }
}

void CefQWidget::initUi()
{
    window_->setFlag(Qt::FramelessWindowHint, true);

    if(!qwindow_containter_){
        qwindow_containter_ = QWidget::createWindowContainer(window_, this, Qt::Widget);
    }

    layout_->setContentsMargins(0,0,0,0);
    layout_->setSpacing(0);
    layout_->addWidget(qwindow_containter_);
    setLayout(layout_);
}

void CefQWidget::resizeBorser(const QSize &size)
{
    QRect rect;
    if(size.isEmpty()){
        rect = qwindow_containter_->rect();
    }else{
        rect.setX(0);
        rect.setY(0);
        rect.setWidth(size.width());
        rect.setHeight(size.height());
    }
    auto browser = browser_window_->GetBrowser();
    if(browser){
        HWND wnd = browser->GetHost()->GetWindowHandle();
        ::MoveWindow(wnd, rect.x(), rect.y(), rect.width(), rect.height(), false);
    }
}
