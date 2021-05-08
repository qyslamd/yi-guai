#include "tabbar.h"
#include "tabbarstyle.h"

#include <QMenu>
#include <QAction>
#include <QContextMenuEvent>

TabBar::TabBar(QWidget *parent)
    : QTabBar(parent)
{
    setDrawBase(false);
    setTabsClosable(true);
    setStyle(new TabbarStyle(false));

    menu_ = new QMenu(this);
    act_new_tab = new QAction( tr("Create a new tab page"),this);
    act_new_tab->setShortcut(QKeySequence(tr("Ctrl+T")));
    act_reload_  = new QAction( tr("Reload"),this);
    act_reload_->setShortcut(QKeySequence(tr("Ctrl+R")));
    act_mute_ = new QAction( tr("Mute the page"),this);
    act_close_this_     = new QAction( tr("Close this tab page"),this);
    act_close_this_->setShortcut(QKeySequence(tr("Ctrl+W")));
    act_close_others_   = new QAction(tr("Close other tab-pages") ,this);
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
