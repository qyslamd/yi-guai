#include "SearchBar.h"
#include <QHBoxLayout>
#include <QToolButton>
#include <QLineEdit>
#include <QGraphicsDropShadowEffect>
#include <QEvent>

SearchBar::SearchBar(bool inprivate, QWidget *parent)
    : QFrame(parent)
    , inprivate_(inprivate)
{
    initUi();
    setIcons();
}

bool SearchBar::eventFilter(QObject *obj, QEvent *ev)
{
    if(obj == line_edit_){
        if(ev->type() == QEvent::FocusIn ||
                ev->type() == QEvent::FocusOut){
            update();
        }
    }
    return QFrame::eventFilter(obj, ev);
}

void SearchBar::initUi()
{
    using ShaowEffect = QGraphicsDropShadowEffect;
    ShaowEffect *shadow = new ShaowEffect(this);
    shadow->setOffset(0, 0);
    shadow->setColor(Qt::gray);
    shadow->setBlurRadius(6);
    setGraphicsEffect(shadow);

//    setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    setMaximumWidth(180);

    layout_ = new QHBoxLayout;
    layout_->setContentsMargins(2, 2, 2, 2);
    layout_->setSpacing(1);

    btn_search_engine_ = new QToolButton(this);
    btn_search_engine_->setToolTip(tr("search engine"));

    line_edit_ = new QLineEdit(this);
    line_edit_->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    line_edit_->installEventFilter(this);

    btn_search_ = new QToolButton(this);
    btn_search_->setToolTip(tr("search"));

    layout_->addWidget(btn_search_engine_);
    layout_->addWidget(line_edit_);
    layout_->addWidget(btn_search_);
    setLayout(layout_);

    setFocusProxy(line_edit_);
}

void SearchBar::setIcons()
{
    btn_search_engine_->setIcon(QIcon(":/icons/resources/imgs/colorful/bing_96px.png"));
    if(inprivate_){
        btn_search_->setIcon(QIcon(":/icons/resources/imgs/dark/search_64px.png"));
    }else{
        btn_search_->setIcon(QIcon(":/icons/resources/imgs/ios7/search_50px.png"));
    }
}
