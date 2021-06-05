#ifndef LISTTABSTYLE_H
#define LISTTABSTYLE_H

#include <QProxyStyle>

class ListTabStyle : public QProxyStyle
{
public:
    ListTabStyle();
    QSize sizeFromContents(ContentsType type, const QStyleOption *option,
            const QSize &size, const QWidget *widget) const override;

    void drawControl(ControlElement element,
                     const QStyleOption *opt,
                     QPainter *p,
                     const QWidget *w) const override;
};

#endif // LISTTABSTYLE_H
