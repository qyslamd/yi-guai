#include "BookmarkBar.h"

#include <QPaintEvent>
#include <QPainter>
#include <QMenu>
#include <QAction>
#include <QElapsedTimer>


#include "managers/BookmarkManager.h"

BookmarkBar::BookmarkBar(QWidget *parent)
    : QFrame(parent)
{
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

