#include "listtabstyle.h"

#include <QPainter>
#include <QPainterPath>
#include <QLinearGradient>
#include <QStyleOptionTab>

ListTabStyle::ListTabStyle()
{

}

QSize ListTabStyle::sizeFromContents(QStyle::ContentsType type,
                                     const QStyleOption *option,
                                     const QSize &size,
                                     const QWidget *widget) const
{
    QSize s = QProxyStyle::sizeFromContents(type, option, size, widget);
    if (type == QStyle::CT_TabBarTab) {
        s.transpose();
        s.rwidth() = 90; // 设置每个tabBar中item的大小
        s.rheight() = 44;
    }
    return s;
}

void ListTabStyle::drawControl(QStyle::ControlElement element,
                               const QStyleOption *opt,
                               QPainter *p,
                               const QWidget *w) const
{
    if (element == CE_TabBarTabLabel) {
        if (const QStyleOptionTab *tab = qstyleoption_cast<const QStyleOptionTab *>(opt)) {
            QRect allRect = tab->rect;

            if (tab->state & QStyle::State_Selected) {
                p->save();
                p->setPen(0x89cfff);
                p->setBrush(QBrush(0x89cfff));
                p->drawRect(allRect.adjusted(6, 6, -6, -6));
                p->restore();
            }
            QTextOption option;
            option.setAlignment(Qt::AlignCenter);
            if (tab->state & QStyle::State_Selected) {
                p->setPen(0xf8fcff);
            }
            else {
                p->setPen(0x5d5d5d);
            }

            p->drawText(allRect, tab->text, option);
            return;
        }
    }

    if (element == CE_TabBarTab) {
        QProxyStyle::drawControl(element, opt, p, w);
    }
}
