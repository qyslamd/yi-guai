#include "frame2.h"
#include "ui_frame2.h"

#include <QGraphicsDropShadowEffect>

Frame2::Frame2(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::Frame2)
{
    ui->setupUi(this);

//    setAttribute(Qt::WA_TranslucentBackground);

    auto effect = new QGraphicsDropShadowEffect;
    effect->setBlurRadius(20);
    effect->setColor(QColor(0, 0, 0, 50));
    effect->setOffset(0, 0);
    ui->frameDialog->setGraphicsEffect(effect);

    connect(ui->pushButton, &QPushButton::clicked, this, &Frame2::btnOkClicked);
    connect(ui->pushButton_2, &QPushButton::clicked, this, &Frame2::btnCancelClicked);
}

Frame2::~Frame2()
{
    delete ui;
}
