#include "ButtonLineEdit.h"
#include <QGraphicsDropShadowEffect>
#include <QEvent>
#include <QtDebug>

ButtonLineEdit::ButtonLineEdit(QWidget *parent)
    : QFrame(parent)
{
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    shadow->setOffset(0, 0);
    shadow->setColor(Qt::gray);
    shadow->setBlurRadius(10);
    setGraphicsEffect(shadow);

    layout_ = new QHBoxLayout;
    layout_->setContentsMargins(8,2,8,2);
    layout_->setSpacing(1);
    btnSiteInfo_ = new QPushButton;
    lineEdit_ = new QLineEdit;
    lineEdit_->setMinimumHeight(30);
    lineEdit_->setStyleSheet("QLineEdit{border:none;}");
    lineEdit_->installEventFilter(this);
    btnZoom_ = new QToolButton;
    btnMark_ = new QToolButton;

    layout_->addWidget(btnSiteInfo_);
    layout_->addWidget(lineEdit_);
    layout_->addWidget(btnZoom_);
    layout_->addWidget(btnMark_);
    setLayout(layout_);

    setStyleSheet(".ButtonLineEdit{"
"border:1px solid gray;"
"border-radius:15px;"
"background-color:white;"
"}"
".ButtonLineEdit:hover{"
"border:1px solid blue;"
"}"
".ButtonLineEdit:focus{"
"border:1px solid SkyBlue;"
"}");

    setFocusProxy(lineEdit_);

}

bool ButtonLineEdit::eventFilter(QObject *obj, QEvent *ev)
{
    if(obj == lineEdit_){
        if(ev->type() == QEvent::FocusIn || ev->type() == QEvent::FocusOut){
            update();
        }
    }
    return QFrame::eventFilter(obj,ev);
}
