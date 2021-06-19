#include "BookmarkBar.h"

#include <QPaintEvent>
#include <QPainter>
#include <QMenu>
#include <QAction>
#include <QLabel>
#include <QElapsedTimer>
#include <QStandardItem>
#include <QHBoxLayout>
#include <QToolButton>
#include <QPushButton>
#include <QStyle>
#include <QFontMetrics>
#include <QToolBar>
#include <QChildEvent>

#include "managers/BookmarkManager.h"
#include "managers/FaviconManager.h"

BookmarkBar::BookmarkBar(QWidget *parent)
    : QFrame(parent)
{
    initUi();
    connect(BookmarkMgr::gToolbarProvider, &ToolBarProviderWnd::loadToUiFinished,
            this, &BookmarkBar::loadBookmarks);
}

void BookmarkBar::paintEvent(QPaintEvent *event)
{
    QFrame::paintEvent(event);

    QPainter p(this);
    p.save();
    qreal penWidth = 0.5f;
    QColor color((QLatin1String("#D2D2D2")));
    p.setPen(QPen(QBrush(color), 0.5f));
    p.drawLine(0.0, height() - penWidth, width(), height() - penWidth);
    p.restore();

    p.save();
    if(!loaded_){
        QString hint = QStringLiteral("正在加载书签");
        QFontMetrics fm(this->font());
        fm.horizontalAdvance(hint);
        auto pixelsHigh = fm.height();

        auto pos = this->rect().center();
        pos.ry() += pixelsHigh / 2;
        p.drawText(pos, hint);
    }
    p.restore();
}

void BookmarkBar::showEvent(QShowEvent *event)
{
    /*本类的首个对象标志*/
    static bool first_wnd = true;
    if(first_wnd){
        first_wnd = false;
        return QFrame::showEvent(event);
    }
    /*
     * 每个对象显示的时候，均判断是否是首次显示
     * 如果是首次显示，加载书签
     */
    if(first_shown_){
        loadBookmarks();
        first_shown_ = false;
    }
    QFrame::showEvent(event);
}

void BookmarkBar::initUi()
{
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &QFrame::customContextMenuRequested, this, &BookmarkBar::onCustomContextMenuRequested);

    layout_ = new QHBoxLayout(this);
    layout_->setSpacing(4);
    layout_->setContentsMargins(4,4,4,4);

    btn_niubi_ = new QToolButton(this);
    btn_niubi_->setToolTip(tr("open applications"));
    btn_niubi_->setIcon(QIcon(":/icons/resources/imgs/gray/squared_menu_96px.png"));
    btn_niubi_->setIconSize(QSize(26,26));
    label_empty_ = new QLabel(QStringLiteral("你还没有书签，赶紧添加一个吧"), this);

    toolbar_ = new BookmarkToolBar(this);

    btn_others_ = new QPushButton(FaviconMgr::systemDirIcon, QStringLiteral("其它书签"), this);
    btn_others_->setObjectName("BookmarkBarOhtersButton");
    QFrame* line = new QFrame(this);
    line->setStyleSheet("margin-top:4px;margin-bottom:4px;");
    line->setObjectName("line");
    line->setMaximumWidth(2);
    line->setFrameShape(QFrame::VLine);
    line->setFrameShadow(QFrame::Sunken);

    layout_->addWidget(btn_niubi_);
    layout_->addWidget(label_empty_);
    layout_->addWidget(toolbar_);
    layout_->addStretch();
    layout_->addWidget(line);
    layout_->addWidget(btn_others_);

    label_empty_->hide();
}

BookmarkMenu *BookmarkBar::makeMenu(const QStandardItem *item)
{
    auto name = item->data(BookmarkMgr::Name).toString();
    BookmarkMenu *menu = new BookmarkMenu(name, this);
    for (int i = 0; i< item->rowCount(); i++){
        auto child = item->child(i);
        if(child){
            auto type = child->data(BookmarkMgr::Type).toString();
            auto name = child->data(BookmarkMgr::Name).toString();
            auto icon = type=="folder"?FaviconMgr::systemDirIcon:FaviconMgr::systemFileIcon;
            auto action = new QAction(icon, name, this);
            menu->addAction(action);
            if(type == "folder"){
                action->setMenu(makeMenu(child));
            }else if(type == "url"){
                connect(action, &QAction::triggered,[=](){
                    if(auto dataItem = (QStandardItem *)action->data().value<void *>()){
                        emit cmdTriggered(BookmarkCmd::Open, dataItem->data(BookmarkMgr::Url));
                    }
                });
                auto url = child->data(BookmarkMgr::Url).toString();
                action->setToolTip(name + "\n" + url);
            }
            action->setData(QVariant::fromValue<void *>(child));
        }
    }
    return menu;
}

void BookmarkBar::onCustomContextMenuRequested(const QPoint &pos)
{
    static StyledMenu menu;
    auto child = childAt(pos);
    qInfo()<<__FUNCTION__<<child;

    menu.clear();
    menu.addAction(BookmarkMgr::Instance()->action_open_new_tab_);
    menu.addAction(BookmarkMgr::Instance()->action_open_new_wnd_);
    menu.addAction(BookmarkMgr::Instance()->action_open_in_private_);
    menu.addSeparator();
    menu.addAction(BookmarkMgr::Instance()->action_modify_);
    menu.addAction(BookmarkMgr::Instance()->action_rename_);
    menu.addSeparator();
    menu.addAction(BookmarkMgr::Instance()->action_cut_);
    menu.addAction(BookmarkMgr::Instance()->action_copy_);
    menu.addAction(BookmarkMgr::Instance()->action_paste_);
    menu.addSeparator();
    menu.addAction(BookmarkMgr::Instance()->action_delete_);
    menu.addSeparator();
    menu.addAction(BookmarkMgr::Instance()->action_add_current_);
    menu.addAction(BookmarkMgr::Instance()->action_add_folder_);
    menu.addSeparator();
    menu.addAction(BookmarkMgr::Instance()->action_show_bookmark_bar_);
    menu.addAction(BookmarkMgr::Instance()->action_show_bookmakr_btn_);
    menu.addAction(BookmarkMgr::Instance()->action_manage_bookmarks_);
    menu.exec(QCursor::pos());
}

void BookmarkBar::loadBookmarks()
{
    loaded_ = false;

    auto providerWnd = BookmarkMgr::gToolbarProvider;

    if(!providerWnd){
        return;
    }
    for (auto item : providerWnd->buttons){
        toolbar_->addAction(item);
    }

    for (auto item : toolbar_->children()){
        if(!item->isWidgetType()){
            continue;
        }
        if(item->inherits("QToolButton") || QString(item->metaObject()->className()).compare("QToolButton") == 0)
        {
            auto button = qobject_cast<QToolButton*>(item);
            button->setPopupMode(QToolButton::InstantPopup);
        }
    }

    if(providerWnd->others_menu_){
        btn_others_->setMenu(providerWnd->others_menu_);
    }
    loaded_ = true;

    label_empty_->setVisible(providerWnd->buttons.isEmpty());
}

BookmarkToolBar::BookmarkToolBar(QWidget *parent)
    : QToolBar(parent)
{
    initUi();
}

BookmarkToolBar::BookmarkToolBar(const QString &title, QWidget *parent)
    : QToolBar(title, parent)
{
    initUi();
}

bool BookmarkToolBar::event(QEvent *ev)
{
    return QToolBar::event(ev);
}

void BookmarkToolBar::initUi()
{
    setFloatable(false);
    setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
}
