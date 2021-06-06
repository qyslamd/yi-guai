#include "AddToFavoritePopup.h"
#include "ui_AddToFavoritePopup.h"

AddToFavoritePopup::AddToFavoritePopup(QWidget *parent)
    : PopupBase(parent)
    , ui(new Ui::AddToFavoritePopup)
{
    ui->setupUi(contentFrame());
}
