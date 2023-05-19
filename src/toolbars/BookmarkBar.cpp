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
#include <QPainterPath>
#include <QTimer>

#include "managers/BookmarkManager.h"
#include "managers/FaviconManager.h"
#include "utils/util_qt.h"

BookmarkBar::BookmarkBar(QWidget *parent)
    : QFrame(parent)
{
    initUi();
    QTimer::singleShot(0, this, &BookmarkBar::loadBookmarks);
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

    if(!loaded_){
        p.save();
        QString hint = QString("正在加载书签");
        QFontMetrics fm(this->font());
#if (QT_VERSION == QT_VERSION_CHECK(5, 11, 1))
        auto width = fm.horizontalAdvance(hint);
#else
        auto width = fm.width(hint);
#endif

        auto pos = this->rect().center();
        pos.ry() += fm.xHeight();
        p.drawText(pos, hint);
        p.restore();

        p.save();
        p.setRenderHint(QPainter::Antialiasing);
        QPainterPath path;
        auto ww = 6;
        auto y = rect().center().y() - ww;
        auto rect1 = QRect(pos.x() + width + ww, y, 2 * ww, 2 * ww);
        auto rect2 = QRect(rect1.topRight().x() + ww, y, 2 * ww, 2 * ww);
        auto rect3 = QRect(rect2.topRight().x() + ww, y, 2 * ww, 2 * ww);
        auto rect4 = QRect(rect3.topRight().x() + ww, y, 2 * ww, 2 * ww);
        if(test_int_ == 1){
            path.addEllipse(rect1);
        }else if(test_int_ == 2){
            path.addEllipse(rect2);
        }else if(test_int_ == 3){
            path.addEllipse(rect3);
        }else{
            path.addEllipse(rect4);
        }
        p.fillPath(path, test_color_);
        p.restore();
    }
}

void BookmarkBar::showEvent(QShowEvent *event)
{
    QFrame::showEvent(event);
}

void BookmarkBar::initUi()
{
    connect(BookmarkMgr::gToolbarProvider, &ToolBarProviderWnd::loadToUiFinished,
            this, &BookmarkBar::loadBookmarks);
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &QFrame::customContextMenuRequested, this, &BookmarkBar::onCustomContextMenuRequested);

    layout_ = new QHBoxLayout(this);
    layout_->setSpacing(4);
    layout_->setContentsMargins(4,2,4,2);

    btn_application_ = new QPushButton(FaviconMgr::systemDirIcon, tr("application"));
    btn_application_->setToolTip(tr("open applications"));
    btn_application_->setIcon(QIcon(":/icons/resources/imgs/gray/squared_menu_96px.png"));
    connect(btn_application_, &QPushButton::clicked, this, [this]()
    {
        emit appBtnClicked(mapToGlobal(btn_application_->geometry().bottomRight()));
    });
    label_empty_ = new QLabel(QString("你还没有书签，赶紧添加一个吧"), this);

    toolbar_ = new BookmarkToolBar(this);
    toolbar_->setIconSize(QSize(16,16));

    btn_others_ = new QPushButton(FaviconMgr::systemDirIcon, QString("其它书签"), this);
    QFrame* line = new QFrame(this);
    line->setStyleSheet("margin-top:4px;margin-bottom:4px;");
    line->setObjectName("line");
    line->setMaximumWidth(2);
    line->setFrameShape(QFrame::VLine);
    line->setFrameShadow(QFrame::Sunken);

    layout_->addWidget(btn_application_);
    layout_->addWidget(label_empty_);
    layout_->addWidget(toolbar_);
    layout_->addStretch();
    layout_->addWidget(line);
    layout_->addWidget(btn_others_);

    label_empty_->hide();
    auto timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [this]()
    {
        test_color_ = QColor(UtilQt::getRandomColor());
        test_int_++;
        if(test_int_ > 4) test_int_ = 1;
        update();
    });
    timer->start(300);
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
    menu.clear();
    if(auto child = childAt(pos))
    {
        if(child == btn_application_) return;
    }

    auto action = toolbar_->actionAt(toolbar_->mapFromParent(pos));
    qInfo()<<__FUNCTION__<<action;
    if(action){
        if(auto dataItem = (QStandardItem *)action->data().value<void *>())
        {
            BookmarkMgr::Instance()->setMenuTriggerItem(dataItem);
            auto type = dataItem->data(BookmarkMgr::Type).toString();
            auto openAction = BookmarkMgr::Instance()->action_open_new_tab_;
            auto openWndAction = BookmarkMgr::Instance()->action_open_new_wnd_;
            auto openPrivateAction = BookmarkMgr::Instance()->action_open_in_private_;

            if(type == "folder"){
                openAction->setText(tr("open all in new tab") + QString("(%1)").arg(dataItem->rowCount()));
                openWndAction->setText(tr("open all in new window") + QString("(%1)").arg(dataItem->rowCount()));
                openPrivateAction->setText(tr("open all in private window") + QString("(%1)").arg(dataItem->rowCount()));
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
    }
    menu.addAction(BookmarkMgr::Instance()->action_add_current_);
    menu.addAction(BookmarkMgr::Instance()->action_add_folder_);
    menu.addSeparator();
    menu.addAction(BookmarkMgr::Instance()->action_show_bookmark_bar_);
    menu.addAction(BookmarkMgr::Instance()->action_show_bookmark_btn_);
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
    if(!providerWnd->loaded()){
        return;
    }
    // 加载书签栏
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

        //class name:QToolBarExtension
        // object name:qt_toolbar_ext_button
        if(item->objectName() == "qt_toolbar_ext_button"){
            auto widget = qobject_cast<QWidget *>(item);
            widget->setMinimumWidth(18);
        }
    }

    // 加载其它书签
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
    auto margins = contentsMargins();
    margins.setRight(8);
    setContentsMargins(margins);
}
