#include "BookmarkWidget.h"
#include "ui_BookmarkWidget.h"

#include <QTreeWidget>
#include <QtDebug>
#include <QShowEvent>
#include <QStandardItemModel>
#include <QFileInfo>
#include <QClipboard>
#include <QTimer>
#include <QStyle>
#include <QMessageBox>
#include <QUrl>
#include <QLabel>

#include "popups/StyledMenu.h"
#include "managers/AppCfgManager.h"
#include "managers/FaviconManager.h"
#include "managers/BookmarkManager.h"
#include "managers/BookmarkManager.h"

BookmarkWidget::BookmarkWidget(QWidget *parent) :
    QWidget(parent)
  , ui(new Ui::BookmarkWidget)
  , menu_more_(new StyledMenu(this))
{
    ui->setupUi(this);
    initUi();
    initSignalSlots();
}

BookmarkWidget::~BookmarkWidget()
{
    delete ui;
}

void BookmarkWidget::onShowModeChanged(ToolWndShowMode mode)
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

bool BookmarkWidget::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == menu_more_){
        if(event->type() == QEvent::Show){
            QTimer::singleShot(5, this, [this](){
                auto pos = mapToGlobal(ui->buttonMore->geometry().bottomRight());
                pos.setX(pos.x() - menu_more_->width());
                menu_more_->move(pos);
            });

        }
    }
    return QWidget::eventFilter(watched, event);
}

void BookmarkWidget::initUi()
{
    ui->treeView->setModel(BookmarkMgr::gBookmarkModel);
    /* *****************General Page*************************/
    menu_more_->installEventFilter(this);
    menu_more_->addAction(BookmarkMgr::Instance()->action_manage_bookmarks_);
    menu_more_->addSeparator();
    menu_more_->addAction(BookmarkMgr::Instance()->action_add_current_);
    menu_more_->addAction(BookmarkMgr::Instance()->action_add_all_);
    menu_more_->addSeparator();
    menu_more_->addAction(BookmarkMgr::Instance()->action_import_bookmarks_);
    menu_more_->addAction(BookmarkMgr::Instance()->action_export_bookmarks_);
    menu_more_->addAction(BookmarkMgr::Instance()->action_delete_duplicate_);
    menu_more_->addSeparator();
    menu_more_->addAction(BookmarkMgr::Instance()->action_show_bookmark_bar_);
    menu_more_->addAction(BookmarkMgr::Instance()->action_show_bookmark_btn_);

    ui->buttonMore->setPopupMode(QToolButton::InstantPopup);
    ui->buttonMore->setMenu(menu_more_);

    ui->treeView->setContextMenuPolicy(Qt::CustomContextMenu);

    /* *****************Search Page*************************/
    QAction *ac_serch_left_icon = new QAction(QIcon(":/Resources/imgs/others/normal_search.png"),
                                              tr(""),
                                              ui->lineEditSearch);
    ui->lineEditSearch->addAction(ac_serch_left_icon, QLineEdit::LeadingPosition);
}

void BookmarkWidget::initSignalSlots()
{
    connect(BookmarkMgr::Instance(), &BookmarkMgr::loadFinished,
            this, &BookmarkWidget::onBookmarksChanged);

    // 搜索按钮点击，跳转到搜索专属界面
    connect(ui->buttonSearch, &QToolButton::clicked, [this](){
        ui->stackedWidget->setCurrentWidget(ui->pageSearch);
    });

    // 搜索返回按钮点击,返回到 通用页面
    connect(ui->buttonSearhBack, &QToolButton::clicked, [this](){
        ui->stackedWidget->setCurrentWidget(ui->pageGeneral);
    });

    // pin or close clicked
    connect(ui->buttonPinOrClose, &QToolButton::clicked, [this]()
    {
        emit pinOrCloseClicked();
    });
    // QTreeWidget 右键菜单
    connect(ui->treeView, &QTreeWidget::customContextMenuRequested,
            this, &BookmarkWidget::onAllBkmkTreeCustomContextMenu);
}

void BookmarkWidget::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
}

void BookmarkWidget::onBookmarksChanged()
{
    auto item = BookmarkMgr::gBookmarkModel->item(0);
    ui->treeView->expand(BookmarkMgr::gBookmarkModel->indexFromItem(item));
}

void BookmarkWidget::onAllBkmkTreeCustomContextMenu(const QPoint &)
{
    static StyledMenu menu;
    menu.clear();
    auto index = ui->treeView->indexAt(ui->treeView->viewport()->mapFromGlobal(QCursor::pos()));
    if(!index.isValid()){
        return;
    }
    auto item = BookmarkMgr::gBookmarkModel->itemFromIndex(index);
    if(item == nullptr){
        return;
    }
    if(item){
        BookmarkMgr::Instance()->setMenuTriggerItem(item);
        auto type = item->data(BookmarkMgr::Type).toString();
        auto openAction = BookmarkMgr::Instance()->action_open_new_tab_;
        auto openWndAction = BookmarkMgr::Instance()->action_open_new_wnd_;
        auto openPrivateAction = BookmarkMgr::Instance()->action_open_in_private_;

        if(type == "folder"){
            openAction->setText(tr("open all in new tab") + QString("(%1)").arg(item->rowCount()));
            openWndAction->setText(tr("open all in new window") + QString("(%1)").arg(item->rowCount()));
            openPrivateAction->setText(tr("open all in private window") + QString("(%1)").arg(item->rowCount()));
            menu.addAction(openAction);
            menu.addAction(openWndAction);
            menu.addAction(openPrivateAction);
            menu.addSeparator();
            menu.addAction(BookmarkMgr::Instance()->action_rename_);

        }else if(type == "url"){
            openAction->setText(tr("open in new tab"));
            openWndAction->setText(tr("open in new window"));
            openPrivateAction->setText(tr("open in private window"));
            menu.addAction(openAction);
            menu.addAction(openWndAction);
            menu.addAction(openPrivateAction);
            menu.addSeparator();
            menu.addAction(BookmarkMgr::Instance()->action_modify_);
        }
        menu.addSeparator();
        menu.addAction(BookmarkMgr::Instance()->action_cut_);
        menu.addAction(BookmarkMgr::Instance()->action_copy_);
        menu.addAction(BookmarkMgr::Instance()->action_paste_);
        menu.addSeparator();
        menu.addAction(BookmarkMgr::Instance()->action_delete_);
        menu.addSeparator();
    }
    menu.addAction(BookmarkMgr::Instance()->action_add_current_);
    menu.addAction(BookmarkMgr::Instance()->action_add_folder_);
    menu.addSeparator();
    menu.addAction(BookmarkMgr::Instance()->action_show_bookmark_bar_);
    menu.addAction(BookmarkMgr::Instance()->action_show_bookmark_btn_);
    menu.addAction(BookmarkMgr::Instance()->action_manage_bookmarks_);
    menu.exec(QCursor::pos());
}
