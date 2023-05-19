#ifndef SITEINFOPOPUP_H
#define SITEINFOPOPUP_H

#include "PopupBase.h"
#include "globaldef.h"

namespace Ui {
class SiteInfoPopup;
}
class QPropertyAnimation;
class SiteInfoPopup : public PopupBase
{
    Q_OBJECT

public:
    explicit SiteInfoPopup(QWidget *parent = nullptr);
    ~SiteInfoPopup();
    bool eventFilter(QObject *obj, QEvent *ev) override;

    void setDomain(const QString &domain);
    void setLevel(SecurityLevel level);
signals:
    void openUrl(const QUrl &link);
protected:
    void showEvent(QShowEvent *ev) override;
    void hideEvent(QHideEvent *ev) override;
private:
    Ui::SiteInfoPopup *ui;
    QPropertyAnimation *animation;

    void showSiteDesc();

private:
    QString site_domain_;
    SecurityLevel level_;
};

#endif // SITEINFOPOPUP_H
