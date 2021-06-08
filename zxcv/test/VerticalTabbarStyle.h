#ifndef VERTICALTABBARSTYLE_H
#define VERTICALTABBARSTYLE_H

#include <QProxyStyle>

class VerticalTabbarStyle : public QProxyStyle
{
public:
    VerticalTabbarStyle();
    QSize sizeFromContents(ContentsType type, const QStyleOption *option,
            const QSize &size, const QWidget *widget) const override;

    void drawControl(ControlElement element,
                     const QStyleOption *opt,
                     QPainter *p,
                     const QWidget *w) const override;

    QRect subElementRect(SubElement subElement,
                         const QStyleOption *option,
                         const QWidget *widget) const override;

private:
    void drawRectShadow(QPainter *p, const QRect &rect,
                        const int shadowWidth) const;
};

#endif // VERTICALTABBARSTYLE_H
