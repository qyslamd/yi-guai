#include "menubarstyle.h"
#include <QStyleOptionMenuItem>
#include <QWindow>
#include <QMenuBar>
#include <QtDebug>
#include <QPainter>

static QWindow *qt_getWindow(const QWidget *widget)
{
    return widget ? widget->window()->windowHandle() : nullptr;
}

MenuBarStyle::MenuBarStyle(QStyle *style)
    : QProxyStyle(style)
{

}

QSize MenuBarStyle::sizeFromContents(QStyle::ContentsType ct,
                                     const QStyleOption *opt,
                                     const QSize &csz,
                                     const QWidget *w) const
{
    QSize sz = csz;
    switch (ct) {
    case QStyle::CT_MenuBarItem:
    {
//        qInfo()<<__FUNCTION__;
        int height = csz.height();
        if(const QMenuBar *bar = static_cast<const QMenuBar *>(w))
        {
//            height = bar->height();
        }
        sz = QSize(75, height);
    }
    default:
        break;
    }
    return sz;
}

void MenuBarStyle::drawControl(QStyle::ControlElement element,
                               const QStyleOption *opt,
                               QPainter *p,
                               const QWidget *w) const
{
    switch (element) {
    case QStyle::CE_MenuBarItem:
        if (const QStyleOptionMenuItem *mbi = qstyleoption_cast<const QStyleOptionMenuItem *>(opt)) {

            uint alignment = Qt::AlignCenter | Qt::TextShowMnemonic | Qt::TextDontClip
                    | Qt::TextSingleLine;
            if (!proxy()->styleHint(SH_UnderlineShortcut, mbi, w))
                alignment |= Qt::TextHideMnemonic;

            int iconExtent = proxy()->pixelMetric(PM_SmallIconSize);
            if(mbi->state & State_Selected ){
                p->fillRect(mbi->rect, QColor(0xa8a8a8));
            }
//            qInfo()<<__FUNCTION__<<mbi->state;

            QPixmap pix = mbi->icon.pixmap(qt_getWindow(w), QSize(iconExtent, iconExtent), (mbi->state & State_Enabled) ? QIcon::Normal : QIcon::Disabled);
            if (!pix.isNull())
                proxy()->drawItemPixmap(p,mbi->rect, alignment, pix);
            else
                proxy()->drawItemText(p, mbi->rect, alignment, mbi->palette, mbi->state & State_Enabled,
                                      mbi->text, QPalette::ButtonText);
        }
        break;
    default:
        QProxyStyle::drawControl(element, opt, p, w);
    }
}
