#ifndef USERINFOPOPUP_H
#define USERINFOPOPUP_H

#include <QWidget>
#include "PopupBase.h"

namespace Ui {
class UserInfoPopup;
}

class UserInfoPopup : public PopupBase
{
    Q_OBJECT

public:
    explicit UserInfoPopup(QWidget *parent = nullptr);
    ~UserInfoPopup();

    bool eventFilter(QObject *obj, QEvent *ve) override;

protected:
    void showEvent(QShowEvent *event) override;

private:
    Ui::UserInfoPopup *ui;
};

#endif // USERINFOPOPUP_H
