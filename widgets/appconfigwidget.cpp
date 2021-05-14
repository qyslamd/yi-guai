#include "appconfigwidget.h"
#include "ui_appconfigwidget.h"

#include <QGraphicsDropShadowEffect>

AppCfgWidget::AppCfgWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AppCfgWidget)
{
    ui->setupUi(this);

    setWindowFlags(Qt::Widget | Qt::Tool);

//    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
//    shadow->setOffset(0, 0);
//    shadow->setColor(QColor(38, 78, 119, 127));
//    shadow->setBlurRadius(16);
//    ui->frameContentShadow->setGraphicsEffect(shadow);
}

AppCfgWidget::~AppCfgWidget()
{
    delete ui;
}
