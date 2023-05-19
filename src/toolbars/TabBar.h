#ifndef TABBAR_H
#define TABBAR_H

#include <QTabBar>
#include "globaldef.h"

class QAction;
class QMenu;
class QContextMenuEvent;
class StyledMenu;

class TabBar final: public QTabBar
{
    Q_OBJECT
    friend class TabPageToolBar;
    TabBar(bool inprivate, QWidget *parent = nullptr);
public:
    bool event(QEvent *e) override;
    int insertTab(int index, const QString &text);
    void setTabHasAudio(int index, bool has);
signals:
    void menuTriggered(TabBarCmd cmd, const QVariant &data);
    void showPreview(const QPoint &g_pos, const int index);
protected:
    // QTabBar interface
    QSize tabSizeHint(int index) const override;
    QSize minimumTabSizeHint(int index) const override;
    void tabInserted(int index) override;
    void tabRemoved(int index) override;

    // QWidget interface
    void contextMenuEvent(QContextMenuEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void timerEvent(QTimerEvent* event) override;
    void resizeEvent(QResizeEvent *event) override;
private:
    bool inprivate_;
    StyledMenu *menu_;
    QAction *act_new_tab;
    decltype (act_new_tab) act_reload_,
    act_mute_,
    act_close_this_,
    act_close_right_,
    act_close_others_,
    act_reopen_closed_,
    act_vertical_tab_mode_,
    act_add_all_favorates_;

    int check_pos_timer_id_;
    int draw_progress_timer_id_;
    int menu_triggered_index_;

    const int LHideW = 80;
    const int RHideW = 40;

    void initUi();
    void initSignalSlots();
    void setIcons();
    void showHideTabButton();
};

#endif // TABBAR_H
