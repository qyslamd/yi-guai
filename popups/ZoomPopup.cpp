#include "ZoomPopup.h"
#include "ui_ZoomPopup.h"

ZoomPopup::ZoomPopup(QWidget *parent) :
    PopupBase(parent),
    ui(new Ui::ZoomPopup)
{
    ui->setupUi(contentFrame());
    resize(260,50);
    QSize size(26,26);
    ui->btnZoomOut->setMinimumSize(size);
    ui->btnZoomIn->setMinimumSize(size);
    ui->btnReset->setMinimumWidth(65);
}

ZoomPopup::~ZoomPopup()
{
    delete ui;
}

void ZoomPopup::setZoomLevelStr(const QString &str)
{
    ui->lineEdit->setText(str);
}
