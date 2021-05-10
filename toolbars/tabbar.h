#ifndef TABBAR_H
#define TABBAR_H

#include <QTabBar>
#include "globaldef.h"

class QAction;
class QMenu;
class QContextMenuEvent;

class TabBar final: public QTabBar
{
    Q_OBJECT
public:
    TabBar(QWidget *parent = nullptr);
    bool event(QEvent *e) override;
signals:
    void menuTriggered(TabBarCmd cmd, const QVariant &data);
    void showPreview(const QPoint &g_pos, const int index);
protected:
    // QTabBar interface
    QSize tabSizeHint(int index) const override;
    QSize minimumTabSizeHint(int index) const override;

    // QWidget interface
    void contextMenuEvent(QContextMenuEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void timerEvent(QTimerEvent* event) override;
private:
    QMenu *menu_;
    QAction *act_new_tab;
    decltype (act_new_tab) act_reload_,/*刷新*/
    act_mute_,  /*使得标签页静音*/
    act_close_this_,    /*关闭鼠标所在标签页*/
    act_close_right_,   /*关关闭鼠标所在标签页的右侧的所有标签页*/
    act_close_others_,  /*关关闭除了鼠标所在标签页的其它所有标签页*/
    act_reopen_closed_,    /*重新打开关闭的标签页*/
    act_vertical_tab_mode_, /*垂直标签页模式*/
    act_add_all_favorates_; /*添加所有标签页到收藏夹*/

    int check_pos_timer_id_;
};

#endif // TABBAR_H
