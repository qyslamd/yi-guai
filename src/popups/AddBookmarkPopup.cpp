#include "AddBookmarkPopup.h"
#include "ui_AddBookmarkPopup.h"

AddBkmkPopup::AddBkmkPopup(QWidget *parent)
    : PopupBase(parent)
    , ui(new Ui::AddBkmkPopup)
{
    ui->setupUi(contentFrame());
    connect(ui->btnClose, &QToolButton::clicked, this, &AddBkmkPopup::close);
    connect(ui->btnMore, &QPushButton::clicked, this, &AddBkmkPopup::onBtnMoreClicked);
    connect(ui->btnFinish, &QPushButton::clicked, this, &AddBkmkPopup::onBtnFinishClicked);
    connect(ui->btnRemove, &QPushButton::clicked, this, &AddBkmkPopup::onBtnFinishClicked);
}

void AddBkmkPopup::onBtnMoreClicked()
{

}

void AddBkmkPopup::onBtnFinishClicked()
{

}

void AddBkmkPopup::noBtnDeleteClicked()
{

}
