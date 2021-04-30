#include "Dp_Widget_Caption.h"
#include "utils/util_qt.h"
#include <QtGlobal>
#include <QtDebug>

#ifdef Q_OS_WIN
#include <QPaintEvent>
#include <QPen>
#include <QMouseEvent>
#include <QHelpEvent>
#include <QPainter>
#include <QtDebug>
#include <QTimer>
#include <QToolTip>
#include <QtWin>
#include <Windows.h>
#pragma comment(lib, "user32.lib")

Dp_Widget_Caption::Dp_Widget_Caption(bool isInprivate, QWidget *parent)
    : QWidget(parent)
    , inprivate_(isInprivate)
{
    setMouseTracking(true);
}

/// QWidget在鼠标移动后停留的短暂时间内，会触发一次QEvent::ToolTip
/// 如果自绘制的按钮，可以在event中显示tooltip
bool Dp_Widget_Caption::event(QEvent *e)
{
    if (e->type() == QEvent::ToolTip) {
        QHelpEvent *helpEvent = static_cast<QHelpEvent *>(e);
        int index = itemAt(helpEvent->pos());
        if (index != -1) {
            QString str;
            switch (index) {
            case Rect_Skin:
                str = tr("skin");
                break;
            case Rect_FullScrn:
                str = tr("fullscreen");
                break;
            case Rect_Mini:
                str = tr("minimize");
                break;
            case Rect_NormalMax:

                str = this->window()->isMaximized()
                        ? tr("restore")
                        : tr("maximize");
                break;
            case Rect_Close:
                str = tr("close");
                break;
            default:
                Q_UNREACHABLE();
            }
            QToolTip::showText(helpEvent->globalPos(), str);
        } else {
            QToolTip::hideText();
            e->ignore();
        }

        return true;
    }
    return QWidget::event(e);
}

void Dp_Widget_Caption::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    if( this->window()->isFullScreen()){
        return QWidget::paintEvent(event);
    }

    QRectF rectSkin = getRect(Rect_Skin);
    QRectF rectFull = getRect(Rect_FullScrn);
    QRectF rectMini = getRect(Rect_Mini);
    QRectF rectMaxNormal = getRect(Rect_NormalMax);
    QRectF rectClose = getRect(Rect_Close);

    QPainter p(this);
    p.save();

    QColor color("#B9B9B9");
    color.setAlphaF(0.7);

    if(skin_hover_ ){
        p.fillRect(rectSkin, color);
    }
    if(full_hover_ ){
        p.fillRect(rectFull, color);
    }
    if(mini_hover_ ){
        p.fillRect(rectMini, color);
    }

    if(max_hover_ ){
        p.fillRect(rectMaxNormal, color);
    }

    if(close_hover_){
        p.fillRect(rectClose, QColor("#E81123"));
    }

    // draw skin icon
    QPixmap pix;
    if(skin_btn_enable_){
    pix = getPixmap(Icon_Skin, getIconColor());
    p.drawPixmap(QPointF(rectSkin.x() + (rectSkin.width() - pix.width()) / 2,
                         rectSkin.y() + (rectSkin.height() - pix.height()) / 2),
                 pix);
    }

    // draw fullscreen icon
    if(full_btn_enable_){
        pix = getPixmap(Icon_FullScrn, getIconColor());
        p.drawPixmap(QPointF(rectFull.x() + (rectFull.width() - pix.width()) / 2,
                             rectFull.y() + (rectFull.height() - pix.height()) / 2),
                     pix);
    }

    // draw minimum icon
    pix = getPixmap(Icon_Mini, getIconColor());
    p.drawPixmap(QPointF(rectMini.x() + (rectMini.width() - pix.width()) / 2,
                         rectMini.y() + (rectMini.height() - pix.height()) / 2),
                 pix);
    // draw maximum and resore icon
    IconType type = window()->isMaximized()
            ? Icon_Restore
            : Icon_Max;
    pix = getPixmap(type, getIconColor());
    p.drawPixmap(QPointF(rectMaxNormal.x() + (rectMaxNormal.width() - pix.width()) / 2,
                         rectMaxNormal.y() + (rectMaxNormal.height() - pix.height()) / 2),
                 pix);

    // draw close icon
    if(close_hover_){
        pix = getPixmap(Icon_Close, Qt::white);
    }else{
        pix = getPixmap(Icon_Close, getIconColor());
    }

    p.drawPixmap(QPointF(rectClose.x() + (rectClose.width() - pix.width()) / 2,
                         rectClose.y() + (rectClose.height() - pix.height()) / 2),
                 pix);
    p.restore();
}

void Dp_Widget_Caption::mouseMoveEvent(QMouseEvent *event)
{
    QWidget::mouseMoveEvent(event);

    QPoint p = event->pos();
    skin_hover_ = getRect(Rect_Skin).contains(p) && skin_btn_enable_;
    full_hover_ = getRect(Rect_FullScrn).contains(p) && full_btn_enable_;
    mini_hover_ = getRect(Rect_Mini).contains(p);
    max_hover_ = getRect(Rect_NormalMax).contains(p);
    close_hover_ = getRect(Rect_Close).contains(p);

    update();
}

void Dp_Widget_Caption::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);

    QPoint p = event->pos();
    if(event->button() == Qt::LeftButton)
    {
        if(getRect(Rect_Skin).contains(p)){
            skin_pressed_ = true;
        }
        else if(getRect(Rect_FullScrn).contains(p)){
            full_pressed_ = true;
        }
        else if(getRect(Rect_Mini).contains(p) ){
            mini_pressed_ = true;
        }
        else if(getRect(Rect_NormalMax).contains(p)){
            max_pressed_ = true;
        }
        else if(getRect(Rect_Close).contains(p)){
            close_pressed_ = true;
        }
        else if(getRect(Rect_Other).contains(p)){

            if(::ReleaseCapture()){
                SendMessage(HWND(this->window()->winId()), WM_SYSCOMMAND, SC_MOVE + HTCAPTION, 0);
                event->ignore();
            }
        }
    }
}

void Dp_Widget_Caption::mouseReleaseEvent(QMouseEvent *event)
{
    QWidget::mouseReleaseEvent(event);

    QPoint p = event->pos();
    if(event->button() == Qt::LeftButton)
    {
        if(getRect(Rect_Skin).contains(p)){
            if(skin_btn_enable_ && skin_pressed_){
                emit btnSkinClicked(mapToGlobal(getRect(Rect_Skin).topLeft().toPoint()));
                skin_pressed_ = false;
            }
        }
        else if(getRect(Rect_FullScrn).contains(p)){
            if(full_btn_enable_ && full_pressed_){
                emit btnFullScrnClicked();
                full_pressed_ = false;
            }
        }
        else if(getRect(Rect_Mini).contains(p) ){
            if(mini_pressed_){
                emit btnMinClicked();
                mini_pressed_ = false;
            }
        }
        else if(getRect(Rect_NormalMax).contains(p)){
            if(max_pressed_){
                emit btnMaxNormalClicked();
                max_pressed_ = false;
            }
        }
        else if(getRect(Rect_Close).contains(p)){
            if(close_pressed_){
                emit btnCloseClicked();
                close_pressed_ = false;
            }
        }
        update();
    }
}

void Dp_Widget_Caption::mouseDoubleClickEvent(QMouseEvent *event)
{
    QWidget::mouseDoubleClickEvent(event);
    auto w = this->window();
    auto p = event->pos();
    if(getRect(Rect_Other).contains(p)){
        if(event->button() == Qt::LeftButton){
            if(w->isMaximized()){
                w->showNormal();
            }else{
                w->showMaximized();
            }
        }
    }
}

void Dp_Widget_Caption::leaveEvent(QEvent *event)
{
    skin_hover_ = false;
    full_hover_ = false;
    mini_hover_ = false;
    max_hover_ = false;
    close_hover_ = false;
    update();
    QWidget::leaveEvent(event);
}

QRectF Dp_Widget_Caption::getRect(Dp_Widget_Caption::RectType type)
{
    //96 45,28
    // 120 58,42
    //144 70,45
    // 168 80,54

    int BtnWidth = 45;
    int BtnHeight = 28;
    int BtnSpacing = 1;
    int dpi = this->window()->logicalDpiX();
    switch (dpi) {
    case 96:
        BtnWidth = 45;
        BtnHeight = 30;
        break;
    case 120:
        BtnWidth = 56;
        BtnHeight = 40;
        break;
    case 144:
        BtnWidth = 68;
        BtnHeight = 42;
        break;
    case 168:
        BtnWidth = 78;
        BtnHeight = 52;
        break;
    default:
        BtnWidth = 45;
        BtnHeight = 28;
        break;
    }


    int x = 0;
    int y = 0;
    int w = this->width();

    QRectF rectClose(x + w - BtnWidth,
                     y,
                     BtnWidth,
                     BtnHeight);
    QRectF rectMax(rectClose.x() - BtnSpacing - BtnWidth,
                   y,
                   BtnWidth,
                   BtnHeight);
    QRectF rectMini(rectMax.x() - BtnSpacing - BtnWidth,
                    y,
                    BtnWidth,
                    BtnHeight);

    QRectF rectFull(rectMini.x() - BtnSpacing - BtnWidth,
                    y,
                    BtnWidth,
                    BtnHeight);
    QRectF rectSkin(rectFull.x() - BtnSpacing - BtnWidth,
                    y,
                    BtnWidth,
                    BtnHeight);

    int rectOtherWidth = x + rectMini.x();
    if( full_btn_enable_){
        rectOtherWidth +=  rectFull.width() + BtnSpacing;
    }
    if(skin_btn_enable_){
        rectOtherWidth += rectSkin.width();
    }
    QRectF rectOther(x,
                     y,
                     rectOtherWidth,
                     42);

    switch (type) {
    case Rect_Close:
        return rectClose;
    case Rect_NormalMax:
        return rectMax;
    case Rect_Mini:
        return rectMini;
    case Rect_FullScrn:
        return rectFull;
    case Rect_Skin:
        return rectSkin;
    case Rect_Other:
        return rectOther;
    default:
        return QRectF();
    }
}

QPixmap Dp_Widget_Caption::getPixmap(Dp_Widget_Caption::IconType type, const QColor &color)
{
    QSize size(10, 10);
    int rr;
    int dpi = this->window()->logicalDpiX();
    switch (dpi) {
    case 96:
        rr = 30 / 3;
        break;
    case 120:
        rr = 40 / 3;
        break;
    case 144:
        rr = 42 /3;
        break;
    case 168:
        rr = 52 / 3;
        break;
    default:
        rr = 30 /3;
        break;
    }
    size.setWidth(rr);
    size.setHeight(rr);

    QPixmap pix(size);
    pix.fill(Qt::transparent);

    QSizeF sizeF(size.width() *1.0,  size.height() * 1.0);
    QPainter p(&pix);
    switch (type) {
    case Icon_Skin:
    {
        p.save();

        QRectF target(0.0, 0.0, sizeF.width(), sizeF.height());
        QRectF source(0.0, 0.0, 24.0, 24.0);
        QPixmap pixmap(":/Resources/imgs/themes_24px.png");
        p.drawPixmap(target, pixmap, source);

        p.restore();
    }
        break;
    case Icon_FullScrn:
    {
        p.save();

        QPen pen(QBrush(color), 1.0);
        p.setPen(pen);

        QPointF p1(0, pix.height() * 3.0 / 5.0);
        QPointF p2(0, pix.height() - 1.0);
        QPointF p3(pix.width() * 2.0 / 5.0, pix.height() - 1.0);

        QPointF p4(pix.width() * 3.0 / 5.0, 0);
        QPointF p5(pix.width() - 1.0, 0);
        QPointF p6(pix.width() - 1.0, pix.height() * 2.0 / 5.0);

        p.drawLine(p1, p2);
        p.drawLine(p2, p3);
        p.drawLine(p2, p5);
        p.drawLine(p4, p5);
        p.drawLine(p5, p6);

        p.restore();
        break;
    }
    case Icon_Mini:
    {
        p.save();
        QPen pen(QBrush(color), 1.0);
        p.setPen(pen);

        p.drawLine(QPointF(0, sizeF.height() / 2),
                   QPointF(sizeF.width(), sizeF.height() / 2));

        p.restore();
    }
        break;
    case Icon_Max:
    {
        p.save();
        QPen pen(QBrush(color), 1.0);
        p.setPen(pen);

        p.drawRect(QRectF(0.0, 0.0, sizeF.width() - 1.0, sizeF.height() - 1.0));

        p.restore();
    }
        break;
    case Icon_Restore:
    {
        p.save();
        p.setRenderHint(QPainter::Antialiasing);
        QPen pen(QBrush(color), 1.0);
        p.setPen(pen);

        QRectF rect(0,0,size.width(), size.height());
        QRectF r1(rect.x(),
                  rect.y() + rect.height() / 4,
                  rect.width() * 3 / 4,
                  rect.width() * 3 / 4);

        p.drawRect(r1);

        QVector<QLineF> lines;
        QLineF line1(QPointF(rect.x() + rect.width() * 1 / 4, rect.y() + rect.width() * 1 / 4),
                     QPointF(rect.x() + rect.width() * 1 / 4, rect.y())
                     );
        QLineF line2(QPointF(rect.x() + rect.width() * 1 / 4, rect.y()),
                     QPointF(rect.x() + rect.width(), rect.y())
                     );
        QLineF line3(QPointF(rect.x() + rect.width(), rect.y()),
                     QPointF(rect.x() + rect.width(), rect.y() + rect.width() * 3 / 4)
                     );
        QLineF line4(QPointF(rect.x() + rect.width(), rect.y() + rect.width() * 3 / 4),
                     QPointF(rect.x() + rect.width() * 3 / 4, rect.y() + rect.width() * 3 / 4)
                     );

        lines.append(line1);
        lines.append(line2);
        lines.append(line3);
        lines.append(line4);

        p.drawLines(lines);

        p.restore();
    }
        break;
    case Icon_Close:
    {
        p.save();
        p.setRenderHint(QPainter::Antialiasing);
        QPen pen(QBrush(color), 1.0);
        p.setPen(pen);

        QRectF rect(0,0,size.width(), size.height());
        p.drawLine(QPointF(rect.x(), rect.y()),
                   QPointF(rect.x() + rect.width(), rect.y() + rect.height())
                   );
        p.drawLine(QPointF(rect.x() + rect.width(), rect.y()),
                   QPointF(rect.x(), rect.y() + rect.height())
                   );

        p.restore();
    }
        break;
    default:
        break;
    }

    return pix;
}

QColor Dp_Widget_Caption::getIconColor(bool isClose)
{
    if(!isClose){
        QColor color(Qt::black);
        if(UtilQt::dwmColorPrevalence() && this->isActiveWindow()){
            color = UtilQt::getForgroundColor(QtWin::colorizationColor());
        }else if(inprivate_){
            color =  QColor("#DEDEDE");
        }

        return color;
    }else{
        return Qt::white;
    }
}

int Dp_Widget_Caption::itemAt(const QPoint &pos)
{
    if(getRect(Rect_Mini).contains(pos)){
        return Rect_Mini;
    }else if(getRect(Rect_NormalMax).contains(pos)){
        return Rect_NormalMax;
    }else if(getRect(Rect_Close).contains(pos)){
        return Rect_Close;
    }else if(getRect(Rect_FullScrn).contains(pos) && full_btn_enable_){
        return Rect_FullScrn;
    }else if(getRect(Rect_Skin).contains(pos) && skin_btn_enable_){
        return Rect_Skin;
    }
    return -1;
}

#endif  // Q_OS_WIN

