#include "BookmarkWidget.h"
#include "ui_BookmarkWidget.h"

#include <QTreeWidget>
#include <QMenu>
#include <QtDebug>
#include <QShowEvent>
#include <QTreeWidgetItem>
#include <QFileInfo>
#include <QClipboard>
#include <QTimer>

#include "managers/AppCfgManager.h"

BookmarkWidget::BookmarkWidget(QWidget *parent) :
    QWidget(parent)
  , ui(new Ui::BookmarkWidget)
  , menu_more_(new QMenu(this))
  , menu_tree_bkmk_(new QMenu(this))
  , menu_tree_bkmk_dir_(new QMenu(this))
{
    ui->setupUi(this);
    initUi();
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

void BookmarkWidget::onBookmarksChanged()
{

}

bool BookmarkWidget::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == menu_more_){
        if(event->type() == QEvent::Show){
            QTimer::singleShot(5, [this](){
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

    menu_more_->setObjectName("BkmkWidgetMoreOptionMenu");
    menu_more_->setWindowFlags(menu_more_->windowFlags() | Qt::FramelessWindowHint);
    menu_more_->setAttribute(Qt::WA_TranslucentBackground);
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

    ui->treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    ac_open_bkmk_in_new_page_    = new QAction(QIcon(), tr("open in new page"), this);
    ac_open_bkmk_in_new_window_  = new QAction(QIcon(), tr("open in new window"), this);
    ac_open_in_inprivate_ = new QAction(QIcon(), tr("open in inprivate"), this);

    ac_rename_bkmk_       = new QAction(QIcon(), tr("rename"), this);
    ac_copy_link_         = new QAction(QIcon(), tr("copy link"), this);
    ac_modify_bkmk_       = new QAction(QIcon(), tr("modify"), this);
    ac_delte_bkmk_        = new QAction(QIcon(":/Resources/imgs/others/normal_remove.png"), tr("delete"), this);

    menu_tree_bkmk_->addAction(ac_open_bkmk_in_new_page_);
    menu_tree_bkmk_->addAction(ac_open_bkmk_in_new_window_);
    menu_tree_bkmk_->addAction(ac_open_in_inprivate_);
    menu_tree_bkmk_->addSeparator();
    menu_tree_bkmk_->addAction(ac_rename_bkmk_);
    menu_tree_bkmk_->addAction(ac_copy_link_);
    menu_tree_bkmk_->addAction(ac_modify_bkmk_);
    menu_tree_bkmk_->addAction(ac_delte_bkmk_);

    connect(ac_open_bkmk_in_new_page_, &QAction::triggered, [this]()
    {

    });
    connect(ac_open_bkmk_in_new_window_, &QAction::triggered, [this]()
    {

    });
    connect(ac_open_in_inprivate_, &QAction::triggered, [this]()
    {

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


    ac_open_all_bkmk_page_    = new QAction(QIcon(), tr("open all"), this);
    ac_open_all_bkmk_window_  = new QAction(QIcon(), tr("open all in new window"), this);
    ac_open_all_in_inprivate_ = new QAction(QIcon(), tr("open all in inprivate"), this);

    ac_sort_as_name_          = new QAction(QIcon(), tr("sort as name"), this);
    ac_rename_folder_         = new QAction(QIcon(), tr("rename"), this);
    ac_delte_folder_          = new QAction(QIcon(), tr("delete"), this);

    ac_add_cur_to_folder_     = new QAction(QIcon(), tr("add current to this folder"), this);
    ac_add_all_to_folder_     = new QAction(QIcon(), tr("add all to this folder"), this);

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

    // QTreeWidget 右键菜单
    connect(ui->treeWidget, &QTreeWidget::customContextMenuRequested,
            this, &BookmarkWidget::onTreeWidgetCustomContextMenu);
    connect(ui->treeWidget, &QTreeWidget::itemClicked,
            this, &BookmarkWidget::onTreeWidgetItemClicked);


    /* *****************Search Page*************************/
    QAction *ac_serch_left_icon = new QAction(QIcon(":/Resources/imgs/others/normal_search.png"),
                                              tr(""),
                                              ui->lineEditSearch);
    ui->lineEditSearch->addAction(ac_serch_left_icon, QLineEdit::LeadingPosition);
}

void BookmarkWidget::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    // 默认显示顶部通用页面
    ui->stackedWidget->setCurrentWidget(ui->pageGeneral);

    // 加载书签
    ui->treeWidget->clear();

}

void BookmarkWidget::onTreeWidgetCustomContextMenu(const QPoint &)
{
//    auto pos = ui->treeWidget->viewport()->mapFromGlobal(QCursor::pos());
//    auto item = ui->treeWidget->itemAt(pos);
//    if(item){
//        bkmk_ac_trigger_data_ = item->data(0, Qt::UserRole + 1)
//                .value<bookmark::BookmarkData>();
//        // on bookmark
//        if(bkmk_ac_trigger_data_.type.compare("url") == 0)
//        {
//            menu_tree_bkmk_->exec(QCursor::pos());
//        }else // on bookmark dir
//        {
//            menu_tree_bkmk_dir_->exec(QCursor::pos());
//        }
//    }
}

void BookmarkWidget::onTreeWidgetItemClicked(QTreeWidgetItem *item, int column)
{
    if(!item) return;

//    bookmark::BookmarkData data = item->data(column, Qt::UserRole + 1).value<bookmark::BookmarkData>();
//    if(data.type.compare("url") == 0){
//        cmdTriggered(static_cast<int>(BkmkWidget_OpenInCurPage),
//                     data.url);
//    }
}
