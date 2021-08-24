#include "FindBar.h"
#include "ui_FindBar.h"

#include <QGraphicsDropShadowEffect>
#include <QtDebug>
#include <QKeyEvent>

FindBar::FindBar(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FindBar)
{
    ui->setupUi(this);
    initUi();
}

FindBar::~FindBar()
{
    qInfo()<<__FUNCTION__;
    delete ui;
}

void FindBar::setFindResult(int count, int index)
{
    QString result = QString::number(index);
    result.append("/");
    result.append(QString::number(count));
    ui->label->setText(result);
    ui->label->show();
}

void FindBar::closeFind()
{
    ui->label->setText("");
    ui->label->hide();
    emit search("", true);
    close();
    emit searchStop();
}

void FindBar::showEvent(QShowEvent *event)
{
    QDialog::showEvent(event);
    activateWindow();
    ui->lineEdit->setFocus();
}

void FindBar::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Escape){
        closeFind();
    }
    QDialog::keyPressEvent(event);
}

void FindBar::initUi()
{
    setWindowFlags(Qt::Tool | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);

    ui->frameShadow->setMinimumHeight(46 * devicePixelRatioF());
    auto effect_ = new QGraphicsDropShadowEffect(this);
    effect_->setBlurRadius(12);
    effect_->setOffset(0, 2);
    effect_->setColor(QColor(119,136,153,127));    //0x409eff
    ui->frameShadow->setGraphicsEffect(effect_);

    ui->label->hide();
    ui->buttonUp->setEnabled(!ui->lineEdit->text().isEmpty());
    ui->buttonDown->setEnabled(!ui->lineEdit->text().isEmpty());

    connect(ui->buttonClose, &QToolButton::clicked, this, &FindBar::closeFind);
    connect(ui->buttonUp, &QToolButton::clicked, this, &FindBar::onBtnUpClicked);
    connect(ui->buttonDown, &QToolButton::clicked, this, &FindBar::onBtnDownClicked);
    connect(ui->lineEdit, &QLineEdit::textChanged, this, &FindBar::onLineEditTextChanged);
    connect(ui->lineEdit, &QLineEdit::textEdited, this, &FindBar::onLineEditTextEdited);
    connect(ui->lineEdit, &QLineEdit::returnPressed, this, &FindBar::onLineEditReturnPressed);
}

void FindBar::onLineEditTextEdited(const QString &arg1)
{
    if(ui->lineEdit->text().isEmpty()){
        ui->label->setText("");
    }

    emit search(arg1, true);
}

void FindBar::onBtnUpClicked()
{
    emit searchUp(ui->lineEdit->text(), true);
}

void FindBar::onBtnDownClicked()
{
    emit searchDown(ui->lineEdit->text(), true);
}

void FindBar::onLineEditTextChanged(const QString &arg1)
{
    ui->buttonUp->setEnabled(!ui->lineEdit->text().isEmpty());
    ui->buttonDown->setEnabled(!ui->lineEdit->text().isEmpty());
}

void FindBar::onLineEditReturnPressed()
{
    emit searchDown(ui->lineEdit->text(), true);
}
