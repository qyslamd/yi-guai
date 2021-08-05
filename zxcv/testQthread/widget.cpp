#include "widget.h"
#include "ui_widget.h"
#include <QtDebug>
#include <QTimer>
#include <QCloseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QtMath>

#include "searchbar.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    connect(ui->verticalSlider, &QSlider::valueChanged, this, [this](int)
    {
        update();
    });

    search_bar_ = new SearchBar(this);
}

Widget::~Widget()
{
    delete ui;
    delete search_bar_;
}

bool Widget::event(QEvent *ev)
{
    switch (ev->type())
    {
    case QEvent::Resize:
    case QEvent::Move: {
        if (search_bar_ && search_bar_->isVisible()) {
            auto pos = ui->pushButton->mapToGlobal(ui->pushButton->rect().topLeft());
            search_bar_->move(pos + QPoint(0, ui->pushButton->height()));
        }
    }
    default:
        break;
    }

    return QWidget::event(ev);
}

bool Widget::eventFilter(QObject *obj, QEvent *event)
{
    return QWidget::eventFilter(obj, event);
}

void Widget::paintEvent(QPaintEvent *)
{
    if (search_bar_ && search_bar_->isVisible()) {
        auto pos = ui->pushButton->mapToGlobal(ui->pushButton->rect().topLeft());
        search_bar_->move(pos + QPoint(0, ui->pushButton->height()));
    }

    auto DD = ui->verticalSlider->value() * 1.0;
    auto RR = DD / 2.0;
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    QRectF rect(10.0,
                10.0,
                DD,
                DD);
    p.save();
    p.setBrush(QColor(0xFFFFFF));
    p.drawEllipse(rect);
    p.restore();

    auto dd = DD / 3.0; // 控制小圆的大小

    QRectF cir1(rect.x() + RR - dd / 2.0,
                rect.y(),
                dd,
                dd);
    QPointF p1(rect.x() + RR, rect.y());
    p.save();
    p.setBrush(QColor(0x6A5ACD));
    p.drawEllipse(cir1);
    p.restore();

    QRectF cir2(QPointF(rect.x() + (DD - qSqrt(3) * RR) / 2.0,
                        rect.y() + RR + RR / 2.0 - dd / 2.0),
                QPointF(rect.x() + (DD - qSqrt(3) * RR) / 2.0 + dd,
                        rect.y() + RR + RR / 2.0 + dd / 2.0));
    QPointF p2(rect.x() + (DD - qSqrt(3) * RR) / 2.0, rect.y() + RR + RR / 2.0);
    p.save();
    p.setBrush(QColor(0x00FF00));
    p.drawEllipse(cir2);
    p.restore();

    QRectF cir3(rect.x() + (DD - qSqrt(3) * RR) / 2.0 + qSqrt(3) * RR - dd + 1,
                rect.y() + DD - DD / 4.0 - dd / 2.0 - dd / 2.0 / 2.0,
                dd,
                dd);
    QPointF p3(rect.x() + (DD - qSqrt(3) * RR) / 2.0 + qSqrt(3) * RR, rect.y() + RR + RR / 2.0);
    p.save();
    p.setBrush(QColor(0x0000FF));
    p.drawEllipse(cir3);
    p.restore();


    p.save();
    p.setPen(Qt::red);
    p.drawLine(p1, p2);
    p.drawLine(p2, p3);
    p.drawLine(p3, p1);
    p.restore();
}

void Widget::on_pushButton_clicked()
{
    auto pos = ui->pushButton->mapToGlobal(ui->pushButton->rect().topLeft());
    search_bar_->move(pos + QPoint(0, ui->pushButton->height()));
    search_bar_->show();
}
