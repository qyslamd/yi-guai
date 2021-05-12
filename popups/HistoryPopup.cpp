#include "HistoryPopup.h"
#include "ui_HistoryPopup.h"

HistoryPopup::HistoryPopup(QWidget *parent) :
    PopupBase(parent),
    ui(new Ui::HistoryPopup)
{
    ui->setupUi(contentFrame());
}

HistoryPopup::~HistoryPopup()
{
    delete ui;
}
