#include "BookmarkBar.h"

#include <QPaintEvent>
#include <QPainter>

BookmarkBar::BookmarkBar(QWidget *parent)
    : QFrame(parent)
{
    setMinimumHeight(30);
}

void BookmarkBar::paintEvent(QPaintEvent *event)
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
