#include "addressbar.h"
#include "utils/util_qt.h"

#include <QHBoxLayout>
#include <QToolButton>

AddressBar::AddressBar(QWidget *parent)
    : QLineEdit(parent)
    , layout_(new QHBoxLayout)
    , btn_site_info_(new QToolButton)
    , btn_find_hint_(new QToolButton)
    , btn_zoom_hint_(new QToolButton)
    , btn_mark_site_(new QToolButton)
{
    layout_->addWidget(btn_site_info_);
    layout_->addStretch();   /*添加空格弹簧*/
    layout_->addWidget(btn_find_hint_);
    layout_->addWidget(btn_zoom_hint_);
    layout_->addWidget(btn_mark_site_);

    setLayout(layout_);
}

void AddressBar::setSpacing(int spacing)
{
    if(spacing < 0)
    {
        layout_->setSpacing(0);
    }else{
        layout_->setSpacing(spacing);
    }
}
