#ifndef ADDTOFAVORITEPOPUP_H
#define ADDTOFAVORITEPOPUP_H

#include "PopupBase.h"

namespace Ui {
class AddToFavoritePopup;
}
class AddToFavoritePopup : public PopupBase
{
public:
    AddToFavoritePopup(QWidget *parent = nullptr);

private:
    Ui::AddToFavoritePopup *ui;
};

#endif // ADDTOFAVORITEPOPUP_H
