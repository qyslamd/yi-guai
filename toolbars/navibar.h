#ifndef NAVIBAR_H
#define NAVIBAR_H

#include <QFrame>
#include "globaldef.h"

class QHBoxLayout;
class QToolButton;
class AddressBar;
class QMenu;
class QAction;
class QWidgetAction;

class NaviBar final: public QFrame
{
    Q_OBJECT
public:
    NaviBar(QWidget *parent = nullptr);

    void setAddress(const QString &url);
    void setLoadingState(bool isLoading, bool canGoBack, bool canGoForward);
    void setFocus(bool focus);

    bool eventFilter(QObject *obj, QEvent *ev) override;

signals:
    void naviBarCmd(NaviBarCmd cmd, const QVariant &para);
protected:
    void paintEvent(QPaintEvent *event) override;
private:
    QHBoxLayout *layout_;   /*整体布局*/

    QToolButton *btn_back_; /*后退*/
    decltype (btn_back_) btn_refresh_,  /*刷新*/
    btn_stop_,  /*停止加载*/
    btn_forward_,  /*前进*/
    btn_home_,  /*主页*/
    btn_user_,   /*用户*/
    btn_more_options_; /*其它选项*/

    AddressBar *address_bar_;   /*地址栏及其孩子*/
    QFrame *frame_extensions_,  /*扩展容器*/
    *frame_tools_;  /*工具容器*/

    QMenu *menu_more_options_ = nullptr;  /*更多选项菜单*/
    QAction *action_new_tab_ = nullptr;   /*新标签页*/
    decltype (action_new_tab_) action_new_window_,  /*新建窗口*/
    action_new_inprivate_window_;   /*新建无痕窗口*/
    QWidgetAction *action_zoom_;    /*缩放栏Action*/
    decltype (action_new_tab_) action_favorates_,  /*收藏夹*/
    action_history_,    /*历史记录*/
    action_download_,   /*下载*/
    action_print_,  /*打印*/
    action_capture_,    /*截图和截屏*/
    action_find_,   /*查找*/
    action_more_tools_, /*更多工具*/
    action_settings_,   /*设置*/
    action_help_,   /*帮助和反馈*/
    action_quit_;    /*退出应用*/

private:
    void initSignals();
    void setAppearance();

};

#endif // NAVIBAR_H
