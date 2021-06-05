#include "InprivatePopup.h"
#include "ui_InprivatePopup.h"

#include <QtDebug>

#include "managers/MainWindowManager.h"

InprivatePopup::InprivatePopup(QWidget *parent)
    : PopupBase(parent)
    , ui(new Ui::InprivatePopup)
{
    ui->setupUi(contentFrame());
    resize(320, 170);
    ui->btnCloseAll->setObjectName("InprivateCloseBtn");
    connect(ui->btnCloseAll, &QPushButton::clicked,this, &InprivatePopup::onCloseAllInprivateWnds);
}

InprivatePopup::~InprivatePopup()
{
    qInfo()<<__FUNCTION__;
}

void InprivatePopup::setHintText(const QString &text)
{
    ui->labelHint->setText(text);
}

void InprivatePopup::onCloseAllInprivateWnds()
{
    MainWndMgr::Instance().closeAllInprivate();
    hide();
}
