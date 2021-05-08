#ifndef SITEINFOPOPUP_H
#define SITEINFOPOPUP_H

#include <QMenu>
class QWidgetAction;
class QWidget;
class QLabel;
class QToolButton;

class SiteInfoPopup : public QMenu
{
    Q_OBJECT

public:
    explicit SiteInfoPopup(QWidget *parent = nullptr);
    ~SiteInfoPopup();

    void setTitle(const QString &title);

private:
    QWidgetAction *action_title_;
    QLabel *label_title_;
protected:
    void showEvent(QShowEvent *event) override;

};

#endif // SITEINFOPOPUP_H
