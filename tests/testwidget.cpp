#include "testwidget.h"
#include "ui_testwidget.h"

#include <QtDebug>
#include <QPalette>
#include <QLinearGradient>
#include <QColor>
#include <QPaintEvent>
#include <QPainter>
#include <QtMath>

#include "utils/util_qt.h"

TestWidget::TestWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TestWidget)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setMouseTracking(true);

    setMinimumSize(300,300);
}

TestWidget::~TestWidget()
{
    delete ui;
}

void TestWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.fillRect(QRect(FrameWidth, FrameWidth,
                           this->width() - 2 * FrameWidth,
                           this->height() - 2 * FrameWidth),
                     QBrush(Qt::white));

    QColor color(100, 100, 100, 30);
    for (int i = 0; i < FrameWidth; i++)
    {
        color.setAlpha(120 - qSqrt(i) * 40);
        painter.setPen(color);
#if 0
        painter.drawRect(SHADOW_WIDTH - i,
                         SHADOW_WIDTH - i,
                         device->width() - (SHADOW_WIDTH - i) * 2,
                         device->height() - (SHADOW_WIDTH - i) * 2);
#else
        painter.drawRoundedRect(FrameWidth - i,
                                FrameWidth - i,
                                this->width() - (FrameWidth - i) * 2,
                                this->height() - (FrameWidth - i) * 2,
                                i,
                                i);
#endif
    }
}

void TestWidget::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        if(region_ != TenRegion::Middle){
            mouse_left_btn_pressed_ = true;
            mouse_left_btn_press_pos_ = QCursor::pos();
        }
    }

    QRect dragRect{FrameWidth,
                FrameWidth,
                width() - 2 * FrameWidth,
                CaptionHeight};
    if(!dragRect.contains(event->pos())){
        return QWidget::mousePressEvent(event);
    }
}

void TestWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(!mouse_left_btn_pressed_){
        judgeRegionChangeCursor(event->pos());
        return QWidget::mouseMoveEvent(event);
    }

    QPoint gloPoint = event->globalPos();
    QRect rect = this->rect();
    QPoint tl = mapToGlobal(rect.topLeft());
    QPoint rb = mapToGlobal(rect.bottomRight());

    QRect rMove(tl, rb);
    switch (region_) {
    case TenRegion::Left:
    {
        if(rb.x() - gloPoint.x() <= this->minimumWidth())
            rMove.setX(tl.x());
        else
            rMove.setX(gloPoint.x());
    }
        break;
    case TenRegion::TopLeft:
    {
        if(rb.x() - gloPoint.x() <= this->minimumWidth())
            rMove.setX(tl.x());
        else
            rMove.setX(gloPoint.x());
        if(rb.y() - gloPoint.y() <= this->minimumHeight())
            rMove.setY(tl.y());
        else
            rMove.setY(gloPoint.y());
    }
        break;
    case TenRegion::Top:
    {
        if(rb.y() - gloPoint.y() <= this->minimumHeight())
            rMove.setY(tl.y());
        else
            rMove.setY(gloPoint.y());
    }
        break;
    case TenRegion::TopRight:
    {
        rMove.setWidth(gloPoint.x() - tl.x());
        rMove.setY(gloPoint.y());
    }
        break;
    case TenRegion::Right:
    {
        rMove.setWidth(gloPoint.x() - tl.x());
    }
        break;
    case TenRegion::BottomRight:
    {
        rMove.setWidth(gloPoint.x() - tl.x());
        rMove.setHeight(gloPoint.y() - tl.y());
    }
        break;
    case TenRegion::Bottom:
    {
        rMove.setHeight(gloPoint.y() - tl.y());
    }
        break;
    case TenRegion::BottomLeft:
    {
        rMove.setX(gloPoint.x());
        rMove.setHeight(gloPoint.y() - tl.y());
    }
        break;
    default:
        break;
    }

    setGeometry(rMove);
    QWidget::mouseMoveEvent(event);
}

void TestWidget::mouseReleaseEvent(QMouseEvent *event)
{
    QWidget::mouseReleaseEvent(event);
    if(event->button() == Qt::LeftButton){
        mouse_left_btn_pressed_ = false;
        mouse_left_btn_press_pos_ = QPoint();
    }
}

void TestWidget::leaveEvent(QEvent *event)
{
//    mouse_left_btn_pressed_ = false;
//    press_pos_ = QPoint();
    QWidget::leaveEvent(event);
}

void TestWidget::judgeRegionChangeCursor(const QPoint &posInThis)
{
    if(QRect{0, FrameWidth, FrameWidth, height() - 2 * FrameWidth}.contains(posInThis))
    {
        region_ = TenRegion::Left;
        setCursor(Qt::SizeHorCursor);
    }else if(QRect{0, 0, FrameWidth, FrameWidth}.contains(posInThis))
    {
        region_ = TenRegion::TopLeft;
        setCursor(Qt::SizeFDiagCursor);
    }else if(QRect{FrameWidth, 0, width() - 2 * FrameWidth, FrameWidth}.contains(posInThis))
    {
        setCursor(Qt::SizeVerCursor);
    }
    else if(QRect{width() - FrameWidth, 0, FrameWidth, FrameWidth}.contains(posInThis))
    {
        region_ = TenRegion::TopRight;
        setCursor(Qt::SizeBDiagCursor);
    }
    else if(QRect{width() - FrameWidth, FrameWidth, FrameWidth, height() - 2 * FrameWidth}.contains(posInThis))
    {
        region_ = TenRegion::Right;
        setCursor(Qt::SizeHorCursor);
    }
    else if(QRect{width() - FrameWidth, height() - FrameWidth, FrameWidth, FrameWidth}.contains(posInThis))
    {
        region_ = TenRegion::BottomRight;
        setCursor(Qt::SizeFDiagCursor);
    }
    else if(QRect{FrameWidth, height() - FrameWidth, width() - 2 * FrameWidth, FrameWidth}.contains(posInThis))
    {
        region_ = TenRegion::Bottom;
        setCursor(Qt::SizeVerCursor);
    }
    else if(QRect{0, height() - FrameWidth, FrameWidth, FrameWidth}.contains(posInThis))
    {
        region_ = TenRegion::BottomLeft;
        setCursor(Qt::SizeBDiagCursor);
    }
    else
    {
        region_ = TenRegion::Middle;
        setCursor(Qt::ArrowCursor);
    }
}

