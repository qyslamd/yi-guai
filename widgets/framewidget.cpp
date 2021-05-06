#include "framewidget.h"
#include "ui_captionwidget.h"

#include <QGraphicsDropShadowEffect>
#include <QMouseEvent>

FrameWidget::FrameWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CaptionWidget)
{
    ui->setupUi(this);
    setMouseTracking(true);

    setAttribute(Qt::WA_TranslucentBackground, true);
    ui->widgetShadow->setAutoFillBackground(true);
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint);

    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    shadow->setOffset(0, 0);
    shadow->setColor(Qt::gray);
    shadow->setBlurRadius(Padding_);
    ui->widgetShadow->setGraphicsEffect(shadow);

    isLeftPressDown = false;
    dir = NONE;
    this->setMinimumHeight(100);
    this->setMinimumWidth(150);
}

FrameWidget::~FrameWidget()
{
    delete ui;
}

void FrameWidget::region(const QPoint &cursorGlobalPoint)
{
    QRect rect = this->rect();
    QPoint tl = mapToGlobal(rect.topLeft());
    QPoint rb = mapToGlobal(rect.bottomRight());

    int x = cursorGlobalPoint.x();
    int y = cursorGlobalPoint.y();

    if(tl.x() + Padding_ >= x && tl.x() <= x && tl.y() + Padding_ >= y && tl.y() <= y) {
        // 左上角
        dir = LEFTTOP;
        this->setCursor(QCursor(Qt::SizeFDiagCursor));  // 设置鼠标形状
    } else if(x >= rb.x() - Padding_ && x <= rb.x() && y >= rb.y() - Padding_ && y <= rb.y()) {
        // 右下角
        dir = RIGHTBOTTOM;
        this->setCursor(QCursor(Qt::SizeFDiagCursor));
    } else if(x <= tl.x() + Padding_ && x >= tl.x() && y >= rb.y() - Padding_ && y <= rb.y()) {
        //左下角
        dir = LEFTBOTTOM;
        this->setCursor(QCursor(Qt::SizeBDiagCursor));
    } else if(x <= rb.x() && x >= rb.x() - Padding_ && y >= tl.y() && y <= tl.y() + Padding_) {
        // 右上角
        dir = RIGHTTOP;
        this->setCursor(QCursor(Qt::SizeBDiagCursor));
    } else if(x <= tl.x() + Padding_ && x >= tl.x()) {
        // 左边
        dir = LEFT;
        this->setCursor(QCursor(Qt::SizeHorCursor));
    } else if( x <= rb.x() && x >= rb.x() - Padding_) {
        // 右边
        dir = RIGHT;
        this->setCursor(QCursor(Qt::SizeHorCursor));
    }else if(y >= tl.y() && y <= tl.y() + Padding_){
        // 上边
        dir = UP;
        this->setCursor(QCursor(Qt::SizeVerCursor));
    } else if(y <= rb.y() && y >= rb.y() - Padding_) {
        // 下边
        dir = DOWN;
        this->setCursor(QCursor(Qt::SizeVerCursor));
    }else {
        // 默认
        dir = NONE;
        this->setCursor(QCursor(Qt::ArrowCursor));
    }
}

void FrameWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
}

void FrameWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton) {
        if(isLeftPressDown){
            isLeftPressDown = false;
            dir = NONE;
            setCursor(QCursor(Qt::ArrowCursor));
        }
    }
}

void FrameWidget::leaveEvent(QEvent *event)
{
    dir = NONE;
    setCursor(QCursor(Qt::ArrowCursor));
}

void FrameWidget::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        if(dir != NONE) {
            isLeftPressDown = true;
        }
    }
    QWidget::mousePressEvent(event);

//    switch(event->button())
//    {
//    case Qt::LeftButton:
//        isLeftPressDown = true;
//        if(dir != NONE) {
//            mouseGrabber();
//        } else {
//            dragPosition = event->globalPos() - this->frameGeometry().topLeft();
//        }
//        break;
//    case Qt::RightButton:
//        this->close();
//        break;
//    default:
//        QWidget::mousePressEvent(event);
//    }
}

void FrameWidget::mouseMoveEvent(QMouseEvent *event)
{
    QPoint gloPoint = event->globalPos();
    QRect rect = this->rect();
    QPoint tl = mapToGlobal(rect.topLeft());
    QPoint rb = mapToGlobal(rect.bottomRight());

    if(!isLeftPressDown) {
        this->region(gloPoint);
    } else {

        if(dir != NONE) {
            QRect rMove(tl, rb);

            switch(dir) {
            case LEFT:
                if(rb.x() - gloPoint.x() <= this->minimumWidth())
                    rMove.setX(tl.x());
                else
                    rMove.setX(gloPoint.x());
                break;
            case RIGHT:
                rMove.setWidth(gloPoint.x() - tl.x());
                break;
            case UP:
                if(rb.y() - gloPoint.y() <= this->minimumHeight())
                    rMove.setY(tl.y());
                else
                    rMove.setY(gloPoint.y());
                break;
            case DOWN:
                rMove.setHeight(gloPoint.y() - tl.y());
                break;
            case LEFTTOP:
                if(rb.x() - gloPoint.x() <= this->minimumWidth())
                    rMove.setX(tl.x());
                else
                    rMove.setX(gloPoint.x());
                if(rb.y() - gloPoint.y() <= this->minimumHeight())
                    rMove.setY(tl.y());
                else
                    rMove.setY(gloPoint.y());
                break;
            case RIGHTTOP:
                rMove.setWidth(gloPoint.x() - tl.x());
                rMove.setY(gloPoint.y());
                break;
            case LEFTBOTTOM:
                rMove.setX(gloPoint.x());
                rMove.setHeight(gloPoint.y() - tl.y());
                break;
            case RIGHTBOTTOM:
                rMove.setWidth(gloPoint.x() - tl.x());
                rMove.setHeight(gloPoint.y() - tl.y());
                break;
            default:
                break;
            }
            this->setGeometry(rMove);
        } else {
            move(event->globalPos() - dragPosition);
            event->accept();
        }
    }
    QWidget::mouseMoveEvent(event);
}
