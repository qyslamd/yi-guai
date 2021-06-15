#include "ttabbar.h"

#include <QtDebug>

TTabBar::TTabBar(QWidget *parent)
    : QTabBar(parent)
{
    setDrawBase(false);
    setShape(QTabBar::RoundedWest);
    setTabsClosable(true);
    setMovable(true);
    setUsesScrollButtons(false);

//    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
//    qInfo()<<this->sizePolicy();
}

QSize TTabBar::tabSizeHint(int index) const
{
//    return QTabBar::tabSizeHint(index);
//    if(index == 0){
//        return QSize(34,40);
//    }
    return QSize(240, 40);
}

QSize TTabBar::minimumTabSizeHint(int index) const
{
//    return QTabBar::minimumTabSizeHint(index);
    return QSize(36,40);
}

void TTabBar::wheelEvent(QWheelEvent *event)
{
    return QWidget::wheelEvent(event);
}

