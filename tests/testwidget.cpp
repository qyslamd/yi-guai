#include "testwidget.h"
#include "ui_testwidget.h"

#include <QPalette>
#include <QLinearGradient>
#include <QColor>
#include <QPaintEvent>
#include <QPainter>
#include <QtMath>

#include "utils/util_qt.h"

#ifdef Q_OS_WIN
#include <Windows.h>
#include <QtWin>
#pragma comment(lib, "Gdi32.lib")
#endif

TestWidget::TestWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TestWidget)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setMouseTracking(true);
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
        mouse_left_btn_pressed_ = true;
        press_pos_ = QCursor::pos();
    }
    QRect dragRect{FrameWidth,
                FrameWidth,
                width() - 2 * FrameWidth,
                CaptionHeight};
    if(!dragRect.contains(event->pos())){
        return QWidget::mousePressEvent(event);
    }

    if(::ReleaseCapture()){
        SendMessage(HWND(this->window()->winId()),
                    WM_SYSCOMMAND,
                    SC_MOVE + HTCAPTION,
                    0);
        event->ignore();
    }
}

void TestWidget::mouseMoveEvent(QMouseEvent *event)
{
    QWidget::mouseMoveEvent(event);

    auto pos = event->pos();
    if(getNineSqureRect(NineSqure::Left).contains(pos))
    {
        setCursor(Qt::SizeHorCursor);
    }
    if(getNineSqureRect(NineSqure::TopLeft).contains(pos))
    {
        setCursor(Qt::SizeFDiagCursor);
    }
    if(getNineSqureRect(NineSqure::Top).contains(pos))
    {
        setCursor(Qt::SizeVerCursor);
    }
    if(getNineSqureRect(NineSqure::TopRight).contains(pos))
    {
        setCursor(Qt::SizeBDiagCursor);
    }
    if(getNineSqureRect(NineSqure::Right).contains(pos))
    {
        setCursor(Qt::SizeHorCursor);
    }
    if(getNineSqureRect(NineSqure::BottomRight).contains(pos))
    {
        setCursor(Qt::SizeFDiagCursor);
    }
    if(getNineSqureRect(NineSqure::Bottom).contains(pos))
    {
        setCursor(Qt::SizeVerCursor);
    }
    if(getNineSqureRect(NineSqure::BottomLeft).contains(pos))
    {
        setCursor(Qt::SizeBDiagCursor);
    }
    if(getNineSqureRect(NineSqure::Middle).contains(pos))
    {
        setCursor(Qt::ArrowCursor);
    }

//    if(mouse_left_btn_pressed_){
//        auto cursorPos = QCursor::pos();
//        auto windowPos = mapFromGlobal(cursorPos);
//        switch(curRect(windowPos)){
//        case NineSqure::Left:
//        {
//            auto rect = this->geometry();
//            rect.setX(rect.x() + press_pos_.x() - cursorPos.x());
//            setGeometry(rect);
//        }
//            break;
//        case NineSqure::TopLeft:
//            break;
//        case NineSqure::Top:
//            break;
//        case NineSqure::TopRight:
//            break;
//        case NineSqure::Right:
//            break;

//        case NineSqure::BottomRight:
//            break;
//        case NineSqure::Bottom:
//            break;
//        case NineSqure::BottomLeft:
//            break;
//        case NineSqure::Middle:
//            break;
//        }
//    }
}

void TestWidget::mouseReleaseEvent(QMouseEvent *event)
{
    QWidget::mouseReleaseEvent(event);
    if(event->button() == Qt::LeftButton){
        mouse_left_btn_pressed_ = false;
        press_pos_ = QPoint();
    }
}

void TestWidget::leaveEvent(QEvent *event)
{
    mouse_left_btn_pressed_ = false;
    press_pos_ = QPoint();
    QWidget::leaveEvent(event);
}

QRect TestWidget::getNineSqureRect(TestWidget::NineSqure type)
{
    switch (type) {
    case NineSqure::Left:
        return QRect{0, FrameWidth, FrameWidth, height() - 2 * FrameWidth};
    case NineSqure::TopLeft:
        return QRect{0, 0, FrameWidth, FrameWidth};
    case NineSqure::Top:
        return QRect{FrameWidth, 0, width() - 2 * FrameWidth, FrameWidth};
    case NineSqure::TopRight:
        return QRect{width() - FrameWidth, 0, FrameWidth, FrameWidth};
    case NineSqure::Right:
        return QRect{width() - FrameWidth, FrameWidth, FrameWidth, height() - 2 * FrameWidth};
    case NineSqure::BottomRight:
        return QRect{width() - FrameWidth, height() - FrameWidth, FrameWidth, FrameWidth};
    case NineSqure::Bottom:
        return QRect{FrameWidth, height() - FrameWidth, width() - 2 * FrameWidth, FrameWidth};
    case NineSqure::BottomLeft:
        return QRect{0, height() - FrameWidth, FrameWidth, FrameWidth};
    default:
        return QRect{FrameWidth, FrameWidth, width() - 2 * FrameWidth, height() - 2 * FrameWidth};
    }
}

TestWidget::NineSqure TestWidget::curRect(const QPoint &windowPos)
{
    auto pos =windowPos;
    if(getNineSqureRect(NineSqure::Left).contains(pos))
    {
        return NineSqure::Left;
    }
    if(getNineSqureRect(NineSqure::TopLeft).contains(pos))
    {
        return NineSqure::TopLeft;
    }
    if(getNineSqureRect(NineSqure::Top).contains(pos))
    {
        return NineSqure::Top;
    }
    if(getNineSqureRect(NineSqure::TopRight).contains(pos))
    {
        return NineSqure::TopRight;
    }
    if(getNineSqureRect(NineSqure::Right).contains(pos))
    {
        return NineSqure::Right;
    }
    if(getNineSqureRect(NineSqure::BottomRight).contains(pos))
    {
        return NineSqure::BottomRight;
    }
    if(getNineSqureRect(NineSqure::Bottom).contains(pos))
    {
        return NineSqure::Bottom;
    }
    if(getNineSqureRect(NineSqure::BottomLeft).contains(pos))
    {
        return NineSqure::BottomLeft;
    }
    return NineSqure::Middle;
}

