#include "TabThumbnailWidget.h"
#include "ui_TabThumbnailWidget.h"

#include <QGraphicsDropShadowEffect>

Tab_Thumbnail_Widget::Tab_Thumbnail_Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Tab_Thumbnail_Widget)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint | Qt::ToolTip);
    setAttribute(Qt::WA_TranslucentBackground, true);

    setWindowFlags(Qt::ToolTip | Qt::FramelessWindowHint);
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    shadow->setOffset(0, 5);
    shadow->setColor(QColor(38, 78, 119, 127));
    shadow->setBlurRadius(10);
    ui->frameShadow->setGraphicsEffect(shadow);
}

Tab_Thumbnail_Widget::~Tab_Thumbnail_Widget()
{
    delete ui;
}

void Tab_Thumbnail_Widget::setDescriptionTxt(const QString &text)
{
    ui->labelDesc->setText(text);
}

void Tab_Thumbnail_Widget::setImgVisible(bool visible)
{
    ui->labelPix->setVisible(visible);
}

void Tab_Thumbnail_Widget::setThumbnailImg(const QPixmap &pix)
{
    auto img = pix.scaled(ui->labelPix->size(),Qt::IgnoreAspectRatio,
                          Qt::SmoothTransformation);
    ui->labelPix->setPixmap(img);
}
