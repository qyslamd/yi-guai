#include "NotificationBar.h"
#include "ui_NotificationBar.h"

#include <QGraphicsDropShadowEffect>
#include <QPainter>

NotificationBar::NotificationBar(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::NotificationBar)
{
    ui->setupUi(this);
    connect(ui->btnClose, &QToolButton::clicked, this, &QFrame::close);
}

NotificationBar::~NotificationBar()
{
    delete ui;
}

void NotificationBar::paintEvent(QPaintEvent *event)
{
    QFrame::paintEvent(event);

    QPainter p(this);
    p.save();
    qreal penWidth = 0.5f;
    QColor color((QLatin1String("#D2D2D2")));
    p.setPen(QPen(QBrush(color), 0.5f));
    p.drawLine(0.0, height() - penWidth, width(), height() - penWidth);
    p.restore();
}
