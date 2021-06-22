#include "AppConfigWidget.h"
#include "ui_AppConfigWidget.h"

#include <QGraphicsDropShadowEffect>
#include <QButtonGroup>
#include <QtDebug>

AppCfgWidget::AppCfgWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AppCfgWidget)
{
//    setWindowFlags(Qt::Widget | Qt::Tool);
    ui->setupUi(this);

    QButtonGroup *naviBtnGroup = new QButtonGroup(this);
    naviBtnGroup->addButton(ui->btnAppearence);
    naviBtnGroup->addButton(ui->btnSearch);
    naviBtnGroup->addButton(ui->btnDownload);
    naviBtnGroup->addButton(ui->btnStartup);
    naviBtnGroup->addButton(ui->btnTabPage);

    naviBtnGroup->setExclusive(true);

    connect(ui->btnAppearence, &QPushButton::clicked, this, &AppCfgWidget::onNaviButtonClicked);
    connect(ui->btnDownload, &QPushButton::clicked, this, &AppCfgWidget::onNaviButtonClicked);
    connect(ui->btnStartup, &QPushButton::clicked, this, &AppCfgWidget::onNaviButtonClicked);
    connect(ui->btnTabPage, &QPushButton::clicked, this, &AppCfgWidget::onNaviButtonClicked);
    connect(ui->btnSearch, &QPushButton::clicked, this, &AppCfgWidget::onNaviButtonClicked);

    ui->btnAppearence->setChecked(true);
    ui->stackedWidget->setCurrentWidget(ui->pageAppearance);

    ui->scrollAreaPageApprearence->viewport()->setStyleSheet(".QWidget{background-color:white;}");

}

AppCfgWidget::~AppCfgWidget()
{
    qInfo()<<__FUNCTION__;
    delete ui;
}

void AppCfgWidget::onNaviButtonClicked()
{
    auto sender = QObject::sender();
    if(sender == ui->btnAppearence){
        ui->stackedWidget->setCurrentWidget(ui->pageAppearance);
    }else if(sender == ui->btnSearch){
        ui->stackedWidget->setCurrentWidget(ui->pageSearch);
    }else if(sender == ui->btnDownload){
        ui->stackedWidget->setCurrentWidget(ui->pageDownload);
    }else if(sender == ui->btnStartup){
        ui->stackedWidget->setCurrentWidget(ui->pageStartup);
    }else if(sender == ui->btnTabPage){
        ui->stackedWidget->setCurrentWidget(ui->pageNewTabPage);
    }
}
