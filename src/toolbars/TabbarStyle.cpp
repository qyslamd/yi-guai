#include "TabbarStyle.h"

#ifdef Q_OS_WIN
#include <QtWin>
#endif

#include <QStyleOptionTab>
#include <QPainter>
#include <QPainterPath>
#include <QApplication>
#include <QScreen>
#include <QtDebug>
#include <QTimer>

#include "utils/util_qt.h"

#include "toolbars/TabBar.h"

TabbarStyle::TabbarStyle(QWidget *widget, bool isInPrivate)
    : widget_(widget)
    , isInprivate_(isInPrivate)
{
//    auto timer = new QTimer(this);
//    connect(timer, &QTimer::timeout, this, [this]()
//    {
//        m_nStartAngle += 10 * 16;
//        if(m_nStartAngle == 360 * 16)
//        {
//            m_nStartAngle = 0;
//        }

//        if(widget_){
//            widget_->update();
//        }
//    });
//    timer->start(40);
}

void TabbarStyle::drawControl(QStyle::ControlElement element,
                              const QStyleOption *option,
                              QPainter *painter,
                              const QWidget *widget) const
{
    switch (element) {
    case CE_TabBarTabLabel:
        drawTabBarTabLabel(option, painter, widget);
        break;
    case CE_TabBarTabShape:
        drawTabBarTabShape(option,painter, widget);
        break;
    default:
        QProxyStyle::drawControl(element, option, painter, widget);
        break;
    }
}

QRect TabbarStyle::subElementRect(QStyle::SubElement subElement,
                                  const QStyleOption *option,
                                  const QWidget *widget) const
{
    const QStyleOptionTab *tabOption = qstyleoption_cast<const QStyleOptionTab *>(option);
    switch (subElement) {
    case QStyle::SE_TabBarTabLeftButton:
    {
        auto iconSize = tabOption->iconSize;
        auto rect = QProxyStyle::subElementRect(QStyle::SE_TabBarTabLeftButton, option, widget);
        rect.setWidth(16);
        rect.setX(rect.x() + 2 + iconSize.width());
        rect.setHeight(16);
        return rect;
    }
//    case QStyle::SE_TabBarTabText:
//    {
//        auto leftButtonRect = subElementRect(QStyle::SE_TabBarTabLeftButton, option, widget);
//        auto textRect = QProxyStyle::subElementRect(subElement, option, widget);

//        textRect.setX(leftButtonRect.x() + leftButtonRect.width());
//        return textRect;
//    }
//        break;

//    case QStyle::SE_TabBarTabRightButton:
//    {
//        auto rect = QProxyStyle::subElementRect(subElement, option, widget);
//         QRect ret(rect.x(),
//                     rect.y(),
//                     32,
//                     32);
//         qInfo()<<ret;
//         return ret;
//    }
    default:
        break;
    }
    return QProxyStyle::subElementRect(subElement, option, widget);
}

void TabbarStyle::drawTabBarTabLabel(const QStyleOption *option,
                                     QPainter *painter,
                                     const QWidget *w) const
{
    auto tabOption = qstyleoption_cast<const QStyleOptionTab *>(option);
    auto tabRect = tabOption->rect;
    auto textRect = subElementRect(QStyle::SE_TabBarTabText, tabOption, w);
    auto iconSize = tabOption->iconSize;
    auto pixmap = tabOption->icon.pixmap(iconSize);

    QRect iconRect(tabRect.x() + iconSize.width() / 2,
             tabRect.y() + ( tabRect.height() - iconSize.height()) / 2,
             iconSize.width(),
             iconSize.height());

    // Since the first tab borrows the range, the icon needs to be moved to the right
    if(tabOption->position == QStyleOptionTab::Beginning || tabOption->position == QStyleOptionTab::OnlyOneTab)
    {
        iconRect = QRect(iconRect.x() + 4,
                         iconRect.y(),
                         iconRect.width(),
                         iconRect.height());
    }
    if(const auto tabbar = qobject_cast<const TabBar *>(w)){
        auto index = tabbar->tabAt(tabRect.center());
        auto isLoading = tabbar->tabData(index).toBool();
        if(isLoading)   //加载状态
        {
            painter->save();

            painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
            QLinearGradient gradient(iconRect.topLeft(), iconRect.bottomRight());
            if(isInprivate_){
                gradient.setColorAt(0, Qt::white);
                gradient.setColorAt(0.7, 0x40E0D0);
                gradient.setColorAt(1, 0xADD8E6);
            }else{
                gradient.setColorAt(0, 0x00008B);//006CBE //Qt::blue
                gradient.setColorAt(0.7, Qt::gray);
                gradient.setColorAt(1, Qt::white);
            }
            painter->setPen(QPen(QBrush(gradient),
                                 2.0,
                                 Qt::DotLine,
                                 Qt::RoundCap,
                                 Qt::RoundJoin));
            painter->drawArc(iconRect,m_nStartAngle, m_nSpanAngle);
            painter->restore();
        }else{
            painter->drawPixmap(iconRect, pixmap);
        }
    }

    if(option->rect.width() <= 60) return;

    auto drawLabel = [=](const QLinearGradient &grident){
        painter->save();
        QPen pen;
        pen.setBrush(QBrush(grident));
        painter->setPen(pen);
        QTextOption option(Qt::AlignLeft | Qt::AlignVCenter);
        option.setWrapMode(QTextOption::NoWrap);

//        drawItemText(painter, textRect.marginsAdded(QMargins(-5,0,0,0)),0,w->palette(),true, tabOption->text);
        painter->drawText(textRect.marginsAdded(QMargins(-5,0,0,0)), tabOption->text, option);
        painter->restore();
    };

    QLinearGradient linearGrad(QPointF(textRect.x(), textRect.y() + textRect.height() / 2),
                               QPointF(textRect.x() + textRect.width(), textRect.y() + textRect.height() / 2));
    const qreal startPercent = 0.8f;
    QColor color = isInprivate_ ? QColor("#FFFFFF") : QColor("#000000");
    linearGrad.setColorAt(startPercent, color);
    QColor color2 = isInprivate_ ? QColor(0,0,0,0) : QColor(255,255,255,0);
    linearGrad.setColorAt(1, color2);
    if (tabOption->state.testFlag(QStyle::State_Selected))
    {
        drawLabel(linearGrad);
    }
    else
    {
        QColor colorOther("#666666");
#ifdef Q_OS_WIN
        // 在 windows 7 以上版本，可能应用 DWM 的颜色，如果应用主题颜色到标题栏
        // 这里绘制的文本需要根据 DWM 的背景颜色进行改变
        if(UtilQt::dwmColorPrevalence() && w->isActiveWindow() && !isInprivate_){
            QColor backgroundColor = QtWin::realColorizationColor();
            backgroundColor.setAlphaF(1);
            colorOther = UtilQt::getForgroundColor(backgroundColor);
        }
#endif
        linearGrad.setColorAt(startPercent, colorOther);
        drawLabel(linearGrad);
    }
}

void TabbarStyle::drawTabBarTabShape(const QStyleOption *option,
                                     QPainter *painter,
                                     const QWidget *w) const
{
    auto drawShape = [=](const QPainterPath &path, const QBrush &color)
    {
        painter->save();
        painter->setRenderHint(QPainter::Antialiasing);
        painter->setPen(Qt::NoPen);
        painter->setBrush(color);
        QPolygonF polygon = path.toFillPolygon();
        painter->drawPolygon(polygon);
        painter->restore();
    };
    const double scale = 1.0 / 3.0; // 圆角占高度的比例
    QStyle::State state =  option->state;
    if(state.testFlag(QStyle::State_Selected))
    {
        QPainterPath path = getSelectedShape(option, scale);
        auto brush = painter->brush();
        if(isInprivate_){
            brush = QBrush(QColor("#3B3B3B"));
        }else{
            brush = QBrush(QColor("#F9F9FB"));
#if 0
            const QStyleOptionTab *tabOption = qstyleoption_cast<const QStyleOptionTab *>(option);
            QRectF rect = tabOption->rect;
            QLinearGradient linearGrad(QPointF(rect.x() + rect.width() / 2, rect.y()),
                                       QPointF(rect.x()  + rect.width() / 2,
                                               rect.y() + rect.height())
                                       );
            QColor color1(0xFFFFFF);
            color1.setAlphaF(0.7);
            linearGrad.setColorAt(0, color1);
            QColor color2(0xFFFFFF);
            color2.setAlphaF(0.1);
            linearGrad.setColorAt(1, color2);
            brush = QBrush(linearGrad);
#endif
        }
        drawShape(path, brush);
    }else if(state.testFlag(QStyle::State_MouseOver))
    {
        QPainterPath path = getHoveredShape(option, scale);
        auto brush = painter->brush();
        if(isInprivate_){
            auto color = QColor(89,89,89);
            color.setAlphaF(0.3);
            brush = color;
        }else{
            auto color = QColor(255,255,255);
            color.setAlphaF(0.3);
            brush = color;
        }
        drawShape(path, brush);
    }else
    {
        auto tabOption = qstyleoption_cast<const QStyleOptionTab *>(option);
        auto rect = tabOption->rect;

        qreal len = rect.height() * 1.0 / 3.0;  // 1 / 3 是绘制的分割线比例

        painter->save();
        QColor color(75,75,75);
#ifdef Q_OS_WIN
        if(UtilQt::dwmColorPrevalence())
        {
            QColor bgColor = QtWin::realColorizationColor();
            w->isActiveWindow() ?
            bgColor.setAlphaF(1) : bgColor.setAlphaF(0.7);

            color = UtilQt::getForgroundColor(bgColor);
        }
#endif
        color = isInprivate_ ? QColor("#D3D3D5") : color;
        QPen pen(color);
        pen.setWidthF(0.5);
        painter->setPen(pen);
        if(tabOption->selectedPosition != QStyleOptionTab::NextIsSelected){
            painter->drawLine(QPointF(rect.x() + rect.width() - len / 3, rect.y() + len / 2),
                              QPointF(rect.x() + rect.width() - len / 3, rect.y() + rect.height() - len / 2));
        }
        painter->restore();
    }
}

QPainterPath TabbarStyle::getSelectedShape(const QStyleOption *option,
                                             const double scale) const
{
    auto tabOption = qstyleoption_cast<const QStyleOptionTab *>(option);
    QRectF rect = tabOption->rect;   // 用于绘制的整个矩形大小
    rect.adjust(0, 2, 0, 0);
    qreal len = 1.0 * rect.height() * scale;   // 画弧形的正方形的边长

    switch (tabOption->position) {
    case QStyleOptionTab::Beginning:
        rect = rect.marginsAdded(QMarginsF(len / 2.0, 0, len, 0));
        break;
    case QStyleOptionTab::Middle:
        rect = rect.marginsAdded(QMarginsF(len, 0, len, 0));
        break;
    case QStyleOptionTab::End:
        rect = rect.marginsAdded(QMarginsF(len, 0, len / 2.0, 0));
        break;
    case QStyleOptionTab::OnlyOneTab:
        rect = rect.marginsAdded(QMarginsF(len / 2.0, 0, len / 2.0, 0));
        break;
    default:
        break;
    }

    // 左下 正方形
    QRectF arcRectLeftBottom(rect.x(),
                             rect.y() + rect.height() - len,
                             len,
                             len);
    // 设置Path的起点
    QPointF pathStartPoint((qreal)arcRectLeftBottom.x() + (qreal)arcRectLeftBottom.width() / 2.0,
                           (qreal)(rect.y() + rect.height()));
    QPainterPath path(pathStartPoint);

    // 绘制左下角的圆弧。逆时针方向，从 270° 开始 绘制 90° 的圆弧
    path.arcTo(arcRectLeftBottom, 270.0, 90.0);

    QPointF p1(path.currentPosition().x(),
               (qreal)rect.y() + len / 2.0);
    path.lineTo(p1);

    QRectF arcRectLeftTop((qreal)rect.x() + len,
                          rect.y(),
                          len,
                          len);
    path.arcTo(arcRectLeftTop, 180.0, -90.0);

    QPointF p2(rect.x() + rect.width() - len - len / 2.0,
               (qreal)rect.y());

    path.lineTo(p2);

    QRectF arcRectRightTop(rect.x() + rect.width() - 2.0 * len,
                           rect.y(),
                           len,
                           len);

    path.arcTo(arcRectRightTop, 90.0, -90.0);

    QPointF p3(path.currentPosition().x(),
               (qreal)rect.y() + rect.height() - len / 2.0);

    path.lineTo(p3);

    QRectF arcRectRightBottom(path.currentPosition().x(),
                           rect.y() + rect.height() - len,
                           len,
                           len);

    path.arcTo(arcRectRightBottom, 180.0, 90.0);

    return path;
}

QPainterPath TabbarStyle::getHoveredShape(const QStyleOption *option,
                                            const double scale) const
{
    auto tabOption = qstyleoption_cast<const QStyleOptionTab *>(option);
    QRectF rect = tabOption->rect;       // 用于绘制的整个矩形大小
//    rect.adjust(0, 2, 0, 0);
    qreal len = rect.height() * scale;   // 画弧形的正方形的边长

    auto selectPos = tabOption->selectedPosition;
    // 根据当前 tab 的位置调整绘制的区域
    switch (tabOption->position) {
    case QStyleOptionTab::Beginning:
    {
        rect = rect.marginsAdded(QMarginsF(len / 2, 0, len, 0));
//        if(selectPos == QStyleOptionTab::NotAdjacent){
//            rect = rect.marginsAdded(QMarginsF(0, 0, -len / 2, 0));
//        }
    }
        break;
    case QStyleOptionTab::Middle:
    {
        rect = rect.marginsAdded(QMarginsF(len, 0, len, 0));
//        if(selectPos == QStyleOptionTab::PreviousIsSelected){
//            rect = rect.marginsAdded(QMarginsF(0, 0, -len / 2, 0));
//        }else if(selectPos == QStyleOptionTab::NextIsSelected){
//            rect = rect.marginsAdded(QMarginsF(-len / 2, 0, 0, 0));
//        }else{
//            rect = rect.marginsAdded(QMarginsF(-len / 2, 0, -len / 2, 0));
//        }
    }
        break;
    case QStyleOptionTab::End:
    {
        rect = rect.marginsAdded(QMarginsF(len / 2, 0, len / 2, 0));
        if(selectPos == QStyleOptionTab::PreviousIsSelected){
            rect = rect.marginsAdded(QMarginsF(len / 2, 0, 0, 0));
        }
    }
        break;
    default:
        break;
    }

    // 左下 正方形
    QRectF arcRectLeftBottom(rect.x(),
                             rect.y() + rect.height() - len,
                             len,
                             len);
    // 设置Path的起点
    QPointF pathStartPoint((qreal)arcRectLeftBottom.x() + (qreal)arcRectLeftBottom.width() / 2,
                           (qreal)(rect.y() + rect.height()));
    QPainterPath path(pathStartPoint);

    // 绘制左下角的圆弧。逆时针方向，从 270° 开始 绘制 90° 的圆弧
    path.arcTo(arcRectLeftBottom, 270.0, 90.0);

    QPointF p1(path.currentPosition().x(),
               (qreal)rect.y() + len / 2);
    path.lineTo(p1);

    QRectF arcRectLeftTop((qreal)rect.x() + len,
                          rect.y(),
                          len,
                          len);
    path.arcTo(arcRectLeftTop, 180.0, -90.0);

    QPointF p2(rect.x() + rect.width() - len - len / 2,
               (qreal)rect.y());

    path.lineTo(p2);

    QRectF arcRectRightTop(rect.x() + rect.width() - 2 * len,
                           rect.y(),
                           len,
                           len);

    path.arcTo(arcRectRightTop, 90.0, -90.0);

    QPointF p3(path.currentPosition().x(),
               (qreal)rect.y() + rect.height() - len / 2);

    path.lineTo(p3);

    QRectF arcRectRightBottom(path.currentPosition().x(),
                           rect.y() + rect.height() - len,
                           len,
                           len);

    path.arcTo(arcRectRightBottom, 180.0, 90.0);

    return path;
}
