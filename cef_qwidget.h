#ifndef CEF_QWIDGET_H
#define CEF_QWIDGET_H

#include <memory>

#include <QWidget>
#include <QWindow>
#include "browser_window.h"

class QHBoxLayout;
class CefQWidget
        : public QWidget
        , public BrowserWindow::Delegate
{
    Q_OBJECT
public:
    explicit CefQWidget(const QString &startup_url, QWidget *parent = nullptr);
    CefQWidget(CefWindowInfo &windowInfo,
               CefRefPtr<CefClient> &client,
               CefBrowserSettings &settings,
               QWidget *parent = nullptr);
    ~CefQWidget();
signals:
    void browserNewForgroundPage(CefQWidget *window);
    void browserAddressChange(const QString &url);
    void browserTitleChange(const QString &title);

    // BrowserWindow::Delegate interface
protected:
    void onBrowserWindowNewForgroundPage(CefWindowInfo &windowInfo,
                                              CefRefPtr<CefClient> &client,
                                              CefBrowserSettings &settings) override;
    void OnBrowserCreated(CefRefPtr<CefBrowser> browser) override;
    void OnBrowserWindowClosing(CefRefPtr<CefBrowser> browser) override;
//    void OnBrowserWindowDestroyed(CefRefPtr<CefBrowser> browser) override;
    void onBrowserWindowAddressChange(const std::string &url) override;
    void onBrowserWindowTitleChange(const std::string &title) override;

protected:
    void resizeEvent(QResizeEvent *event) override;
    void closeEvent(QCloseEvent *event) override;

private:
    void initUi();
    QWindow *window_;
    scoped_ptr<BrowserWindow> browser_window_;
    QWidget *qwindow_containter_;
    QHBoxLayout *layout_;

private:
    void resizeBorser();

};

#endif // CEF_QWIDGET_H
