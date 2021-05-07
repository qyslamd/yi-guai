#include "tabbar.h"
#include "tabbarstyle.h"

TabBar::TabBar(QWidget *parent)
    : QTabBar(parent)
{
    setDrawBase(false);
    setTabsClosable(true);
    setStyle(new TabbarStyle(false));
}

QSize TabBar::tabSizeHint(int index) const
{
#ifdef Q_OS_WIN
    int dpi = this->logicalDpiX();
    switch (dpi) {
    case 96:
        return QSize(240, 34);
    case 120:
        return QSize(300, 40);
    case 144:
        return QSize(360, 50);
    case 168:
        return QSize(420, 58);
    default:
        break;
    }
    return QTabBar::tabSizeHint(index);
#else
    return QSize(240, 32);
#endif
}

QSize TabBar::minimumTabSizeHint(int index) const
{
    return QSize(36,36);
    Q_UNUSED(index)
}
