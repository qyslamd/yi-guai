#include "appconfigwidget.h"
#include "ui_appconfigwidget.h"

#include <QGraphicsDropShadowEffect>
#include <QButtonGroup>

AppCfgWidget::AppCfgWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AppCfgWidget)
{
    ui->setupUi(this);

    setWindowFlags(Qt::Widget | Qt::Tool);

    for(auto item : ui->frameNaviBtns->children()){
        if(item->isWidgetType() && item->metaObject()->className() == QString("QPushButton")){
            auto w = qobject_cast<QWidget*>(item);
            QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
            shadow->setOffset(0, 0);
            shadow->setColor(QColor(38, 78, 119, 127));
            shadow->setBlurRadius(9);
            w->setGraphicsEffect(shadow);
        }
    }

    QButtonGroup *naviBtnGroup = new QButtonGroup(this);
    naviBtnGroup->addButton(ui->btnAppearence);
    naviBtnGroup->addButton(ui->btnDownload);
    naviBtnGroup->addButton(ui->btnPersonal);
    naviBtnGroup->addButton(ui->btnPrivacy);
    naviBtnGroup->addButton(ui->btnStartup);
    naviBtnGroup->addButton(ui->btnTabPage);

    naviBtnGroup->setExclusive(true);

    connect(ui->btnAppearence, &QPushButton::clicked, this, &AppCfgWidget::onNaviButtonClicked);
    connect(ui->btnDownload, &QPushButton::clicked, this, &AppCfgWidget::onNaviButtonClicked);
    connect(ui->btnPersonal, &QPushButton::clicked, this, &AppCfgWidget::onNaviButtonClicked);
    connect(ui->btnPrivacy, &QPushButton::clicked, this, &AppCfgWidget::onNaviButtonClicked);
    connect(ui->btnStartup, &QPushButton::clicked, this, &AppCfgWidget::onNaviButtonClicked);
    connect(ui->btnTabPage, &QPushButton::clicked, this, &AppCfgWidget::onNaviButtonClicked);

    ui->btnAppearence->setChecked(true);
    ui->stackedWidget->setCurrentWidget(ui->pageAppearence);

    ui->scrollAreaPageApprearence->viewport()->setStyleSheet(".QWidget{background-color:white;}");

}

AppCfgWidget::~AppCfgWidget()
{
    delete ui;
}

void AppCfgWidget::onNaviButtonClicked()
{
    auto sender = QObject::sender();
    if(sender == ui->btnAppearence){
        ui->stackedWidget->setCurrentWidget(ui->pageAppearence);
    }else if(sender == ui->btnDownload){
        ui->stackedWidget->setCurrentWidget(ui->pageDownload);
    }else if(sender == ui->btnPersonal){
        ui->stackedWidget->setCurrentWidget(ui->pagePersonal);
    }else if(sender == ui->btnPrivacy){
        ui->stackedWidget->setCurrentWidget(ui->pagePrivacy);
    }else if(sender == ui->btnStartup){
        ui->stackedWidget->setCurrentWidget(ui->pageStartup);
    }else if(sender == ui->btnTabPage){
        ui->stackedWidget->setCurrentWidget(ui->pageNewTabPage);
    }
}
