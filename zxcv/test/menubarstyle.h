#ifndef MENUBARSTYLE_H
#define MENUBARSTYLE_H

#include <QProxyStyle>

class MenuBarStyle : public QProxyStyle
{
public:
    MenuBarStyle(QStyle *style = nullptr);

    // QStyle interface
public:
    QSize sizeFromContents(ContentsType ct, const QStyleOption *opt, const QSize &contentsSize, const QWidget *w) const override;
    void drawControl(ControlElement element, const QStyleOption *opt, QPainter *p, const QWidget *w) const override;
};

#endif // MENUBARSTYLE_H
