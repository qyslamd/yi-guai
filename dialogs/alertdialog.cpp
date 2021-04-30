#include "alertdialog.h"
#include "ui_alertdialog.h"

#include <QGraphicsDropShadowEffect>

AlertDialog::AlertDialog(const QString &title, const QString &msg, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AlertDialog)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_TranslucentBackground, true);
    ui->frameShadow->setAutoFillBackground(true);
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);

    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    shadow->setOffset(0, 0);
    shadow->setColor(QColor(38, 78, 119, 180));
    shadow->setBlurRadius(10);
    ui->frameShadow->setGraphicsEffect(shadow);

    ui->labelTitle->setText(title);
    ui->labelMsg->setText(msg);

    connect(ui->btnOK, &QPushButton::clicked, this, &AlertDialog::accept);
}

AlertDialog::~AlertDialog()
{
    delete ui;
}
