#include "HistoryWidget.h"
#include "ui_HistoryWidget.h"

#include "managers/HistoryManager.h"
#include "managers/FaviconManager.h"
#include "globaldef.h"

#include <QStandardItemModel>
#include <QDateTime>
#include <QStyle>

HistoryWidget::HistoryWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HistoryWidget)
{
    ui->setupUi(this);
    initUi();
    initSignalSlots();

    loadAllHistories();
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

void HistoryWidget::initUi()
{
    all_model_ = new QStandardItemModel(ui->treeViewAll);
    ui->treeViewAll->setModel(all_model_);
}

void HistoryWidget::initSignalSlots()
{

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

void HistoryWidget::loadAllHistories()
{
    auto allHistories = HistoryMgr::Instance().allHistories();

    QIcon icon(":/icons/resources/imgs/one/time_machine_64px.png");
//    QDateTime now = QDateTime::currentDateTime();
//    QDate today = QDate::currentDate();
//    QDate yestoday = today.addDays(-1);

    //一级节点，目前只处理今天和昨天
    QStandardItem* todayItem = new QStandardItem(icon, tr("Today"));
    QStandardItem* yestodayItem = new QStandardItem(icon, tr("Yestoday"));
    all_model_->appendRow(todayItem);
    all_model_->appendRow(yestodayItem);


    for(auto &data : allHistories){
        QIcon icon(FaviconMgr::Instance().iconFilePath(data.url));
        if(icon.isNull()){
            icon = style()->standardIcon(QStyle::SP_FileIcon);
        }
        QStandardItem *item = new QStandardItem(icon, data.title);
        item->setToolTip(data.title + "\n" + data.url);
        todayItem->appendRow(item);
    }
}
