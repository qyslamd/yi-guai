#include "HistoryWidget.h"
#include "ui_HistoryWidget.h"

HistoryWidget::HistoryWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HistoryWidget)
{
    ui->setupUi(this);

    connect(ui->buttonPinOrClose, &QToolButton::clicked, this, &HistoryWidget::pinOrCloseClicked);
}

HistoryWidget::~HistoryWidget()
{
    delete ui;
}
