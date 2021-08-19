#include "pagedialog.h"
#include "ui_pagedialog.h"

#include <QGraphicsDropShadowEffect>

PageDialog::PageDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PageDialog)
{
    ui->setupUi(this);

    setWindowFlag(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);

    auto effect = new QGraphicsDropShadowEffect(this);
    effect->setOffset(0, 0);
    effect->setColor(QColor(0, 0, 0, 100));
    effect->setBlurRadius(20);
    ui->frame->setGraphicsEffect(effect);

    connect(ui->pushButton, &QPushButton::clicked, this, &PageDialog::btnOkClicked);
}

PageDialog::~PageDialog()
{
    delete ui;
}
