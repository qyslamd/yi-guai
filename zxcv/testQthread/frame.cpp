#include "frame.h"
#include "ui_frame.h"

Frame::Frame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::Frame)
{
    ui->setupUi(this);

    connect(ui->pushButton, &QPushButton::clicked, this, &Frame::btnClicked);
}

Frame::~Frame()
{
    delete ui;
}
