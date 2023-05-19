#ifndef INPRIVATEPOPUP_H
#define INPRIVATEPOPUP_H

#include "PopupBase.h"

namespace Ui {
class InprivatePopup;
}

class InprivatePopup : public PopupBase
{
    Q_OBJECT
public:
    InprivatePopup(QWidget *parent = nullptr);
    ~InprivatePopup();
    void setHintText(const QString &text);
private:
    Ui::InprivatePopup *ui;

private Q_SLOTS:
    void onCloseAllInprivateWnds();
};

#endif // INPRIVATEPOPUP_H
