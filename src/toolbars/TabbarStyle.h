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
    // QObject interface
    void timerEvent(QTimerEvent *event) override;
    void drawTabBarTabLabel(const QStyleOption *option,
                            QPainter *painter,
                            const QWidget *w) const;

    //
    void drawTabBarTabShape(const QStyleOption *option,
                            QPainter *painter,
                            const QWidget *w) const;
private:
    int progress_timer_id_;
    int m_nStartAngle{ 0 };
    int m_nSpanAngle{ 4320 };
    bool isInprivate_;
    mutable double dpi_;


    QPainterPath getSelectedShape(const QStyleOption *option,
                                  const double scale) const;
    QPainterPath getHoveredShape(const QStyleOption *option,
                                 const double scale) const;
};

#endif // TABBARSTYLE_H
