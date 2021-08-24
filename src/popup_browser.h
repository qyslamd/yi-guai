#ifndef POPUP_BROWSER_H
#define POPUP_BROWSER_H

#include <QWidget>


namespace Ui {
class PopupBrowser;
}

class QAction;
class SiteInfoPopup;
class QToolButton;
class CefQWidget;
class PopupBrowser : public QWidget
{
    Q_OBJECT

public:
    explicit PopupBrowser(CefQWidget *browser, QWidget *parent = nullptr);
    ~PopupBrowser();
private:
    Ui::PopupBrowser *ui;

    QAction *action_site_info_;
    SiteInfoPopup *site_info_widget_;
    CefQWidget *browser_;
    QToolButton *internal_btn_siteInfo_ = nullptr;

private slots:
    void onSiteInfoClicked();
    void onBrowserTitleChange(const QString &text);
    void onBrowserAddrChange(const QString &text);
    void onBrowserFavicon(const QPixmap &pix);
};

#endif // POPUP_BROWSER_H
