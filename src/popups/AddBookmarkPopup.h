#ifndef ADDBOOKMARKPOPUP_H
#define ADDBOOKMARKPOPUP_H

#include "PopupBase.h"

namespace Ui {
class AddBkmkPopup;
}
class AddBkmkPopup : public PopupBase
{
public:
    AddBkmkPopup(QWidget *parent = nullptr);

private:
    Ui::AddBkmkPopup *ui;

private:
    void onBtnMoreClicked();
    void onBtnFinishClicked();
    void noBtnDeleteClicked();
};

#endif // ADDBOOKMARKPOPUP_H
