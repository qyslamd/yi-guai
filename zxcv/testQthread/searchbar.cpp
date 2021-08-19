#include "searchbar.h"
#include "ui_searchbar.h"
#include <QGraphicsDropShadowEffect>
#include <QtDebug>

SearchBar::SearchBar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SearchBar)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint | Qt::ToolTip | Qt::SubWindow);
    setAttribute(Qt::WA_TranslucentBackground);

    using Shadow = QGraphicsDropShadowEffect;
    auto effect = new Shadow;
    effect->setBlurRadius(11);
    effect->setColor(QColor(0, 0, 0, 50));
    effect->setOffset(0, 0);
    ui->frameShadow->setGraphicsEffect(effect);
}

SearchBar::~SearchBar()
{
    delete ui;
}
