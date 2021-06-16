#include "HistoryWidget.h"
#include "ui_HistoryWidget.h"

#include "managers/HistoryManager.h"
#include "managers/FaviconManager.h"
#include "globaldef.h"
#include "mainwindow.h"

#include <QtDebug>
#include <QStandardItemModel>
#include <QClipboard>
#include <QKeySequence>
#include <QDateTime>
#include <QStyle>
#include <QTimer>
#include "popups/StyledMenu.h"

HistoryWidget::HistoryWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HistoryWidget)
{
    ui->setupUi(this);
    initUi();
    initSignalSlots();
    setIcons();
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

bool HistoryWidget::eventFilter(QObject *obj, QEvent *ev)
{
    if(obj == ui->listViewRecently){
        if(ev->type() == QEvent::Show)
        {
            loadRecentlyHistories();
        }
    }
    return QWidget::eventFilter(obj, ev);
}

void HistoryWidget::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    loadAllHistories();
}

void HistoryWidget::initUi()
{
    ui->treeViewAll->setContextMenuPolicy(Qt::CustomContextMenu);
    all_model_ = new QStandardItemModel(ui->treeViewAll);
    ui->treeViewAll->setModel(all_model_);

    ui->listViewRecently->installEventFilter(this);
    ui->listViewRecently->setContextMenuPolicy(Qt::CustomContextMenu);
    recently_model_ = new QStandardItemModel(ui->listViewRecently);
    ui->listViewRecently->setModel(recently_model_);

    menu_in_all_ = new StyledMenu;
    menu_in_all_->setObjectName("HisWidgetAllHisMenu");
    ac_open_ = new QAction(tr("Open"), this);
    ac_open_in_new_tab_ = new QAction(tr("Open in new tab"), this);
    ac_open_in_new_window_ = new QAction(tr("Open in new window"), this);
    ac_open_in_inpriavte_ = new QAction(tr("Open in new Inprivate"), this);
    ac_delete_ = new QAction(tr("Delete the record"), this);
    ac_copy_link_ = new QAction(tr("Copy link"), this);
    ac_add2favorite_ = new QAction(tr("Add to favorite"), this);

    ui->tabWidget->setCurrentWidget(ui->tabAll);
}

void HistoryWidget::initSignalSlots()
{
    connect(menu_in_all_, &QMenu::aboutToShow, this, [this](){
        auto size = menu_in_all_->size();
        menu_in_all_->resize(size);
    });
    connect(ui->treeViewAll, &QTreeView::clicked,
            this, &HistoryWidget::onTreeAllHisItemClicked);
    connect(ui->treeViewAll, &QTreeView::customContextMenuRequested,
            this, &HistoryWidget::onTreeAllHisContextMenu);
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

    connect(ac_open_, &QAction::triggered, this, [this](){
        emit menuCmd(HistoryCmd::Open, menu_trigger_data_.url);
    });
    connect(ac_open_in_new_tab_, &QAction::triggered, this, [this](){
        emit menuCmd(HistoryCmd::OpenInNewPage, menu_trigger_data_.url);
    });
    connect(ac_open_in_new_window_, &QAction::triggered, this, [this](){
        emit menuCmd(HistoryCmd::OpenInNewWnd, menu_trigger_data_.url);
    });
    connect(ac_open_in_inpriavte_, &QAction::triggered, this, [this](){
        emit menuCmd(HistoryCmd::OpenInInprivate, menu_trigger_data_.url);
    });
}

void HistoryWidget::setIcons()
{
    ac_open_->setIcon(QIcon(""));
    ac_open_in_new_tab_->setIcon(QIcon(":/icons/resources/imgs/light/add_tab_64px.png"));
    ac_open_in_new_window_->setIcon(QIcon(":/icons/resources/imgs/light/new_window_64px.png"));
    ac_open_in_inpriavte_->setIcon(QIcon(""));
    ac_delete_->setIcon(QIcon(":/icons/resources/imgs/light/trash_64px.png"));
    ac_copy_link_->setIcon(QIcon(":/icons/resources/imgs/light/link_64px.png"));
    ac_add2favorite_->setIcon(QIcon(":/icons/resources/imgs/light/add_to_favorites_64px.png"));
}

void HistoryWidget::loadAllHistories()
{
    auto allHistories = HistoryMgr::Instance().allHistories();
    QIcon icon(":/icons/resources/imgs/light/time_machine_64px.png");
    QDateTime now = QDateTime::currentDateTime();
    QDate today = QDate::currentDate();
    QDate yestoday = today.addDays(-1);

    all_model_->clear();

    //一级节点，今天、昨天和过去一周
    QStandardItem* todayItem = new QStandardItem(icon, tr("Today"));
    todayItem->setData(false, Qt::UserRole + 1);
    QStandardItem* yestodayItem = new QStandardItem(icon, tr("Yestoday"));
    yestodayItem->setData(false, Qt::UserRole + 1);
    QStandardItem* weekItem = new QStandardItem(icon, tr("last week"));
    weekItem->setData(false, Qt::UserRole + 1);

    all_model_->appendRow(todayItem);
    all_model_->appendRow(yestodayItem);
    all_model_->appendRow(weekItem);


    for(auto &data : allHistories){
        QIcon icon(FaviconMgr::Instance().iconFilePath(data.url));
        if(icon.isNull()){
            icon = style()->standardIcon(QStyle::SP_MessageBoxInformation);
        }
        QStandardItem *item = new QStandardItem(icon, data.title);
        item->setData(true, Qt::UserRole + 1);
        item->setData(QVariant::fromValue(data), Qt::UserRole + 2);
        item->setToolTip(data.title + "\n" + data.url);
        auto datetime = QDateTime::fromSecsSinceEpoch(data.lastVisitedTime.toLongLong());
        if(datetime.date() == today){
            todayItem->appendRow(item);
        }else if(datetime.date() == yestoday){
            yestodayItem->appendRow(item);
        }else{
            weekItem->appendRow(item);
        }
    }
    qInfo()<<__FUNCTION__<<all_model_->columnCount();
}

void HistoryWidget::loadRecentlyHistories()
{
    recently_model_->clear();

    for (auto data : MainWindow::RecentlyHistory){
        QIcon icon(FaviconMgr::Instance().iconFilePath(data.url));
        if(icon.isNull()){
            icon = style()->standardIcon(QStyle::SP_MessageBoxInformation);
        }
        QStandardItem *item = new QStandardItem(icon, data.title);
        item->setData(true, Qt::UserRole + 1);
        item->setData(QVariant::fromValue(data), Qt::UserRole + 2);
        item->setToolTip(data.title + "\n" + data.url);
        recently_model_->appendRow(item);
    }
}

void HistoryWidget::onTreeAllHisItemClicked(const QModelIndex &index)
{
    auto item = all_model_->itemFromIndex(index);
    if(!item){
        return;
    }
    auto isData = item->data(Qt::UserRole + 1).value<bool>();
    if(isData){
        auto data = item->data(Qt::UserRole + 2).value<History>();
        emit menuCmd(HistoryCmd::Open, data.url);
    }
}

void HistoryWidget::onTreeAllHisContextMenu(const QPoint &)
{
    // parameter pos is wrong.
#if 0
    auto gPos = QCursor::pos();
    auto viewportPos = ui->treeViewAll->viewport()->mapFromGlobal(gPos);
    qInfo()<<"slot pos:"<<pos
            <<"viewportPos:"<<viewportPos;
    return;
#endif
    auto index = ui->treeViewAll->indexAt(ui->treeViewAll->viewport()->mapFromGlobal(QCursor::pos()));
    auto item = all_model_->itemFromIndex(index);
    if(!item){
        return;
    }
    auto isData = item->data(Qt::UserRole + 1).value<bool>();
    if(isData){
        menu_trigger_data_ = item->data(Qt::UserRole + 2).value<History>();

        menu_in_all_->clear();
        menu_in_all_->addAction(ac_open_);
        menu_in_all_->addAction(ac_open_in_new_tab_);
        menu_in_all_->addAction(ac_open_in_new_window_);
        menu_in_all_->addAction(ac_open_in_inpriavte_);
        menu_in_all_->addSeparator();
        menu_in_all_->addAction(ac_delete_);
        menu_in_all_->addAction(ac_copy_link_);
        menu_in_all_->addSeparator();
        menu_in_all_->addAction(ac_add2favorite_);
        menu_in_all_->exec(QCursor::pos());
    }
}
