#include "widget.h"
#include "ui_FrameCaption.h"

#include <QTabBar>
#include <QVBoxLayout>
#include <QStackedWidget>

Widget::Widget(QWidget *parent) :
    QWidget(parent)
  , layout_(new QVBoxLayout(this))
  , frame_caption(new QFrame)
  , ui_frame_caption_(new Ui::FrameCaption)
{
    ui_frame_caption_->setupUi(frame_caption);

    auto tabbar = new QTabBar;
    ui_frame_caption_->tabbarLayout->insertWidget(1,tabbar);

    tabbar->addTab("ABCD");
    tabbar->addTab("456465");
    tabbar->addTab("46546");

    layout_->setContentsMargins(0,0,0,0);

    layout_->addWidget(frame_caption, 1);
    auto stacked = new QStackedWidget;
    layout_->addWidget(stacked, 9);

    stacked->addWidget(new QWidget);
    stacked->addWidget(new QWidget);
    stacked->addWidget(new QWidget);
}

Widget::~Widget()
{
    delete  ui_frame_caption_;
}
