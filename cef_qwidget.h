﻿#ifndef CEF_QWIDGET_H
#define CEF_QWIDGET_H

#include <memory>

#include <QWidget>
#include <QWindow>
#include "browser/browser_window.h"

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

    void Navigate(const QString &url);

signals:
    void browserClosing(CefQWidget *page);
    void browserNewForgroundPage(CefQWidget *window);
    void browserAddressChange(const QString &url);
    void browserTitleChange(const QString &title);

public slots:
    void onTopLevelWindowStateChanged(Qt::WindowStates state, const QVariant &data);

    // BrowserWindow::Delegate interface
protected:
    void onBrowserWindowNewForgroundPage(CefWindowInfo &windowInfo,
                                              CefRefPtr<CefClient> &client,
                                              CefBrowserSettings &settings) override;
    void OnBrowserCreated() override;
    void OnBrowserWindowClosing() override;
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
    void resizeBorser(const QSize &size = QSize());

};

#endif // CEF_QWIDGET_H
