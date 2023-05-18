#include "PopupBase.h"
#include "ui_PopupBase.h"

#include <QGraphicsDropShadowEffect>
#include <QMouseEvent>
#include <QtDebug>

PopupBase::PopupBase(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::PopupBase)
{
    ui->setupUi(this);
    ui->frameShadow->setAutoFillBackground(true);
    setWindowFlags(Qt::Popup | Qt::FramelessWindowHint);
#ifdef Q_OS_WIN
    setAttribute(Qt::WA_TranslucentBackground, true);
    setWindowFlag(Qt::NoDropShadowWindowHint, true);
#endif

    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    shadow->setOffset(0, 0);
    shadow->setColor(QColor(	105,105,105, 100)); // 38, 78, 119
    shadow->setBlurRadius(20);
    ui->frameShadow->setGraphicsEffect(shadow);
}

QWidget *PopupBase::contentFrame()
{
    return ui->frameShadow;
}

int PopupBase::shadowWidth() const
{
    return ui->verticalLayout->contentsMargins().left()
            +ui->verticalLayout->contentsMargins().right();
}

int PopupBase::shadowLeftWidth() const
{
    return ui->verticalLayout->contentsMargins().left();
}

int PopupBase::shadowRightWidth() const
{
    return ui->verticalLayout->contentsMargins().right();
}

int PopupBase::shadowBottomWidth() const
{
    return ui->verticalLayout->contentsMargins().bottom();
}

PopupBase::~PopupBase()
{
    delete ui;
}


void PopupBase::mousePressEvent(QMouseEvent *event)
{
    setAttribute(Qt::WA_NoMouseReplay);
    QFrame::mousePressEvent(event);
}
