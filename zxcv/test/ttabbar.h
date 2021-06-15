#ifndef TTABBAR_H
#define TTABBAR_H

#include <QTabBar>

class TTabBar : public QTabBar
{
    Q_OBJECT
public:
    TTabBar(QWidget *parent = nullptr);
protected:
    // QTabBar interface
    QSize tabSizeHint(int index) const override;
    QSize minimumTabSizeHint(int index) const override;
    void wheelEvent(QWheelEvent *event) override;
};

#endif // TTABBAR_H
