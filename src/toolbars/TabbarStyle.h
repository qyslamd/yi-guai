#ifndef TABBARSTYLE_H
#define TABBARSTYLE_H

#include <QProxyStyle>

class TabbarStyle final: public QProxyStyle
{
public:
    TabbarStyle( bool isInPrivate = false);
    virtual void drawControl(ControlElement element,
                             const QStyleOption *option,
                             QPainter *painter,
                             const QWidget *widget) const override;
    virtual QRect subElementRect(SubElement subElement,
                                 const QStyleOption *option,
                                 const QWidget *widget) const override;

protected:
    void drawTabBarTabLabel(const QStyleOption *option,
                            QPainter *painter,
                            const QWidget *w) const;

    //
    void drawTabBarTabShape(const QStyleOption *option,
                            QPainter *painter,
                            const QWidget *w) const;
private:
    bool isInprivate_;
    mutable double dpi_;


    QPainterPath getSelectedShape(const QStyleOption *option,
                                  const double scale) const;
    QPainterPath getHoveredShape(const QStyleOption *option,
                                 const double scale) const;
};

#endif // TABBARSTYLE_H
