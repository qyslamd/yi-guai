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

#include "managers/BookmarkManager.h"
#include "managers/FaviconManager.h"

BookmarkBar::BookmarkBar(QWidget *parent)
    : QFrame(parent)
{
    initUi();
    connect(BookmarkMgr::Instance(), &BookmarkMgr::bookmarksChanged,
            this, &BookmarkBar::onBookmarksChanged);
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
    static bool firstShown = true;
    if(firstShown){
        firstShown = false;
    }else{
        onBookmarksChanged();
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

    item_layout_ = new QHBoxLayout;
    item_layout_->setSpacing(2);
    item_layout_->setContentsMargins(1,1,1,1);

    btn_others_ = new QPushButton(FaviconMgr::systemDirIcon, QStringLiteral("其它书签"), this);
    QFrame* line = new QFrame(this);
    line->setStyleSheet("margin-top:4px;margin-bottom:4px;");
    line->setObjectName("line");
    line->setMaximumWidth(2);
    line->setFrameShape(QFrame::VLine);
    line->setFrameShadow(QFrame::Sunken);

    layout_->addWidget(btn_niubi_);
    layout_->addWidget(label_empty_);
    layout_->addLayout(item_layout_);
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

void BookmarkBar::onBookmarksChanged()
{
    if(loaded_){return;}

    QElapsedTimer timer;
    timer.start();

    auto model = BookmarkMgr::Instance()->gBookmarkModel;
    auto barItem = model->item(0);
    if(barItem){
        for (int i = 0; i< barItem->rowCount(); i++)
        {
            auto child = barItem->child(i);
            if(child){
                auto type = child->data(BookmarkMgr::Type).toString();
                auto name = child->data(BookmarkMgr::Name).toString();
                auto icon = type=="folder"?FaviconMgr::systemDirIcon:FaviconMgr::systemFileIcon;
                auto button = new BarItem(icon, name, this);
                item_layout_->addWidget(button);

                if(type == "folder"){
                    button->setMenu(makeMenu(child));
                }else if(type == "url"){
                    auto url = child->data(BookmarkMgr::Url).toString();
                    button->setToolTip(name + "\n" + url);
                    connect(button, &QPushButton::clicked, [=](){
                        emit cmdTriggered(BookmarkCmd::Open, url);
                    });
                }
                button->setData(QVariant::fromValue<void *>(child));
            }
        }
        label_empty_->setVisible(barItem->rowCount() == 0);
    }
    auto otherItem = model->item(1);
    if(otherItem){
        btn_others_->setMenu(makeMenu(otherItem));
    }

    loaded_ = true;
    qInfo()<<"\033[32m[Execute Time]"<<__FUNCTION__<<":" << timer.elapsed() << "ms"<<"\033[0m";
}


BarItem::BarItem(QWidget *parent)
    : QPushButton(parent)
{
    init();
}

void BarItem::setData(const QVariant &var)
{
    data_ = var;
}

QVariant BarItem::data() const
{
    return data_;
}

BarItem::BarItem(const QIcon &icon, const QString &title, QWidget *parent)
    :QPushButton(icon,title, parent)
{
    init();
}

BarItem::BarItem(const QString &title, QWidget *parent)
    :QPushButton(title, parent)
{
    init();
}

void BarItem::init()
{
    setMaximumWidth(150);
}


BookmarkMenu::BookmarkMenu(const QString &title, QWidget *parent)
    : QMenu(title, parent)
{
    initUi();
}

BookmarkMenu::BookmarkMenu(QWidget *parent)
    : QMenu(parent)
{
    initUi();
}

void BookmarkMenu::initUi()
{
    setWindowFlag(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setAutoFillBackground(true);
    setContextMenuPolicy(Qt::CustomContextMenu);
    setMaximumWidth(560);
    setToolTipsVisible(true);

    connect(this, &BookmarkMenu::customContextMenuRequested,
            this, &BookmarkMenu::onCustomContextMenuRequested);
}

void BookmarkMenu::onCustomContextMenuRequested(const QPoint &)
{
    static StyledMenu menu;
    menu.clear();
    auto action = activeAction();
    // without menu
    if(action){
        if(action->isSeparator()){
            return;
        }
    }else{
        action = menuAction();
    }
    if(auto dataItem = (QStandardItem *)action->data().value<void *>()){
        auto type = dataItem->data(BookmarkMgr::Type).toString();
        auto openAction = BookmarkMgr::Instance()->action_open_new_tab_;
        auto openWndAction = BookmarkMgr::Instance()->action_open_new_wnd_;
        auto openPrivateAction = BookmarkMgr::Instance()->action_open_in_private_;

        if(type == "folder"){
            openAction->setText(tr("open all") + QString("(%1)").arg(dataItem->rowCount()));
            openWndAction->setText(tr("open all in new window") + QString("(%1)").arg(dataItem->rowCount()));
            openPrivateAction->setText(tr("open all in private window") + QString("(%1)").arg(dataItem->rowCount()));
            menu.addAction(openAction);
            menu.addAction(openWndAction);
            menu.addAction(openPrivateAction);
            menu.addSeparator();
            menu.addAction(BookmarkMgr::Instance()->action_rename_);

        }else if(type == "url"){
            openAction->setText(tr("open") );
            openWndAction->setText(tr("open in new window"));
            openPrivateAction->setText(tr("open in private window"));
            menu.addAction(openAction);
            menu.addAction(openWndAction);
            menu.addAction(openPrivateAction);
            menu.addSeparator();
            menu.addAction(BookmarkMgr::Instance()->action_modify_);
        }
    }
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
