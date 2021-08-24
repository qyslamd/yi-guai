#include "aboutdialog.h"
#include "ui_aboutdialog.h"

#include <QGraphicsDropShadowEffect>
#include <QPropertyAnimation>
#include <QTimer>

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);

    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);

    auto effect = new QGraphicsDropShadowEffect(this);
    effect->setBlurRadius(20);
    effect->setOffset(0, 0);
    effect->setColor(QColor(0x409eff));
    ui->frame->setGraphicsEffect(effect);

    connect(ui->toolButton, &QToolButton::clicked, this, [this]()
    {
        moveFrame(ui->frame->pos(),
                  QPoint((ui->frameShadow->width() - ui->frame->width()) / 2, -ui->frame->height()));
        need_close_ = true;
    });

    anime_ = new QPropertyAnimation(ui->frame, "pos", this);
    anime_->setDuration(300);
    anime_->setLoopCount(1);
    connect(anime_, &QPropertyAnimation::finished, this, [this](){
        if(need_close_){
            close();
        }
    });

    ui->frame->move(- ui->frame->width(),
                    (ui->frameShadow->height() - ui->frame->height()) / 2);
}

AboutDialog::~AboutDialog()
{
    delete ui;
}

void AboutDialog::showEvent(QShowEvent *event)
{
    QDialog::showEvent(event);
    QTimer::singleShot(100, [this](){
        int y = (ui->frameShadow->height() - ui->frame->height()) / 2;
        moveFrame(QPoint(- ui->frame->width(), y),
                  QPoint((ui->frameShadow->width() - ui->frame->width()) / 2, y));
    });
}

void AboutDialog::moveFrame(const QPoint &start, const QPoint &end, int duration)
{
    anime_->setStartValue(start);
    anime_->setEndValue(end);
    anime_->setDuration(duration);
    anime_->start();
    need_close_ = false;
}
