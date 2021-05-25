#include "TabBar.h"
#include "TabbarStyle.h"

#include <QtDebug>
#include <QMenu>
#include <QAction>
#include <QContextMenuEvent>
#include <QToolButton>

TabBar::TabBar(bool inprivate, QWidget *parent)
    : QTabBar(parent)
{
    setDrawBase(false);
    setTabsClosable(true);
    setStyle(new TabbarStyle(inprivate));

    menu_ = new QMenu(this);
    menu_->setWindowFlag(Qt::NoDropShadowWindowHint);
    menu_->setWindowFlag(Qt::FramelessWindowHint);
    menu_->setAttribute(Qt::WA_TranslucentBackground);
    menu_->setObjectName("TabBarTabMenu");
    act_new_tab = new QAction( tr("Create a new tab page"),this);
    act_new_tab->setShortcut(QKeySequence(tr("Ctrl+T")));
    act_reload_  = new QAction( tr("Reload"),this);
    act_reload_->setShortcut(QKeySequence(tr("Ctrl+R")));
    act_mute_ = new QAction( tr("Mute the page"),this);
    act_mute_->setIcon(QIcon(":/icons/resources/imgs/normal_opensound.png"));
    act_close_this_     = new QAction( tr("Close this tab page"),this);
    act_close_this_->setShortcut(QKeySequence(tr("Ctrl+W")));
    act_close_others_   = new QAction(tr("Close other tab pages") ,this);
    act_close_right_    = new QAction(tr("Close all on the right"), this);
    act_reopen_closed_ = new QAction(tr("Reopen closed tab pages"), this);
    act_vertical_tab_mode_ = new QAction(tr("Open vertical tab"), this);
    act_add_all_favorates_ = new QAction(tr("Add all pages to favorates"), this);

    menu_->addAction(act_new_tab);
    menu_->addSeparator();

    menu_->addAction(act_reload_);
    menu_->addAction(act_mute_);
    menu_->addSeparator();

    menu_->addAction(act_close_this_);
    menu_->addAction(act_close_right_);
    menu_->addAction(act_close_others_);
    menu_->addSeparator();
    menu_->addAction(act_vertical_tab_mode_);
    menu_->addAction(act_reopen_closed_);
    menu_->addAction(act_add_all_favorates_);

    setIcons();

    connect(act_new_tab, &QAction::triggered, this, [this](){
        emit menuTriggered(TabBarCmd::NewTabPage, "");
    });
    connect(act_reload_, &QAction::triggered, this,[this](){

        emit menuTriggered(TabBarCmd::Reload, "");
    });
    connect(act_mute_, &QAction::triggered, this,[this](){

        emit menuTriggered(TabBarCmd::Mute, "");
    });
    connect(act_close_this_, &QAction::triggered, this,[this](){
        emit menuTriggered(TabBarCmd::CloseTab, "");
    });
    connect(act_close_right_, &QAction::triggered, this,[this](){
        emit menuTriggered(TabBarCmd::CloseRight, "");
    });
    connect(act_close_others_, &QAction::triggered, this,[this](){
        emit menuTriggered(TabBarCmd::CloseOther, "");
    });


    check_pos_timer_id_ = startTimer(1000);
}

bool TabBar::event(QEvent *e)
{
    switch(e->type()){
    case QEvent::ToolTip:
    {
        auto helpEvent = static_cast<QHelpEvent *>(e);
        auto index = tabAt(helpEvent->pos());   // 鼠标所在tab索引
        auto rect = tabRect(index);
        auto pos = QPoint(rect.x() + rect.width() / 2, rect.y() + rect.height());
        pos = mapToGlobal(pos);
//        emit showPreview(pos, index);
    }break;
    default:
        break;
    }
    return QTabBar::event(e);
}

int TabBar::insertTab(int index, const QString &text)
{
    int ret = QTabBar::insertTab(index, text);
//    QToolButton *btn = new QToolButton;
//    btn->setToolTip(tr("mute this page"));
//    btn->setIcon(QIcon(":/icons/resources/newIcons/sound.png"));
//    setTabButton(ret, QTabBar::LeftSide, btn);
    return ret;
}

void TabBar::timerEvent(QTimerEvent *event)
{
    QTabBar::timerEvent(event);
    if(event->timerId() == check_pos_timer_id_){
        auto pos = QCursor::pos();
        pos = mapFromGlobal(pos);
        if(tabAt(pos) == -1)
            emit showPreview(QPoint(), -1);
    }
}

void TabBar::setIcons()
{
    act_new_tab->setIcon(QIcon(":/icons/resources/newIcons/new_window_dark_100px.png"));
    act_reload_->setIcon(QIcon(":/icons/resources/newIcons/reload_drak_100px.png"));
    act_mute_->setIcon(QIcon(":/icons/resources/newIcons/mute_dark_100px.png"));
    act_close_this_->setIcon(QIcon(":/icons/resources/newIcons/delete_dark_64px.png"));
}

void TabBar::mousePressEvent(QMouseEvent *event)
{
    QTabBar::mousePressEvent(event);
    emit showPreview(QPoint(), -1);
}

QSize TabBar::tabSizeHint(int index) const
{
#ifdef Q_OS_WIN
    int dpi = this->logicalDpiX();
    switch (dpi) {
    case 96:
        return QSize(240, 34);
    case 120:
        return QSize(300, 40);
    case 144:
        return QSize(360, 50);
    case 168:
        return QSize(420, 58);
    default:
        break;
    }
    return QTabBar::tabSizeHint(index);
#else
    return QSize(240, 32);
#endif
}

QSize TabBar::minimumTabSizeHint(int index) const
{
    return QSize(36,36);
    Q_UNUSED(index)
}

void TabBar::contextMenuEvent(QContextMenuEvent *event)
{
    this->count() > 1 ? act_close_others_->setEnabled(true):
                        act_close_others_->setEnabled(false);

    int index = this->tabAt(event->pos());

    this->count() - 1 > index ? act_close_right_->setEnabled(true):
                            act_close_right_->setEnabled(false);

    menu_->exec(event->globalPos());
}
