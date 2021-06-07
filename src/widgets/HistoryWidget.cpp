#include "HistoryWidget.h"
#include "ui_HistoryWidget.h"

HistoryWidget::HistoryWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HistoryWidget)
{
    ui->setupUi(this);

    connect(ui->buttonSearch, &QToolButton::clicked,
            this, [this](){
        ui->stackedWidget->setCurrentWidget(ui->pageSearch);
    });

    connect(ui->buttonSearchBack, &QToolButton::clicked,
            this, [this](){
        ui->stackedWidget->setCurrentWidget(ui->pageGeneral);
    });

    connect(ui->buttonPinOrClose, &QToolButton::clicked,
            this, &HistoryWidget::pinOrCloseClicked);
}

HistoryWidget::~HistoryWidget()
{
    delete ui;
}

void HistoryWidget::onShowModeChanged(ToolWndShowMode mode)
{
    QIcon icon(":/icons/resources/imgs/pin_64px.png");
    QString tooltip(tr("pin to right"));
    if(mode == ToolWndShowMode::Dock){
        icon = QIcon(":/icons/resources/imgs/close_64px_2.png");
        tooltip = tr("close");
    }
    ui->buttonPinOrClose->setIcon(icon);
    ui->buttonPinOrClose->setToolTip(tooltip);
}
