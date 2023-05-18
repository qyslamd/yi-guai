#include "TabBar.h"
#include "TabbarStyle.h"

#include <QtDebug>
#include <QMenu>
#include <QAction>
#include <QContextMenuEvent>
#include <QToolButton>
#include <QGraphicsDropShadowEffect>
#include <QChildEvent>
#include <QAbstractButton>

#include "popups/StyledMenu.h"

TabBar::TabBar(bool inprivate, QWidget *parent)
    : QTabBar(parent)
    , inprivate_(inprivate)
{
    initUi();
    initSignalSlots();
    setIcons();
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
    case QEvent::ChildAdded:
    {
//        auto childEvent = static_cast<QChildEvent *>(e);
//        auto child = childEvent->child();
//        if(child){
//            qInfo()<<child;
//        }
    }break;
    default:
        break;
    }
    return QTabBar::event(e);
}

int TabBar::insertTab(int index, const QString &text)
{
    int ret = QTabBar::insertTab(index, style()->standardIcon(QStyle::SP_FileIcon), text);
    return ret;
}

void TabBar::setTabHasAudio(int index, bool has)
{
    if(has){
        QToolButton *btn = new QToolButton;
        btn->setMinimumSize(18, 18);
        btn->setToolTip(tr("mute this page"));
        if(!inprivate_){
            btn->setIcon(QIcon(":/icons/resources/imgs/light/sound_64px.png"));
        }else{
            btn->setIcon(QIcon(":/icons/resources/imgs/dark/sound_64px.png"));
        }
        setTabButton(index, QTabBar::LeftSide, btn);

        auto tabWidth = tabRect(index);
        if(!tabWidth.isEmpty()){
           btn->setVisible(tabWidth.width() > LHideW);
        }
    }else{
        if(auto btn = tabButton(index, QTabBar::LeftSide))
        {
            setTabButton(index, QTabBar::LeftSide, nullptr);
            delete btn;
            btn = nullptr;
        }
    }
}

void TabBar::timerEvent(QTimerEvent *event)
{
    QTabBar::timerEvent(event);
    auto timerId = event->timerId();
    if(timerId == check_pos_timer_id_){
        auto pos = QCursor::pos();
        pos = mapFromGlobal(pos);
        if(tabAt(pos) == -1)
            emit showPreview(QPoint(), -1);
    }else if(timerId == draw_progress_timer_id_){
        update();
    }
}

void TabBar::resizeEvent(QResizeEvent *event)
{
    QTabBar::resizeEvent(event);
    showHideTabButton();
}

void TabBar::tabInserted(int index)
{
    update();
    if(auto closeBtn = tabButton(index, QTabBar::RightSide)){
        if(auto btn = qobject_cast<QAbstractButton *>(closeBtn))
        {
            btn->setMinimumSize(18, 18);
            if(inprivate_){
                btn->setIcon(QIcon(":/icons/resources/imgs/dark/delete_64px.png"));
            }else{
                btn->setIcon(QIcon(":/icons/resources/imgs/light/delete_64px.png"));
            }
        }
    }
    showHideTabButton();
}

void TabBar::tabRemoved(int index)
{
    Q_UNUSED(index);
    showHideTabButton();
}

void TabBar::initUi()
{
    setDrawBase(false);
    setTabsClosable(true);
    setMovable(true);
    setStyle(new TabbarStyle(this, inprivate_));

    menu_ = new StyledMenu(this);

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

    //    check_pos_timer_id_ = startTimer(1000);

    draw_progress_timer_id_ = startTimer(33);
}

void TabBar::initSignalSlots()
{
    connect(act_new_tab, &QAction::triggered, this, [this](){
        emit menuTriggered(TabBarCmd::NewTabPage, menu_triggered_index_);
    });
    connect(act_reload_, &QAction::triggered, this,[this](){

        emit menuTriggered(TabBarCmd::Reload, menu_triggered_index_);
    });
    connect(act_mute_, &QAction::triggered, this,[this](){

        emit menuTriggered(TabBarCmd::Mute, menu_triggered_index_);
    });
    connect(act_close_this_, &QAction::triggered, this,[this](){
        emit menuTriggered(TabBarCmd::CloseTab, menu_triggered_index_);
    });
    connect(act_close_right_, &QAction::triggered, this,[this](){
        emit menuTriggered(TabBarCmd::CloseRight, menu_triggered_index_);
    });
    connect(act_close_others_, &QAction::triggered, this,[this](){
        emit menuTriggered(TabBarCmd::CloseOther, menu_triggered_index_);
    });
}

void TabBar::setIcons()
{
    act_new_tab->setIcon(QIcon(":/icons/resources/imgs/light/add_tab_64px.png"));
    act_reload_->setIcon(QIcon(":/icons/resources/imgs/light/refresh_64px.png"));
    act_mute_->setIcon(QIcon(":/icons/resources/imgs/light/no_audio_64px.png"));
    act_close_this_->setIcon(QIcon(":/icons/resources/imgs/light/delete_64px.png"));
}

void TabBar::showHideTabButton()
{
    auto tabWidth = tabRect(0);
    if(tabWidth.isEmpty()){
        return;
    }

    for(int i =0; i< count(); i++){
        if(auto lBtn = tabButton(i, QTabBar::LeftSide)){
            lBtn->setVisible(tabWidth.width() > LHideW);
        }

        if(auto rBtn = tabButton(i, QTabBar::RightSide)){
            rBtn->setVisible(tabWidth.width() > RHideW);
        }
    }
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
    return QSize(36, 32);
    Q_UNUSED(index)
}

void TabBar::contextMenuEvent(QContextMenuEvent *event)
{
    this->count() > 1 ? act_close_others_->setEnabled(true):
                        act_close_others_->setEnabled(false);

    int index = this->tabAt(event->pos());
    menu_triggered_index_ = index;
    qInfo()<<__FUNCTION__<<index;

    this->count() - 1 > index ? act_close_right_->setEnabled(true):
                            act_close_right_->setEnabled(false);

    menu_->exec(event->globalPos());
}
