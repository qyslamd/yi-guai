#ifndef TABBAR_H
#define TABBAR_H

#include <QTabBar>

class TabBar final: public QTabBar
{
    Q_OBJECT
public:
    TabBar(QWidget *parent = nullptr);

protected:
    // QTabBar interface
    QSize tabSizeHint(int index) const override;
    QSize minimumTabSizeHint(int index) const override;
};

#endif // TABBAR_H
