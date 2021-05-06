#include "navibar.h"
#include "addressbar.h"

#include <QHBoxLayout>
#include <QToolButton>

NaviBar::NaviBar(QWidget *parent)
    : QFrame(parent)
    , layout_(new QHBoxLayout)
    , btn_back_(new QToolButton)
    , btn_forward_(new QToolButton)
    , btn_refresh_stop_(new QToolButton)
    , btn_home_(new QToolButton)
    , btn_user_(new QToolButton)
    , btn_other_options_(new QToolButton)
    , address_bar_(new AddressBar)
    , frame_extensions_(new QFrame)
    , frame_tools_(new QFrame)

{
    layout_->addWidget(btn_back_);
    layout_->addWidget(btn_forward_);
    layout_->addWidget(btn_refresh_stop_);
    layout_->addWidget(btn_home_);
    layout_->addWidget(address_bar_);
    layout_->addWidget(frame_extensions_);
    layout_->addWidget(frame_tools_);
    layout_->addWidget(btn_user_);
    layout_->addWidget(btn_other_options_);

    setLayout(layout_);
}

void NaviBar::setSpacing(int spacing)
{
    if(spacing < 0){
       layout_->setSpacing(0);
    }else{
       layout_->setSpacing(spacing);
    }
}
