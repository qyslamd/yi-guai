#include "ttabbar.h"

#include <QtDebug>

TTabBar::TTabBar(QWidget *parent)
    : QTabBar(parent)
{
    setDrawBase(false);
    setShape(QTabBar::RoundedWest);
    setTabsClosable(true);
    setMovable(true);
    setUsesScrollButtons(true);

    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
//    qInfo()<<this->sizePolicy();
}

QSize TTabBar::tabSizeHint(int index) const
{
//    return QTabBar::tabSizeHint(index);
    if(index == 0){
        return QSize(34,34);
    }
    return QSize(240, 34);
}

QSize TTabBar::minimumTabSizeHint(int index) const
{
//    return QTabBar::minimumTabSizeHint(index);
    return QSize(36,34);
}
