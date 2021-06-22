#include "FullscnHint.h"
#include "ui_FullscnHint.h"
#include <QPaintEvent>
#include <QPainter>
#include <QtDebug>

FullscnHint::FullscnHint(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::FullscnHint)
{
    ui->setupUi(this);
    setWindowFlags(Qt::ToolTip | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
}

FullscnHint::~FullscnHint()
{
    qInfo()<<__FUNCTION__;
    delete ui;
}

void FullscnHint::setKeyStr(const QString &str)
{
    ui->labelKeyStr->setText(str);
}

void FullscnHint::paintEvent(QPaintEvent *e)
{
    QFrame::paintEvent(e);

    QPainter p(this);
    p.save();
    QColor color(Qt::black);
    color.setAlphaF(0.6);
    p.setPen(Qt::NoPen);
    p.setBrush(color);
    p.drawRoundedRect(this->rect(),4.0,4.0);
    p.restore();
}
