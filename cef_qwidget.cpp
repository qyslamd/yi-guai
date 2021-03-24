#include "cef_qwidget.h"

#ifdef Q_OS_WIN
#include <Windows.h>
#endif

#include <QtDebug>
#include <QThread>
#include <QHBoxLayout>
#include <QCloseEvent>
#include <QTimer>

CefQWidget::CefQWidget(const QString &startup_url, QWidget *parent)
    : QWidget(parent)
    , window_(new QWindow)
    , qwindow_containter_(nullptr)
    , layout_(new QHBoxLayout(this))
{
    browser_window_.reset(new BrowserWindow(this, startup_url.toStdString()));
    initUi();

    auto handle = (HWND)window_->winId();
    CefRect rect{};
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

void CefQWidget::onBrowserWindowNewForgroundPage(CefWindowInfo &windowInfo,
                                                 CefRefPtr<CefClient> &client,
                                                 CefBrowserSettings &settings)
{
    CefQWidget *window = new CefQWidget(windowInfo, client, settings);

    emit browserNewForgroundPage(window);
}

void CefQWidget::OnBrowserCreated(CefRefPtr<CefBrowser> browser)
{
    resizeBorser();
}

void CefQWidget::OnBrowserWindowClosing(CefRefPtr<CefBrowser> browser)
{
    if(browser_window_ && browser_window_->IsClosing())
    {
        qInfo()<<__FUNCTION__;
        browser_window_->onBrowserComfirmClose();
        // 关键问题是释放引用计数
        this->deleteLater();
    }
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
    if(browser_window_ && !browser_window_->IsClosing()){
        qInfo()<<__FUNCTION__<<"browser closing";
        browser_window_->closeBrowser(false);
        event->ignore();
    }
}

void CefQWidget::initUi()
{
    window_->setFlag(Qt::FramelessWindowHint, true);

    if(!qwindow_containter_){
        qwindow_containter_ = QWidget::createWindowContainer(window_);
    }

    layout_->setContentsMargins(0,0,0,0);
    layout_->setSpacing(0);
    layout_->addWidget(qwindow_containter_);
    setLayout(layout_);
}

void CefQWidget::resizeBorser()
{
    auto browser = browser_window_->GetBrowser();
    auto rect = qwindow_containter_->rect();
    if(browser){
        HWND wnd = browser->GetHost()->GetWindowHandle();
        ::MoveWindow(wnd, rect.x(), rect.y(), rect.width(), rect.height(), true);
    }
}
