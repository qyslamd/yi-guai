#include "testwidget.h"
#include "ui_testwidget.h"

#include <QPalette>
#include <QLinearGradient>
#include <QColor>

TestWidget::TestWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TestWidget)
{
    ui->setupUi(this);

    QPalette pl = ui->widget_2->palette();
    QBrush brush;
    QRectF rect = ui->widget_2->rect();
    QLinearGradient linearGrad(rect.topLeft(),rect.bottomRight());
    linearGrad.setColorAt(0, 0x0C7BB3);
    linearGrad.setColorAt(1, 0xF2BAE8);
    brush = QBrush(linearGrad); // 拷贝赋值
    pl.setBrush(QPalette::Button, brush);
    pl.setBrush(QPalette::Window, brush);
    pl.setBrush(QPalette::Base, brush);
    ui->widget_2->setPalette(pl);
}

TestWidget::~TestWidget()
{
    delete ui;
}
