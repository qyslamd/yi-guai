#ifndef NAVIGATEBAR_H
#define NAVIGATEBAR_H

#include <QFrame>
#include "globaldef.h"

class QHBoxLayout;
class QToolButton;
class AddressBar;
class QMenu;
class QAction;
class QLabel;
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
public slots:
    void onHistoryPopupVisibleChange(bool visible);
    void onUserInfoPopupVisibleChange(bool visible);
protected:
    void paintEvent(QPaintEvent *event) override;
private:
    QHBoxLayout *layout_;

    QToolButton *btn_back_,
    *btn_refresh_,
    *btn_stop_,
    *btn_forward_,
    *btn_home_,
    *btn_user_,
    *btn_more_options_;

    AddressBar *address_bar_;
    QFrame *frame_extensions_,
    *frame_tools_;

    QToolButton *btn_bookmarks_,
    *btn_history_,
    *btn_download_,
    *btn_capture_;

    QMenu *menu_more_options_ = nullptr,
    *menu_more_tools_ = nullptr,
    *menu_help_ = nullptr;

    QAction *action_new_tab_ = nullptr;
    QAction * action_new_window_,
    *action_new_inprivate_window_;

    QWidgetAction *action_zoom_;
    QFrame *frame_zoom_;
    QLabel *label_zoom_,
    *label_zoom_value_;
    decltype (btn_back_) btn_zoom_out_,
    btn_zoom_in_,
    btn_fullscreen_;

    decltype (action_new_tab_) action_favorates_,
    action_history_,
    action_download_,
    action_print_,
    action_capture_,
    action_find_,
    action_more_tools_,
    action_task_mgr_,
    action_dev_tools_,
    action_settings_,
    action_helps_,
    action_help_you_,
    action_feed_back_,
    action_like_,
    action_about_qt_,
    action_about_cef_,
    action_quit_;

private:
    void initSignals();
    void setAppearance();

};

#endif // NAVIGATEBAR_H
