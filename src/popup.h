#ifndef POPUP_H
#define POPUP_H

#include <QWidget>


namespace Ui {
class Popup;
}

class QAction;
class SiteInfoPopup;
class QToolButton;
class CefQWidget;
class Popup : public QWidget
{
    Q_OBJECT

public:
    explicit Popup(CefQWidget *browser, QWidget *parent = nullptr);
    ~Popup();
private:
    Ui::Popup *ui;

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

#endif // POPUP_H
