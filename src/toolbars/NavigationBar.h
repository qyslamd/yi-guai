#ifndef NAVIGATIONBAR_H
#define NAVIGATIONBAR_H

#include <QFrame>
#include "globaldef.h"

class QHBoxLayout;
class QToolButton;
class QPushButton;
class AddressBar;
class SearchBar;
class QMenu;
class QAction;
class QLabel;
class QWidgetAction;
class ZoomPopup;
class StyledMenu;

class NavigateToolBar final: public QFrame
{
    Q_OBJECT
public:
    explicit NavigateToolBar(bool inprivate, QWidget *parent = nullptr);

    void setAddress(const QString &url,bool edited = false);
    void setLoadingState(bool isLoading, bool canGoBack, bool canGoForward);
    void setZoomLevelValue(double value);
    void setFocus(bool focus);
    bool eventFilter(QObject *obj, QEvent *ev) override;

    QPoint addBkmkBtnPos() const;
    QPoint historyBtnPos() const;
    QPoint bookmarkBtnPos() const;
    QPoint downloadBtnPos() const;
    QPoint inprivateBtnPos() const;
    QPoint userBtnPos() const;
    QPoint zoomBtnPos() const;
    QPoint findHintBtnPos() const;
    void inpWndCntChanged();
    void updatePreference();
signals:
    void naviBarCmd(NaviBarCmd cmd, const QVariant &para);
public slots:
    void onToolWndVisibleChanged(ToolWndType type, bool visible);
protected:
    void paintEvent(QPaintEvent *event) override;
private:
    bool inprivate_ = false;
    QHBoxLayout *layout_;

    QToolButton *btn_back_;
    QToolButton *btn_refresh_;
    QToolButton *btn_stop_;
    QToolButton *btn_forward_;
    QToolButton *btn_home_;
    QToolButton *btn_favorites_;
    QToolButton *btn_history_;
    QToolButton *btn_download_;
    QToolButton *btn_capture_;
    QPushButton *btn_inprivate_;
    QToolButton *btn_user_;
    QToolButton *btn_more_options_;

    AddressBar *address_bar_;
    SearchBar *search_bar_;
    QFrame *frame_extensions_;
    QFrame *frame_tools_;

    StyledMenu *menu_more_options_ = nullptr;
    StyledMenu *menu_favorite_ = nullptr;
    StyledMenu *menu_more_tools_ = nullptr;
    StyledMenu *menu_help_ = nullptr;

    QAction *action_new_tab_ = nullptr;
    QAction * action_new_window_;
    QAction *action_new_inprivate_window_;

    QWidgetAction *action_zoom_;
    QFrame *frame_zoom_bar_;
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
    void initUi();
    void initSignalSlot();
    void setIcons();

};

#endif // NAVIGATIONBAR_H
