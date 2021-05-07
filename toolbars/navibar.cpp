#include "navibar.h"
#include "addressbar.h"
#include "utils/util_qt.h"

#include <QHBoxLayout>
#include <QToolButton>
#include <QPainter>

NaviBar::NaviBar(QWidget *parent)
    : QFrame(parent)
    , layout_(new QHBoxLayout)
    , btn_back_(new QToolButton)
    , btn_refresh_(new QToolButton)
    , btn_stop_(new QToolButton)
    , btn_forward_(new QToolButton)
    , btn_home_(new QToolButton)
    , btn_user_(new QToolButton)
    , btn_more_options_(new QToolButton)
    , address_bar_(new AddressBar)
    , frame_extensions_(new QFrame)
    , frame_tools_(new QFrame)

{
    layout_->addWidget(btn_back_);
    layout_->addWidget(btn_forward_);
    layout_->addWidget(btn_refresh_);
    layout_->addWidget(btn_stop_);
    layout_->addWidget(btn_home_);
    layout_->addWidget(address_bar_);
    layout_->addWidget(frame_extensions_);
    layout_->addWidget(frame_tools_);
    layout_->addWidget(btn_user_);
    layout_->addWidget(btn_more_options_);

    setLayout(layout_);
    layout_->setContentsMargins(4,4,4,5);

    initSignals();

    setAppearance();
}

void NaviBar::setSpacing(int spacing)
{
    if(spacing < 0){
       layout_->setSpacing(0);
    }else{
       layout_->setSpacing(spacing);
    }
}

void NaviBar::paintEvent(QPaintEvent *event)
{
    QFrame::paintEvent(event);

    QPainter p(this);
    p.save();
    qreal penWidth = 0.5f;
    p.setPen(QPen(QBrush(QColor("#D2D2D2")), 0.5f));
    p.drawLine(0.0, height() - penWidth, width(), height() - penWidth);
    p.restore();
}

void NaviBar::initSignals()
{
    connect(address_bar_, &AddressBar::returnPressed, [this]()
    {
        emit naviBarCmd(NaviBarCmd::Navigate, address_bar_->text());
    });
}

void NaviBar::setAppearance()
{
    btn_back_->setIcon(QIcon(":/icons/resources/normal_back.png"));
    btn_refresh_->setIcon(QIcon(":/icons/resources/normal_refresh.png"));
    btn_stop_->setIcon(QIcon(":/icons/resources/window_close_light_2x.png"));
    btn_forward_->setIcon(QIcon(":/icons/resources/normal_forward.png"));
    btn_home_->setIcon(QIcon(":/icons/resources/home_blabk_64px.png"));
    btn_user_->setIcon(QIcon(":/icons/resources/user_64px.png"));
    btn_more_options_->setIcon(QIcon(":/icons/resources/normal_more.png"));

    QSize iconSize(26,26);
    for(auto item : this->children()){
        if(item->isWidgetType() &&
                item->metaObject()->className() == QString("QToolButton"))
        {
            auto btn = qobject_cast<QToolButton*>(item);
            btn->setIconSize(iconSize);
        }
    }
}
