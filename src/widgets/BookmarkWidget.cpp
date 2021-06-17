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

#include "popups/StyledMenu.h"
#include "managers/AppCfgManager.h"
#include "managers/FaviconManager.h"
#include "managers/BookmarkManager.h"
#include "managers/BookmarkManager.h"

BookmarkWidget::BookmarkWidget(QWidget *parent) :
    QWidget(parent)
  , ui(new Ui::BookmarkWidget)
  , menu_more_(new StyledMenu(this))
  , menu_tree_bkmk_(new StyledMenu(this))
  , menu_tree_bkmk_dir_(new StyledMenu(this))
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

void BookmarkWidget::onBkmkBarVisibleChanged()
{
    action_show_bookmark_bar_->setChecked(AppCfgMgr::bookmarkBarVisible());
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
    // 更多按钮菜单
    action_manage_bookmarks_  = new QAction(QIcon(), tr("bookmark manage"), this);
    action_add_current_       = new QAction(QIcon(), tr("add current to favorites"), this);
    action_add_all_           = new QAction(QIcon(), tr("add all to favorites"), this);
    action_import_bookmarks_  = new QAction(QIcon(), tr("import favorites"), this);
    action_export_bookmarks_  = new QAction(QIcon(), tr("export favorites"), this);
    action_delete_duplicate_  = new QAction(QIcon(), tr("delete duplicate favorites"), this);
    action_show_bookmark_bar_ = new QAction(QIcon(), tr("show bookmark bar"), this);
    action_show_bookmark_bar_->setCheckable(true);
    action_show_bookmark_bar_->setChecked(AppCfgMgr::bookmarkBarVisible());
    action_show_bookmark_btn_ = new QAction(QIcon(), tr("show bookmark button in toolbar"), this);

    menu_more_->installEventFilter(this);
    menu_more_->addAction(action_manage_bookmarks_);
    menu_more_->addSeparator();
    menu_more_->addAction(action_add_current_);
    menu_more_->addAction(action_add_all_);
    menu_more_->addSeparator();
    menu_more_->addAction(action_import_bookmarks_);
    menu_more_->addAction(action_export_bookmarks_);
    menu_more_->addAction(action_delete_duplicate_);
    menu_more_->addSeparator();
    menu_more_->addAction(action_show_bookmark_bar_);
    menu_more_->addAction(action_show_bookmark_btn_);

    ui->buttonMore->setPopupMode(QToolButton::InstantPopup);
    ui->buttonMore->setMenu(menu_more_);

    ui->treeView->setContextMenuPolicy(Qt::CustomContextMenu);
    ac_details_ = new QAction(tr("show details"), this);

    ac_open_bkmk_in_new_page_    = new QAction(QIcon(), tr("open in new page"), this);
    ac_open_bkmk_in_new_window_  = new QAction(QIcon(), tr("open in new window"), this);
    ac_open_in_inprivate_ = new QAction(QIcon(), tr("open in inprivate"), this);

    ac_rename_bkmk_       = new QAction(QIcon(), tr("rename"), this);
    ac_copy_link_         = new QAction(QIcon(), tr("copy link"), this);
    ac_modify_bkmk_       = new QAction(QIcon(), tr("modify"), this);
    ac_delte_bkmk_        = new QAction(QIcon(":/Resources/imgs/others/normal_remove.png"), tr("delete"), this);

    menu_tree_bkmk_->addAction(ac_details_);
    menu_tree_bkmk_->addSeparator();
    menu_tree_bkmk_->addAction(ac_open_bkmk_in_new_page_);
    menu_tree_bkmk_->addAction(ac_open_bkmk_in_new_window_);
    menu_tree_bkmk_->addAction(ac_open_in_inprivate_);
    menu_tree_bkmk_->addSeparator();
    menu_tree_bkmk_->addAction(ac_rename_bkmk_);
    menu_tree_bkmk_->addAction(ac_copy_link_);
    menu_tree_bkmk_->addAction(ac_modify_bkmk_);
    menu_tree_bkmk_->addAction(ac_delte_bkmk_);


    ac_open_all_bkmk_page_    = new QAction(QIcon(), tr("open all"), this);
    ac_open_all_bkmk_window_  = new QAction(QIcon(), tr("open all in new window"), this);
    ac_open_all_in_inprivate_ = new QAction(QIcon(), tr("open all in inprivate"), this);

    ac_sort_as_name_          = new QAction(QIcon(), tr("sort as name"), this);
    ac_rename_folder_         = new QAction(QIcon(), tr("rename"), this);
    ac_delte_folder_          = new QAction(QIcon(), tr("delete"), this);

    ac_add_cur_to_folder_     = new QAction(QIcon(), tr("add current to this folder"), this);
    ac_add_all_to_folder_     = new QAction(QIcon(), tr("add all to this folder"), this);

    menu_tree_bkmk_dir_->addAction(ac_details_);
    menu_tree_bkmk_dir_->addSeparator();
    menu_tree_bkmk_dir_->addAction(ac_open_all_bkmk_page_);
    menu_tree_bkmk_dir_->addAction(ac_open_all_bkmk_window_);
    menu_tree_bkmk_dir_->addAction(ac_open_all_in_inprivate_);
    menu_tree_bkmk_dir_->addSeparator();
    menu_tree_bkmk_dir_->addAction(ac_sort_as_name_);
    menu_tree_bkmk_dir_->addAction(ac_rename_folder_);
    menu_tree_bkmk_dir_->addAction(ac_delte_folder_);
    menu_tree_bkmk_dir_->addSeparator();
    menu_tree_bkmk_dir_->addAction(ac_add_cur_to_folder_);
    menu_tree_bkmk_dir_->addAction(ac_add_all_to_folder_);



    /* *****************Search Page*************************/
    QAction *ac_serch_left_icon = new QAction(QIcon(":/Resources/imgs/others/normal_search.png"),
                                              tr(""),
                                              ui->lineEditSearch);
    ui->lineEditSearch->addAction(ac_serch_left_icon, QLineEdit::LeadingPosition);
}

void BookmarkWidget::initSignalSlots()
{
    connect(BookmarkMgr::Instance(), &BookmarkMgr::bookmarksChanged,
            this, &BookmarkWidget::onBookmarksChanged);

    connect(action_manage_bookmarks_, &QAction::triggered, this, [this]()
    {
        qInfo()<<"TODO:";
    });
    connect(action_add_current_, &QAction::triggered, this, [this]()
    {

    });
    connect(action_add_all_, &QAction::triggered, this, [this]()
    {

    });
    connect(action_import_bookmarks_, &QAction::triggered, this, [this]()
    {

    });
    connect(action_export_bookmarks_, &QAction::triggered, this, [this]()
    {
    });
    connect(action_delete_duplicate_, &QAction::triggered, this, [this]()
    {

    });
    connect(action_show_bookmark_bar_, &QAction::triggered, this, [this]()
    {
//         emit cmdTriggered(static_cast<int>(BkmkWidget_ShowBkmkBar), QVariant());
    });
    connect(action_show_bookmark_btn_, &QAction::triggered, this, [this]()
    {

    });

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
    connect(ac_details_, &QAction::triggered, this, [this](){
        auto name = menu_trigger_item_->data(BookmarkMgr::Name).toString();
        auto type = menu_trigger_item_->data(BookmarkMgr::Type).toString();
        QString hint = QString("type:%1\nname:%2\nid:%3\nGUID:%4\ndateAdded:%5\n")
                .arg(menu_trigger_item_->data(BookmarkMgr::Type).toString())
                .arg(menu_trigger_item_->data(BookmarkMgr::Name).toString())
                .arg(menu_trigger_item_->data(BookmarkMgr::Id).toString())
                .arg(menu_trigger_item_->data(BookmarkMgr::Guid).toString())
                .arg(menu_trigger_item_->data(BookmarkMgr::DateAdded).toString());
        if(type == "folder"){
            hint.append(QString("children:%1\n").arg(menu_trigger_item_->rowCount()));
        }else if(type == "url"){
            hint.append(QString("url:%1\n").arg(menu_trigger_item_->data(BookmarkMgr::Url).toString()));
        }
        QMessageBox::information(this, name, hint);
    });
    connect(ac_open_bkmk_in_new_page_, &QAction::triggered, [this]()
    {
        auto data = menu_trigger_item_->data(BookmarkMgr::Url);
        emit menuCmd(BookmarkCmd::OpenInNewPage, data);
    });
    connect(ac_open_bkmk_in_new_window_, &QAction::triggered, [this]()
    {
        auto data = menu_trigger_item_->data(BookmarkMgr::Url);
        emit menuCmd(BookmarkCmd::OpenInNewWnd, data);
    });
    connect(ac_open_in_inprivate_, &QAction::triggered, [this]()
    {
        auto data = menu_trigger_item_->data(BookmarkMgr::Url);
        emit menuCmd(BookmarkCmd::OpenInInprivate, data);
    });
    connect(ac_rename_bkmk_, &QAction::triggered, [this]()
    {

    });
    connect(ac_copy_link_, &QAction::triggered, [this]()
    {

    });
    connect(ac_modify_bkmk_, &QAction::triggered, [this]()
    {

    });
    connect(ac_delte_bkmk_, &QAction::triggered, [this]()
    {

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
    auto index = ui->treeView->indexAt(ui->treeView->viewport()->mapFromGlobal(QCursor::pos()));
    if(!index.isValid()){
        return;
    }
    auto item = BookmarkMgr::gBookmarkModel->itemFromIndex(index);
    if(item == nullptr){
        return;
    }
    menu_trigger_item_ = item;
    auto type = item->data(BookmarkMgr::Type).toString();
    if(type == "folder"){
        menu_tree_bkmk_dir_->exec(QCursor::pos());
    }else if(type == "url"){
        menu_tree_bkmk_->exec(QCursor::pos());
    }
}
