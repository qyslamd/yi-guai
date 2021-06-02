#include "framelesswidget.h"

#include <QtDebug>
#include <QPalette>
#include <QLinearGradient>
#include <QColor>
#include <QPaintEvent>
#include <QPainter>
#include <QtMath>
#include <QToolTip>
#include <QHBoxLayout>

#include "utils/util_qt.h"
#ifdef Q_OS_WIN
#include <Windows.h>
#include <Windowsx.h>
#pragma comment(lib, "User32.lib")
#endif

FramelessWidget::FramelessWidget(QWidget *parent) :
    QWidget(parent),
    layout_(new QHBoxLayout(this))
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setMouseTracking(true);
    setMinimumSize(300,300);

    layout_->setContentsMargins(FrameWidth,
                                FrameWidth + CaptionHeight,
                                FrameWidth,
                                FrameWidth);
    layout_->setSpacing(0);
}

FramelessWidget::~FramelessWidget()
{

}

bool FramelessWidget::event(QEvent *e)
{
    if (e->type() == QEvent::ToolTip)
    {
        QHelpEvent *helpEvent = static_cast<QHelpEvent *>(e);
        auto button = buttonAt(helpEvent->pos());
        if (button != CaptionButtons::Button_None) {
            QString str;
            switch (button) {
            case CaptionButtons::Button_Mini:
                str = tr("minimize");
                break;
            case CaptionButtons::Button_NormalMax:

                str = this->window()->isMaximized()
                        ? tr("restore")
                        : tr("maximize");
                break;
            case CaptionButtons::Button_Close:
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

void FramelessWidget::setWidget(QWidget *widget)
{
    // 如果有了，移除并删除
    if(this->widget()){
        auto item = layout_->takeAt(0);
        if(item){
            delete item->widget();
            delete item;
        }
    }
    // 重新添加
    layout_->addWidget(widget);
}

QWidget *FramelessWidget::widget()
{
    auto item = layout_->itemAt(0);
    if(item){
        return item->widget();
    }
    return nullptr;
}

bool FramelessWidget::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
#ifdef Q_OS_WIN
    //Workaround for known bug -> check Qt forum : https://forum.qt.io/topic/93141/qtablewidget-itemselectionchanged/13
#if (QT_VERSION == QT_VERSION_CHECK(5, 11, 1))
    MSG* msg = *reinterpret_cast<MSG**>(message);
#else
    MSG* msg = reinterpret_cast<MSG*>(message);
#endif

#define LOG_MACRO(x) #x
    switch (msg->message)
    {
    case WM_DWMCOLORIZATIONCOLORCHANGED:
        return false;
    case WM_DPICHANGED:
        return false;
    case WM_NCPAINT:
        return false;
    case WM_NCRBUTTONDOWN:
    {
        qInfo()<<__FUNCTION__<<LOG_MACRO(WM_NCRBUTTONDOWN);
        auto xPos = GET_X_LPARAM(msg->lParam);
        auto yPos = GET_Y_LPARAM(msg->lParam);
        HMENU sysMenu = ::GetSystemMenu((HWND)winId(), FALSE);
        ::TrackPopupMenu(sysMenu,0, xPos, yPos, NULL, (HWND)winId(), NULL);
    }
        return false;
    case WM_NCRBUTTONUP:

        return false;
    case WM_SYSKEYDOWN:

        return false;
    default:
        break;
    }
#endif
    return QWidget::nativeEvent(eventType, message, result);
}

void FramelessWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);

    /*--------------------------------------------------
    *   draw caption background
    *--------------------------------------------------*/
    p.save();
    QRect captionRect{FrameWidth,
                FrameWidth,
                width() - 2 * FrameWidth,
                CaptionHeight};
    p.fillRect(captionRect, QColor(0xCECECE));
    p.restore();

    /*--------------------------------------------------
    *   draw client background
    *--------------------------------------------------*/
    p.save();
    QRect clientRect{FrameWidth,
                FrameWidth + CaptionHeight,
                width() - 2 * FrameWidth,
                height() - 2 * FrameWidth - CaptionHeight};
    p.fillRect(clientRect,
                     QBrush(Qt::white));
    p.restore();

    /*--------------------------------------------------
    *   draw window shadow
    *--------------------------------------------------*/
    drawShadow(&p);

    /*--------------------------------------------------
    *   draw buttons
    *--------------------------------------------------*/
    drawButtons(&p);
}

void FramelessWidget::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        if(region_ != Region::HT_Client
                && region_ != Region::HT_MinButton
                && region_ != Region::HT_MaxButton
                && region_ != Region::HT_CloseButton)
        {
            mouse_left_btn_pressed_ = true;
            mouse_left_btn_press_pos_ = event->pos();
        }else if(region_ == Region::HT_MinButton)
        {
            min_button_press_ = true;
        }else if(region_ == Region::HT_MaxButton)
        {
            max_buttton_press_ = true;
        }else if(region_ == Region::HT_CloseButton){
            close_button_press_ = true;
        }
    }
    else if(event->button() == Qt::RightButton)
    {
#ifdef Q_OS_WIN
        if(region_ == Region::HT_Caption){
            ::SendMessage((HWND)winId(), WM_NCRBUTTONDOWN, HTCAPTION,
                          MAKELPARAM(event->globalX(), event->globalY()));
        }
#endif
    }
}

void FramelessWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(!mouse_left_btn_pressed_){
        judgeRegion(event->pos());
    }else{
        QPoint gloPoint = event->globalPos();
        QRect rect = this->rect();
        QPoint tl = mapToGlobal(rect.topLeft());
        QPoint rb = mapToGlobal(rect.bottomRight());

        QRect rMove(tl, rb);
        switch (region_) {
        case Region::HT_Left:
        {
            if(rb.x() - gloPoint.x() <= this->minimumWidth())
                rMove.setX(tl.x());
            else
                rMove.setX(gloPoint.x());
            setGeometry(rMove);
        }
            break;
        case Region::HT_TopLeft:
        {
            if(rb.x() - gloPoint.x() <= this->minimumWidth())
                rMove.setX(tl.x());
            else
                rMove.setX(gloPoint.x());
            if(rb.y() - gloPoint.y() <= this->minimumHeight())
                rMove.setY(tl.y());
            else
                rMove.setY(gloPoint.y());
            setGeometry(rMove);
        }
            break;
        case Region::HT_Top:
        {
            if(rb.y() - gloPoint.y() <= this->minimumHeight())
                rMove.setY(tl.y());
            else
                rMove.setY(gloPoint.y());
            setGeometry(rMove);
        }
            break;
        case Region::HT_Caption:
        {
            move(event->pos() - mouse_left_btn_press_pos_ + pos());
        }
            break;
        case Region::HT_TopRight:
        {
            rMove.setWidth(gloPoint.x() - tl.x());
            rMove.setY(gloPoint.y());
            setGeometry(rMove);
        }
            break;
        case Region::HT_Right:
        {
            rMove.setWidth(gloPoint.x() - tl.x());
            setGeometry(rMove);
        }
            break;
        case Region::HT_BottomRight:
        {
            rMove.setWidth(gloPoint.x() - tl.x());
            rMove.setHeight(gloPoint.y() - tl.y());
            setGeometry(rMove);
        }
            break;
        case Region::HT_Bottom:
        {
            rMove.setHeight(gloPoint.y() - tl.y());
            setGeometry(rMove);
        }
            break;
        case Region::HT_BottomLeft:
        {
            rMove.setX(gloPoint.x());
            rMove.setHeight(gloPoint.y() - tl.y());
            setGeometry(rMove);
        }
            break;
        default:
            break;
        }
    }
    QWidget::mouseMoveEvent(event);
}

void FramelessWidget::mouseReleaseEvent(QMouseEvent *event)
{
    QWidget::mouseReleaseEvent(event);
    if(event->button() == Qt::LeftButton)
    {
        mouse_left_btn_pressed_ = false;
        mouse_left_btn_press_pos_ = QPoint();

        if(region_ == Region::HT_MinButton)
        {
            if(min_button_press_)
            {
                showMinimized();
                min_button_press_ = false;
            }
        }else if(region_ == Region::HT_MaxButton)
        {
            if(max_buttton_press_){
                if(this->windowState() == Qt::WindowNoState){
                    showMaximized();
                }else{
                    if(isMaximized()){
                        showNormal();
                    }
                }
                max_buttton_press_ = false;
            }
        }else if(region_ == Region::HT_CloseButton){
            if(close_button_press_){
                close();
                close_button_press_ = false;
            }
        }
    }
}

void FramelessWidget::leaveEvent(QEvent *event)
{
    clearButtonHover();
    setCursor(Qt::ArrowCursor);
    QWidget::leaveEvent(event);
}

void FramelessWidget::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::WindowStateChange){
        auto windowState = this->windowState();
        if(windowState == Qt::WindowMaximized || windowState == Qt::WindowFullScreen){
            setFrameWidth(0);
        }else{
            setFrameWidth(10);
        }
    }
}

void FramelessWidget::setFrameWidth(int width)
{
    if(width < 0){
        FrameWidth = 0;
    }
    FrameWidth = width;

    layout_->setContentsMargins(FrameWidth,
                                FrameWidth + CaptionHeight,
                                FrameWidth,
                                FrameWidth);
}

void FramelessWidget::judgeRegion(const QPoint &windowPos)
{
    clearButtonHover();

    if(QRect{0, FrameWidth, FrameWidth, height() - 2 * FrameWidth}.contains(windowPos))
    {
        region_ = Region::HT_Left;
        setCursor(Qt::SizeHorCursor);
    }else if(QRect{0, 0, FrameWidth + 4, FrameWidth + 4}.contains(windowPos))
    {
        region_ = Region::HT_TopLeft;
        setCursor(Qt::SizeFDiagCursor);
    }else if(QRect{FrameWidth, 0, width() - 2 * FrameWidth, FrameWidth}.contains(windowPos))
    {
        region_ = Region::HT_Top;
        setCursor(Qt::SizeVerCursor);
    }
    else if(QRect{FrameWidth, FrameWidth, width() - 2 * FrameWidth, CaptionHeight}.contains(windowPos))
    {
        if(btnRect(CaptionButtons::Button_Mini).contains(windowPos)){
            min_button_hover_ = true;
            region_ = Region::HT_MinButton;
        }else if(btnRect(CaptionButtons::Button_NormalMax).contains(windowPos))
        {
            max_button_hover_ = true;
            region_ = Region::HT_MaxButton;
        }else if(btnRect(CaptionButtons::Button_Close).contains(windowPos))
        {
            close_button_hover_ = true;
            region_ = Region::HT_CloseButton;
        }else{
            region_ = Region::HT_Caption;
        }
        update();
        setCursor(Qt::ArrowCursor);
    }
    else if(QRect{width() - FrameWidth, 0, FrameWidth, FrameWidth}.contains(windowPos))
    {
        region_ = Region::HT_TopRight;
        setCursor(Qt::SizeBDiagCursor);
    }
    else if(QRect{width() - FrameWidth, FrameWidth, FrameWidth, height() - 2 * FrameWidth}.contains(windowPos))
    {
        region_ = Region::HT_Right;
        setCursor(Qt::SizeHorCursor);
    }
    else if(QRect{width() - FrameWidth, height() - FrameWidth, FrameWidth, FrameWidth}.contains(windowPos))
    {
        region_ = Region::HT_BottomRight;
        setCursor(Qt::SizeFDiagCursor);
    }
    else if(QRect{FrameWidth, height() - FrameWidth, width() - 2 * FrameWidth, FrameWidth}.contains(windowPos))
    {
        region_ = Region::HT_Bottom;
        setCursor(Qt::SizeVerCursor);
    }
    else if(QRect{0, height() - FrameWidth, FrameWidth, FrameWidth}.contains(windowPos))
    {
        region_ = Region::HT_BottomLeft;
        setCursor(Qt::SizeBDiagCursor);
    }
    else
    {
        region_ = Region::HT_Client;
        setCursor(Qt::ArrowCursor);
    }
}

QRectF FramelessWidget::btnRect(FramelessWidget::CaptionButtons button)
{
    int BtnWidth = 45;
    int BtnHeight = 28;
    int BtnSpacing = 1;

//    int dpi = this->window()->logicalDpiX();
//    switch (dpi) {
//    case 96:
//        BtnWidth = 45;
//        BtnHeight = 30;
//        break;
//    case 120:
//        BtnWidth = 56;
//        BtnHeight = 40;
//        break;
//    case 144:
//        BtnWidth = 68;
//        BtnHeight = 42;
//        break;
//    case 168:
//        BtnWidth = 78;
//        BtnHeight = 52;
//        break;
//    default:
//        BtnWidth = 45;
//        BtnHeight = 28;
//        break;
//    }

    QRect captionRect{FrameWidth, FrameWidth, width() - 2 * FrameWidth, CaptionHeight};
    int y = captionRect.y();
    QRectF rectClose(captionRect.x() + captionRect.width() - BtnWidth,
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

    switch (button) {
    case CaptionButtons::Button_Close:
        return rectClose;
        case CaptionButtons::Button_NormalMax:
        return rectMax;
    case CaptionButtons::Button_Mini:
        return rectMini;
    default:
        break;
    }
    return QRect();

}

QPixmap FramelessWidget::btnPixmap(FramelessWidget::CaptionButtons button)
{
    QColor color(Qt::black);
    QSize size(10, 10);
    QPixmap pix(size);
    pix.fill(Qt::transparent);
    QSizeF sizeF(size.width() *1.0,  size.height() * 1.0);
    QPainter p(&pix);
    switch (button) {
    case CaptionButtons::Button_Mini:
    {
        p.save();
        QPen pen(QBrush(color), 1.0);
        p.setPen(pen);

        p.drawLine(QPointF(0, sizeF.height() / 2),
                   QPointF(sizeF.width(), sizeF.height() / 2));

        p.restore();
    }
        break;
    case CaptionButtons::Button_NormalMax:
    {
        // max
        p.save();
        QPen pen(QBrush(color), 1.0);
        p.setPen(pen);

        p.drawRect(QRectF(0.0, 0.0, sizeF.width() - 1.0, sizeF.height() - 1.0));

        p.restore();
    }
        break;
    case CaptionButtons::Button_Close:
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

FramelessWidget::CaptionButtons FramelessWidget::buttonAt(const QPoint &pos)
{
    if(btnRect(CaptionButtons::Button_Mini).contains(pos)){
        return CaptionButtons::Button_Mini;
    }else if(btnRect(CaptionButtons::Button_NormalMax).contains(pos)){
        return CaptionButtons::Button_NormalMax;
    }else if(btnRect(CaptionButtons::Button_Close).contains(pos)){
        return CaptionButtons::Button_Close;
    }
    return CaptionButtons::Button_None;
}

void FramelessWidget::drawShadow(QPainter *p)
{
    if(FrameWidth == 0){
        return;
    }
    p->save();
    QColor color(100, 100, 100, 30);
    for (int i = 0; i < FrameWidth; i++)
    {
        color.setAlpha(120 - qSqrt(i) * 40);
        p->setPen(color);
        p->drawRoundedRect(FrameWidth - i,
                                FrameWidth - i,
                                width() - (FrameWidth - i) * 2,
                                height() - (FrameWidth - i) * 2,
                                i,
                                i);
    }
    p->restore();
}

void FramelessWidget::drawButtons(QPainter *p)
{
    p->save();
    QRectF rectMini = btnRect(CaptionButtons::Button_Mini);
    QRectF rectMax = btnRect(CaptionButtons::Button_NormalMax);
    QRectF rectClose = btnRect(CaptionButtons::Button_Close);
    if(min_button_hover_){
        QColor color(0xB9B9B9);
        color.setAlphaF(0.7);
        p->fillRect(rectMini, color);
    }
    if(max_button_hover_){
        QColor color(0xB9B9B9);
        color.setAlphaF(0.7);
        p->fillRect(rectMax, color);
    }
    if(close_button_hover_){
        p->fillRect(rectClose, QColor(0xE81123));
    }
    QPixmap pix = btnPixmap(CaptionButtons::Button_Mini);
    p->drawPixmap(QPointF(rectMini.x() + (rectMini.width() - pix.width()) / 2,
                         rectMini.y() + (rectMini.height() - pix.height()) / 2),
                 pix);
    pix = btnPixmap(CaptionButtons::Button_NormalMax);
    p->drawPixmap(QPointF(rectMax.x() + (rectMax.width() - pix.width()) / 2,
                         rectMax.y() + (rectMax.height() - pix.height()) / 2),
                 pix);
    pix = btnPixmap(CaptionButtons::Button_Close);
    p->drawPixmap(QPointF(rectClose.x() + (rectClose.width() - pix.width()) / 2,
                         rectClose.y() + (rectClose.height() - pix.height()) / 2),
                 pix);

    p->restore();
}

void FramelessWidget::clearButtonHover()
{
    min_button_hover_ = false;
    max_button_hover_ = false;
    close_button_hover_ = false;
    update();
}

