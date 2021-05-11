#ifndef POPUPBROWSERWIDGET_H
#define POPUPBROWSERWIDGET_H

#include <QWidget>


namespace Ui {
class PopupBrowserWidget;
}

class QAction;
class SiteInfoPopup;
class QToolButton;
class CefQWidget;
class PopupBrowserWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PopupBrowserWidget(CefQWidget *browser, QWidget *parent = nullptr);
    ~PopupBrowserWidget();
private:
    Ui::PopupBrowserWidget *ui;

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

#endif // POPUPBROWSERWIDGET_H
