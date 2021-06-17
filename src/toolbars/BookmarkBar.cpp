#include "BookmarkBar.h"

#include <QPaintEvent>
#include <QPainter>
#include <QMenu>
#include <QAction>
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
    setStyleSheet("QPushButton:menu-indicator{"
"image:none;"
"}"
"QPushButton{"
"text-align: left;"
"border:none;"
"color:#666666;"
"padding:4px;"
"}"
"QPushButton:hover{"
"background: #DCDCDC;"
"border:none;"
"border-radius:4px;"
"}"
"QPushButton:pressed{"
"background: #e8ebec;"
"}"
"QPushButton:open{"
"background: #DCDCDC;"
"border:none;"
"border-radius:4px;"
"}");

    layout_ = new QHBoxLayout(this);
    layout_->setSpacing(4);
    layout_->setContentsMargins(4,4,4,4);

    item_layout_ = new QHBoxLayout;
    item_layout_->setSpacing(2);
    item_layout_->setContentsMargins(1,1,1,1);

    btn_others_ = new QPushButton(FaviconMgr::systemDirIcon, QStringLiteral("其它书签"), this);
    QFrame* line = new QFrame(this);
    line->setStyleSheet("margin-top:4px;margin-bottom:4px;");
    line->setObjectName(QString::fromUtf8("line"));
    line->setGeometry(QRect(160, 150, 3, 3));
    line->setFrameShape(QFrame::VLine);
    line->setFrameShadow(QFrame::Sunken);

    layout_->addLayout(item_layout_);
    layout_->addStretch();
    layout_->addWidget(line);
    layout_->addWidget(btn_others_);
}

BookmarkMenu *BookmarkBar::makeMenu(const QStandardItem *item)
{
    auto name = item->data(BookmarkMgr::Name).toString();
    BookmarkMenu *menu = new BookmarkMenu(name, this);
    for (int i = 0; i< item->rowCount(); i++){
        auto child = item->child(i);
        if(child){
            auto type = child->data(BookmarkMgr::Type).toString();
            auto icon = type=="folder"?FaviconMgr::systemDirIcon:FaviconMgr::systemFileIcon;
            auto action = new QAction(icon, child->data(BookmarkMgr::Name).toString(), this);
            menu->addAction(action);
            if(type == "folder"){
                action->setMenu(makeMenu(child));
            }
        }
    }
    return menu;
}

void BookmarkBar::onCustomContextMenuRequested(const QPoint &)
{
    static BookmarkMenu menu;
    menu.clear();
    menu.addAction("Hello");
    menu.addAction("world");
    menu.addAction("printf");
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
        for (int i = 0; i< barItem->rowCount(); i++){
            auto child = barItem->child(i);
            if(child){
                auto type = child->data(BookmarkMgr::Type).toString();
                auto icon = type=="folder"?FaviconMgr::systemDirIcon:FaviconMgr::systemFileIcon;
                auto button = new BarItem(icon, child->data(BookmarkMgr::Name).toString(), this);
                item_layout_->addWidget(button);

    //            if(type == "folder"){
    //                button->setMenu(makeMenu(child));
    //            }
            }
        }
    }
    auto otherItem = model->item(1);
    if(otherItem){
        btn_others_->setMenu(makeMenu(otherItem));
    }

    loaded_ = true;
    qInfo()<<"\033[32m[Time:]"<<__FUNCTION__<<":" << timer.elapsed() << "ms"<<"\033[0m";
}


BarItem::BarItem(QWidget *parent)
    : QPushButton(parent)
{
    init();
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
}
