#include "VerticalTabbarStyle.h"

#include <QPainter>
#include <QPainterPath>
#include <QLinearGradient>
#include <QStyleOptionTab>
#include <QtDebug>
#include <QtMath>
#include <QPainterPath>

VerticalTabbarStyle::VerticalTabbarStyle()
{

}

QSize VerticalTabbarStyle::sizeFromContents(QStyle::ContentsType type,
                                     const QStyleOption *option,
                                     const QSize &size,
                                     const QWidget *widget) const
{
    return QProxyStyle::sizeFromContents(type, option, size, widget);
//    if (type == QStyle::CT_TabBarTab) {
//        s.transpose();
//        s.rwidth() = 150;
//        s.rheight() = 30;
//    }
//    return s;
}

void VerticalTabbarStyle::drawControl(QStyle::ControlElement element,
                               const QStyleOption *opt,
                               QPainter *p,
                               const QWidget *w) const
{
    auto tabOpt = qstyleoption_cast<const QStyleOptionTab *>(opt);
    // 不用怕，先绘制的是shape，然后才是lable
    if(element == CE_TabBarTabShape)
    {
        p->setRenderHint(QPainter::Antialiasing);
        if(tabOpt->state.testFlag(QStyle::State_Selected)){
            auto rect = tabOpt->rect;
            drawRectShadow(p, rect, 4);

            QPainterPath path;
            path.addRoundedRect(rect.marginsAdded(QMargins(-4,-2,-4,-4)),4,4);
            p->save();
            p->setPen(QPen(QColor(0xFF1493)));
            p->drawPath(path);
            p->restore();
            p->fillPath(path, Qt::white);
        }else if(tabOpt->state.testFlag(QStyle::State_MouseOver)){
            auto rect = tabOpt->rect;
            drawRectShadow(p, rect, 4);

            QPainterPath path;
            path.addRoundedRect(rect.marginsAdded(QMargins(-4,-2,-4,-4)),4,4);
            p->save();
            p->setPen(QPen(QColor(0xD2D2D2)));
            p->drawPath(path);
            p->restore();
            p->fillPath(path, Qt::white);
        }
    }else if(element == CE_TabBarTabLabel){
        static const int leftSpacing = 8;
        auto rect = tabOpt->rect;
        auto size = tabOpt->iconSize;
        QRect iconRect(rect.x() + leftSpacing,
                      rect.y() + (rect.height()-size.height()) / 2,
                      size.width(),
                      size.height()
                      );
        drawItemPixmap(p, iconRect, Qt::AlignCenter,tabOpt->icon.pixmap(tabOpt->iconSize));
        drawItemText(p, rect, Qt::AlignCenter, w->palette(), true, tabOpt->text);
    }
    else{
        QProxyStyle::drawControl(element, opt, p, w);
    }
}

QRect VerticalTabbarStyle::subElementRect(SubElement subElement,
                                   const QStyleOption *option,
                                   const QWidget *widget) const
{
//    return QProxyStyle::subElementRect(subElement,option, widget);
    auto tabOpt = qstyleoption_cast<const QStyleOptionTab *>(option);
    switch (subElement) {
    case QStyle::SE_TabBarTabRightButton:
    {
       auto rect = QProxyStyle::subElementRect(subElement, option, widget);
       return QRect(tabOpt->rect.x() + tabOpt->rect.width() - rect.width() - 8,
                    rect.y() + (rect.height()-rect.width()) / 2,
                    rect.width(),
                    rect.height());
        return QProxyStyle::subElementRect(subElement, option, widget);
    }
//    case QStyle::SE_TabBarScrollLeftButton:
//    {
//        auto rect = QProxyStyle::subElementRect(subElement, option, widget);
//        return QRect(widget->rect().x() + (widget->rect().width() - rect.width()) / 2,
//                     rect.y(),
//                     16,
//                     16);
//    }
    default:
        return QProxyStyle::subElementRect(subElement,option, widget);
    }
}

void VerticalTabbarStyle::drawRectShadow(QPainter *p,
                                  const QRect &rect,
                                  const int shadowWidth) const
{
    if(shadowWidth == 0){
        return;
    }
    p->save();
    // 满足 y = kx + b
    qreal k = -1.0 / shadowWidth * 1.0;
    int b = 10;
    QColor color(0,0,0,b);
    for (int i = 0; i < shadowWidth; i++)
    {
        // 控制透明度和圆角矩形即可
        color.setAlpha(k * i + b);
        p->setPen(color);
        // drawRoundedRect函数的半径是百分比
        p->drawRoundedRect(rect.x() + shadowWidth - i,
                           rect.y() + shadowWidth - i,
                           rect.width() -  (shadowWidth - i) * 2,
                           rect.height() - (shadowWidth - i) * 2,
                           0.2 * rect.height(),
                           0.2 * rect.height());
    }
    p->restore();
}
