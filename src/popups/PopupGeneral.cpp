#include "PopupGeneral.h"

#include <QVBoxLayout>

PopupGeneral::PopupGeneral(QWidget *parent)
    : PopupBase(parent)
{
    layout_ = new QVBoxLayout;
    layout_->setContentsMargins(0,0,0,0);
    layout_->setSpacing(0);
    contentFrame()->setLayout(layout_);
}

PopupGeneral::~PopupGeneral()
{

}

void PopupGeneral::setWidget(QWidget *widget)
{
    layout_->addWidget(widget);
}
