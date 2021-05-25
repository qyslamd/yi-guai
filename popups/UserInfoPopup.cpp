#include "UserInfoPopup.h"
#include "ui_UserInfoPopup.h"

UserInfoPopup::UserInfoPopup(QWidget *parent) :
    PopupBase(parent),
    ui(new Ui::UserInfoPopup)
{
    ui->setupUi(contentFrame());
}

UserInfoPopup::~UserInfoPopup()
{
    delete ui;
}
