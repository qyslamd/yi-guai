#include "NavigationBar.h"
#include "AddressBar.h"
#include "SearchBar.h"
#include "utils/util_qt.h"

#include "managers/AppCfgManager.h"
#include "browser/CefManager.h"
#include "managers/MainWindowManager.h"
#include "managers/BookmarkManager.h"
#include "popups/ZoomPopup.h"
#include "popups/StyledMenu.h"

#include <QtDebug>
#include <QHBoxLayout>
#include <QToolButton>
#include <QPushButton>
#include <QLabel>
#include <QPainter>
#include <QMenu>
#include <QWidgetAction>
#include <QShowEvent>
#include <QStyle>
#include <QTimer>

NavigateToolBar::NavigateToolBar(bool inprivate, QWidget *parent)
    : QFrame(parent)
    , inprivate_(inprivate)

{
    initUi();
    initSignalSlot();
    setIcons();
}

void NavigateToolBar::setAddress(const QString &url, bool edited)
{
    address_bar_->setText(url, edited);
    address_bar_->setCursorPosition(0);
}

void NavigateToolBar::setLoadingState(bool isLoading, bool canGoBack, bool canGoForward)
{
    btn_back_->setEnabled(canGoBack);
    btn_forward_->setEnabled(canGoForward);
    btn_refresh_->setVisible(!isLoading);
    btn_stop_->setVisible(isLoading);
}

void NavigateToolBar::setZoomLevelValue(double value)
{
    auto zoomStr = CefManager::Instance().zoom_map.value(static_cast<int>(value));
    label_zoom_value_->setText(zoomStr);
    address_bar_->setZoomLevelValue(value);
}

void NavigateToolBar::setFocus(bool focus)
{
    if(focus){
        address_bar_->clearFocus();
    }
}

bool NavigateToolBar::eventFilter(QObject *obj, QEvent *ev)
{
    if(menu_more_options_ == obj){
        if(ev->type() == QEvent::Show){
            QTimer::singleShot(10, [this](){
                auto pos = mapToGlobal(btn_more_options_->pos());

                pos.setX(pos.x() - menu_more_options_->width());
                pos.setX(pos.x() + btn_more_options_->width());
                pos.setY(pos.y() + btn_more_options_->height());
                menu_more_options_->move(pos);
            });
        }
        // Filter zoom-bar's mouse enter event, and cancel more options menu's active action
    }else if(frame_zoom_bar_ == obj){
        if(ev->type() == QEvent::Enter)
        {
            menu_more_options_->setActiveAction(nullptr);
        }
    }
    return QFrame::eventFilter(obj, ev);
}

QPoint NavigateToolBar::addBkmkBtnPos() const
{
    return address_bar_->gGeometryBtnAddBkmk().bottomRight();
}

QPoint NavigateToolBar::historyBtnPos() const
{
    return mapToGlobal(btn_history_->geometry().bottomRight());
}

QPoint NavigateToolBar::bookmarkBtnPos() const
{
    return mapToGlobal(btn_favorites_->geometry().bottomRight());
}

QPoint NavigateToolBar::downloadBtnPos() const
{
    return mapToGlobal(btn_download_->geometry().bottomRight());
}

QPoint NavigateToolBar::inprivateBtnPos() const
{
    return mapToGlobal(btn_inprivate_->geometry().bottomRight());
}

QPoint NavigateToolBar::userBtnPos() const
{
    return mapToGlobal(btn_user_->geometry().bottomRight());
}

QPoint NavigateToolBar::zoomBtnPos() const
{
    return address_bar_->gGeometryBtnZoom().bottomRight();
}

QPoint NavigateToolBar::findHintBtnPos() const
{
    return address_bar_->findButtonGlobalPos();
}

void NavigateToolBar::inpWndCntChanged()
{
    if(inprivate_){
        auto cnt = MainWndMgr::Instance().inprivateCount();
        if(cnt <= 1){
            btn_inprivate_->setText(tr("InPrivate"));
        }else{
            btn_inprivate_->setText(tr("InPrivate(%1)").arg(cnt));
        }
    }
}

void NavigateToolBar::updatePreference()
{
    btn_favorites_->setVisible(AppCfgMgr::bookmarkBtnVisible());
}

void NavigateToolBar::onToolWndVisibleChanged(ToolWndType type, bool visible)
{
    switch (type) {
    case ToolWndType::AddFavorite:

        break;
    case ToolWndType::Favorite:
        btn_favorites_->setChecked(visible);
        break;
    case ToolWndType::History:
        btn_history_->setChecked(visible);
        break;
    case ToolWndType::Download:
        btn_download_->setChecked(visible);
        break;
    case ToolWndType::UserInfo:
        btn_user_->setChecked(visible);
        break;
    default:
        break;
    }
}

void NavigateToolBar::paintEvent(QPaintEvent *event)
{
    QFrame::paintEvent(event);

    if(!AppCfgMgr::Instance().bookmarkBarVisible()){
        QPainter p(this);
        p.save();
        qreal penWidth = 0.5f;
        QColor color(0xD2D2D2);
        p.setPen(QPen(QBrush(color), 0.5f));
        p.drawLine(0.0, height() - penWidth, width(), height() - penWidth);
        p.restore();
    }
}

void NavigateToolBar::initUi()
{
    layout_ = new QHBoxLayout;
    layout_->setContentsMargins(4,4,4,2);
    layout_->setSpacing(4);
    setLayout(layout_);

    btn_back_ = new QToolButton;
    btn_back_->setToolTip(tr("click to back, press to show more histories"));

    btn_refresh_ = new QToolButton;
    btn_refresh_->setToolTip(tr("refresh"));

    btn_stop_ = new QToolButton;
    btn_stop_->setToolTip(tr("stop loading"));

    btn_forward_ = new QToolButton;
    btn_forward_->setToolTip(tr("click to forward, press to show more histories"));

    btn_home_ = new QToolButton;
    btn_home_->setToolTip(tr("homepage"));


    address_bar_ = new AddressBar(inprivate_);

    QFrame* line = new QFrame(this);
//    line->setStyleSheet("margin-top:4px;margin-bottom:4px;");
    line->setStyleSheet("margin:4px;");
    line->setObjectName("line");
    line->setMaximumWidth(2);
    line->setFrameShape(QFrame::VLine);
    line->setFrameShadow(QFrame::Sunken);

    search_bar_ = new SearchBar(inprivate_);

    frame_extensions_ = new QFrame;
    frame_tools_ = new QFrame;

    btn_favorites_ = new QToolButton;
    btn_favorites_->setCheckable(true);
    btn_favorites_->setToolTip(tr("bookmarks"));

    btn_history_ = new QToolButton;
    btn_history_->setCheckable(true);
    btn_history_->setToolTip(tr("histories"));

    btn_download_ = new QToolButton;
    btn_download_->setCheckable(true);
    btn_download_->setToolTip(tr("downlaods"));

    btn_capture_ = new QToolButton;
    btn_capture_->setCheckable(true);
    btn_capture_->setToolTip(tr("screen shot"));

    btn_inprivate_ = new QPushButton;
    btn_inprivate_->setObjectName("NaviBarInprivateBtn");
    btn_inprivate_->setText(tr("InPrivate"));
    btn_inprivate_->setCheckable(true);
    btn_inprivate_->setToolTip(tr("You are browing InPrivate"));
    btn_inprivate_->setLayoutDirection(Qt::RightToLeft);

    btn_user_ = new QToolButton;
    btn_user_->setCheckable(true);
    btn_user_->setToolTip(tr("show user information"));

    btn_more_options_ = new QToolButton;
    btn_more_options_->setToolTip(tr("settings and other options"));

    menu_more_options_ = new StyledMenu(this);
    menu_more_options_->installEventFilter(this);
    menu_more_tools_ = new StyledMenu;
    menu_help_ = new StyledMenu;
    menu_favorite_ = new StyledMenu(this);

    action_new_tab_ = new QAction;
    action_new_tab_->setText(tr("create new tab page"));
    action_new_tab_->setShortcut(QKeySequence("Ctrl+T"));
    action_new_window_ = new QAction(this);
    action_new_window_->setText(tr("create new window"));
    action_new_window_->setShortcut(QKeySequence("Ctrl+N"));
    action_new_inprivate_window_ = new QAction;
    action_new_inprivate_window_->setText(tr("create new inprivate window"));
    action_new_inprivate_window_->setShortcut(QKeySequence("Ctrl+Shift+N"));

    action_zoom_ = new QWidgetAction(this);
    frame_zoom_bar_ = new QFrame;
    frame_zoom_bar_->installEventFilter(this);
    frame_zoom_bar_->setFrameShape(QFrame::NoFrame);
    frame_zoom_bar_->setObjectName("ZoomBarFrame");
    label_zoom_ = new QLabel(tr("Zoom"));
    label_zoom_value_ = new QLabel;
    label_zoom_value_->setObjectName("ZoomValueLabel");
    label_zoom_value_->setAlignment(Qt::AlignCenter);
    label_zoom_value_->setText("100%");
    label_zoom_value_->setMaximumWidth(42);
    btn_zoom_out_ = new QToolButton;
    btn_zoom_out_->setObjectName("ZoomOutToolButton");
    btn_zoom_in_ = new QToolButton;
    btn_zoom_in_->setObjectName("ZoomInToolButton");
    btn_fullscreen_ = new QToolButton;
    btn_fullscreen_->setObjectName("ZoomBarFullscrnButton");
    QSize zoomBtnSize(40,30);
    btn_zoom_out_->setFixedSize(zoomBtnSize);
    btn_zoom_in_->setFixedSize(zoomBtnSize);
    btn_fullscreen_->setFixedSize(zoomBtnSize);
    QHBoxLayout *zoom_layout = new QHBoxLayout;
    zoom_layout->setContentsMargins(0,0,0,0);
    zoom_layout->setSpacing(0);
    zoom_layout->addSpacerItem(new QSpacerItem(26,5, QSizePolicy::Fixed));
    zoom_layout->addWidget(label_zoom_);
    zoom_layout->addWidget(btn_zoom_out_);
    zoom_layout->addWidget(label_zoom_value_);
    zoom_layout->addWidget(btn_zoom_in_);
    zoom_layout->addWidget(btn_fullscreen_);
    frame_zoom_bar_->setLayout(zoom_layout);
    action_zoom_->setDefaultWidget(frame_zoom_bar_);

    action_favorates_ = new QAction;
    action_favorates_->setText(tr("favorates"));
    menu_favorite_->addAction(BookmarkMgr::Instance()->action_add_current_);
    menu_favorite_->addAction(BookmarkMgr::Instance()->action_add_all_);
    menu_favorite_->addSeparator();
    menu_favorite_->addAction(BookmarkMgr::Instance()->action_show_bookmark_bar_);
    menu_favorite_->addAction(BookmarkMgr::Instance()->action_show_bookmark_btn_);
    menu_favorite_->addAction(BookmarkMgr::Instance()->action_manage_bookmarks_);
    action_favorates_->setMenu(menu_favorite_);

    action_history_ = new QAction;
    action_history_->setText(tr("histories"));
    action_history_->setShortcut(QKeySequence("Ctrl+H"));
    action_download_ = new QAction;
    action_download_->setText(tr("downloads"));
    action_download_->setShortcut(QKeySequence("Ctrl+J"));
    action_print_ = new QAction;
    action_print_->setText(tr("print"));
    action_print_->setShortcut(QKeySequence("Ctrl+P"));
    action_capture_ = new QAction;
    action_capture_->setText(tr("capture"));
    action_find_ = new QAction;
    action_find_->setText(tr("find in this page"));

    action_find_->setShortcut(QKeySequence("Ctrl+F"));
    action_more_tools_ = new QAction;
    action_more_tools_->setText(tr("more tools"));
    action_task_mgr_ = new QAction;
    action_task_mgr_->setText(tr("browser task manager"));

    action_dev_tools_ = new QAction;
    action_dev_tools_->setText(tr("developer tool"));
    action_settings_ = new QAction;
    action_settings_->setText(tr("settings"));

    action_helps_ = new QAction;
    action_helps_->setText(tr("help and feedback"));
    action_help_you_ = new QAction;
    action_help_you_->setText(tr("help"));
    action_feed_back_ = new QAction;
    action_feed_back_->setText(tr("send feed back"));
    action_like_ = new QAction;
    action_like_->setText(tr("give a like"));
    action_about_qt_ = new QAction;
    action_about_qt_->setText(tr("about qt"));
    action_about_cef_ = new QAction;
    action_about_cef_->setText(tr("about cef"));
    action_quit_ = new QAction;
    action_quit_->setText(tr("quit app"));

    menu_more_options_->addAction(action_new_tab_);
    menu_more_options_->addAction(action_new_window_);
    menu_more_options_->addAction(action_new_inprivate_window_);
    //    menu_more_options_->addSeparator();
    menu_more_options_->addAction(action_zoom_);
    //    menu_more_options_->addSeparator();
    menu_more_options_->addAction(action_favorates_);
    menu_more_options_->addAction(action_history_);
    menu_more_options_->addAction(action_download_);
    menu_more_options_->addSeparator();
    menu_more_options_->addAction(action_print_);
    menu_more_options_->addAction(action_capture_);
    menu_more_options_->addAction(action_find_);
    menu_more_options_->addAction(action_more_tools_);
    menu_more_options_->addSeparator();
    menu_more_options_->addAction(action_settings_);
    menu_more_options_->addAction(action_helps_);
    menu_more_options_->addAction(action_quit_);

    btn_more_options_->setObjectName("NaviBarOptionBtn");
    btn_more_options_->setMenu(menu_more_options_);
    btn_more_options_->setPopupMode(QToolButton::InstantPopup);

    menu_more_tools_->addAction(action_task_mgr_);
    menu_more_tools_->addAction(action_dev_tools_);
    action_more_tools_->setMenu(menu_more_tools_);

    menu_help_->addAction(action_help_you_);
    menu_help_->addAction(action_feed_back_);
    menu_help_->addAction(action_like_);
    menu_help_->addAction(action_about_qt_);
    menu_help_->addAction(action_about_cef_);
    action_helps_->setMenu(menu_help_);

    layout_->addWidget(btn_back_);
    layout_->addWidget(btn_forward_);
    layout_->addWidget(btn_refresh_);
    layout_->addWidget(btn_stop_);
    layout_->addWidget(btn_home_);
    layout_->addWidget(address_bar_);
    layout_->addSpacerItem(new QSpacerItem(4,10,QSizePolicy::Fixed));
    layout_->addWidget(line);
    layout_->addSpacerItem(new QSpacerItem(4,10,QSizePolicy::Fixed));
    layout_->addWidget(search_bar_);
    layout_->addWidget(frame_extensions_);
    layout_->addWidget(frame_tools_);
    layout_->addWidget(btn_favorites_);
    layout_->addWidget(btn_history_);
    layout_->addWidget(btn_download_);
    layout_->addWidget(btn_capture_);
    layout_->addWidget(btn_inprivate_);
    layout_->addWidget(btn_user_);
    layout_->addWidget(btn_more_options_);

    btn_favorites_->setVisible(AppCfgMgr::bookmarkBtnVisible());
}

void NavigateToolBar::initSignalSlot()
{
    connect(address_bar_, &AddressBar::returnPressed, this, [this]()
    {
        emit naviBarCmd(NaviBarCmd::Navigate, address_bar_->text());
    });
    connect(address_bar_, &AddressBar::viewSiteInfo, this, [this]()
    {
        auto rect = address_bar_->gGeometryBtnSiteInfo();
        emit naviBarCmd(NaviBarCmd::ViewSiteInfo, rect.bottomLeft());
    });
    connect(address_bar_, &AddressBar::addFavorite, this, [this]()
    {
        auto rect = address_bar_->gGeometryBtnAddBkmk();
        emit naviBarCmd(NaviBarCmd::AddFavorite, rect.bottomRight());
    });
    connect(address_bar_, &AddressBar::textEdited, this, [this](const QString &txt)
    {
        emit naviBarCmd(NaviBarCmd::AddressEdited, txt);
    });
    connect(address_bar_, &AddressBar::showZoomBar, this, [this]()
    {
        auto rect = address_bar_->gGeometryBtnZoom();
        emit naviBarCmd(NaviBarCmd::ShowZoomBar, rect.bottomRight());
    });
    connect(btn_back_, &QToolButton::clicked, this, [this]()
    {
        emit naviBarCmd(NaviBarCmd::Back, QVariant());
    });
    connect(btn_home_, &QToolButton::clicked, this, [this]()
    {
        emit naviBarCmd(NaviBarCmd::HomePage, QVariant());
    });
    connect(btn_forward_, &QToolButton::clicked, this, [this]()
    {
        emit naviBarCmd(NaviBarCmd::Forward, QVariant());
    });
    connect(btn_refresh_, &QToolButton::clicked, this, [this]()
    {
        emit naviBarCmd(NaviBarCmd::Refresh, QVariant());
    });
    connect(btn_stop_, &QToolButton::clicked, this, [this]()
    {
        emit naviBarCmd(NaviBarCmd::StopLoading, QVariant());
    });
    connect(btn_favorites_, &QToolButton::clicked, this, [this]()
    {
        auto pos = mapToGlobal(btn_favorites_->geometry().bottomRight());
        emit naviBarCmd(NaviBarCmd::Favorite, pos);
    });
    connect(btn_history_, &QToolButton::clicked, this, [this]()
    {
        auto pos = mapToGlobal(btn_history_->geometry().bottomRight());
        emit naviBarCmd(NaviBarCmd::History, pos);
    });
    connect(btn_download_, &QToolButton::clicked, this, [this]()
    {
        auto pos = mapToGlobal(btn_download_->geometry().bottomRight());
        emit naviBarCmd(NaviBarCmd::Download, pos);
    });
    connect(btn_capture_, &QToolButton::clicked, this, [this]()
    {
        auto pos = mapToGlobal(btn_capture_->geometry().bottomRight());
        emit naviBarCmd(NaviBarCmd::Capture, pos);
    });
    connect(btn_inprivate_, &QPushButton::clicked, this, [this]()
    {
        auto pos = mapToGlobal(btn_inprivate_->geometry().bottomRight());
        emit naviBarCmd(NaviBarCmd::Inprivate, pos);
    });
    connect(btn_user_, &QToolButton::clicked, this, [this]()
    {
        auto pos = mapToGlobal(btn_user_->geometry().bottomRight());
        emit naviBarCmd(NaviBarCmd::User, pos);
    });
    connect(action_new_tab_, &QAction::triggered, this, [this]()
    {
        emit naviBarCmd(NaviBarCmd::NewTabPage, QVariant());
    });
    connect(action_new_window_, &QAction::triggered, this, [this]()
    {
        qInfo()<<"new window";
        emit naviBarCmd(NaviBarCmd::NewWindow, QVariant());
    });
    connect(action_new_inprivate_window_, &QAction::triggered, this, [this]()
    {
        emit naviBarCmd(NaviBarCmd::NewInprivateWindow, QVariant());
    });
    connect(btn_zoom_out_, &QToolButton::clicked, this, [this]()
    {
        emit naviBarCmd(NaviBarCmd::ZoomOut, QVariant());
    });
    connect(btn_zoom_in_, &QToolButton::clicked, this, [this]()
    {
        emit naviBarCmd(NaviBarCmd::ZoomIn, QVariant());
    });
    connect(btn_fullscreen_, &QToolButton::clicked, this, [this]()
    {
        emit naviBarCmd(NaviBarCmd::FullScreen, QVariant());
        menu_more_options_->hide();
    });
    connect(action_favorates_, &QAction::triggered, this, [this]()
    {
        emit naviBarCmd(NaviBarCmd::Favorite, QVariant());
    });
    connect(action_history_, &QAction::triggered, this, [this]()
    {
        auto pos = mapToGlobal(btn_history_->geometry().bottomRight());
        emit naviBarCmd(NaviBarCmd::History, pos);
    });
    connect(action_download_, &QAction::triggered, this, [this]()
    {
        emit naviBarCmd(NaviBarCmd::Download, QVariant());
    });
    connect(action_print_, &QAction::triggered, this, [this]()
    {
        emit naviBarCmd(NaviBarCmd::Print, QVariant());
    });
    connect(action_capture_, &QAction::triggered, this, [this]()
    {
        emit naviBarCmd(NaviBarCmd::Capture, QVariant());
    });
    connect(action_find_, &QAction::triggered, this, [this]()
    {
        emit naviBarCmd(NaviBarCmd::Find, QVariant());
    });
    connect(action_task_mgr_, &QAction::triggered, this, [this]()
    {
        emit naviBarCmd(NaviBarCmd::TaskMgr, QVariant());
    });
    connect(action_dev_tools_, &QAction::triggered, this, [this]()
    {
        emit naviBarCmd(NaviBarCmd::DevTool, QVariant());
    });
    connect(action_settings_, &QAction::triggered, this, [this]()
    {
        emit naviBarCmd(NaviBarCmd::Settings, QVariant());
    });
    connect(action_help_you_, &QAction::triggered, this, [this]()
    {
        emit naviBarCmd(NaviBarCmd::About, QVariant());
    });
    connect(action_feed_back_, &QAction::triggered, this, [this]()
    {
        emit naviBarCmd(NaviBarCmd::Feedback, QVariant());
    });
    connect(action_like_, &QAction::triggered, this, [this]()
    {
        emit naviBarCmd(NaviBarCmd::Like, QVariant());
    });
    connect(action_about_qt_, &QAction::triggered, this, [this]()
    {
        emit naviBarCmd(NaviBarCmd::AboutQt, QVariant());
    });
    connect(action_about_cef_, &QAction::triggered, this, [this]()
    {
        emit naviBarCmd(NaviBarCmd::AboutCef, QVariant());
    });
    connect(action_quit_, &QAction::triggered, this, [this]()
    {
        emit naviBarCmd(NaviBarCmd::QuitApp, QVariant());
    });

}

void NavigateToolBar::setIcons()
{
    btn_inprivate_->setVisible(inprivate_);
    btn_user_->setVisible(!inprivate_);
    btn_history_->setVisible(!inprivate_);
    btn_download_->setVisible(!inprivate_);
    if(inprivate_){
        btn_back_->setIcon(QIcon(":/icons/resources/imgs/dark/left_64px.png"));
        btn_refresh_->setIcon(QIcon(":/icons/resources/imgs/dark/refresh_64px.png"));
        btn_stop_->setIcon(QIcon(":/icons/resources/imgs/dark/delete_64px.png"));
        btn_forward_->setIcon(QIcon(":/icons/resources/imgs/dark/right_64px.png"));
        btn_home_->setIcon(QIcon(":/icons/resources/imgs/dark/home_64px.png"));
        btn_favorites_->setIcon(QIcon(":/icons/resources/imgs/dark/favorite_window_64px.png"));
        btn_history_->setIcon(QIcon(":/icons/resources/imgs/dark/time_machine_64px.png"));
        btn_download_->setIcon(QIcon(":/icons/resources/imgs/dark/download_64px.png"));
        btn_capture_->setIcon(QIcon(":/icons/resources/imgs/dark/screenshot_64px.png"));
        btn_inprivate_->setIcon(QIcon(":/icons/resources/imgs/dark/private_64px.png"));
        btn_user_->setIcon(QIcon(":/icons/resources/imgs/dark/user_64px.png"));
        btn_more_options_->setIcon(QIcon(":/icons/resources/imgs/dark/squared_menu_48px.png"));
    }else{
#if BLACK_BOLD
        btn_back_->setIcon(QIcon(":/icons/resources/imgs/light/left_64px.png"));
        btn_refresh_->setIcon(QIcon(":/icons/resources/imgs/light/refresh_64px.png"));
        btn_stop_->setIcon(QIcon(":/icons/resources/imgs/light/delete_64px.png"));
        btn_forward_->setIcon(QIcon(":/icons/resources/imgs/light/right_64px.png"));
        btn_home_->setIcon(QIcon(":/icons/resources/imgs/light/home_64px.png"));
        btn_favorites_->setIcon(QIcon(":/icons/resources/imgs/light/favorite_window_64px.png"));
        btn_history_->setIcon(QIcon(":/icons/resources/imgs/light/time_machine_64px.png"));
        btn_download_->setIcon(QIcon(":/icons/resources/imgs/light/download_64px.png"));
        btn_capture_->setIcon(QIcon(":/icons/resources/imgs/light/screenshot_64px.png"));
        btn_user_->setIcon(QIcon(":/icons/resources/imgs/light/user_64px.png"));
        btn_more_options_->setIcon(QIcon(":/icons/resources/imgs/light/squared_menu_48px.png"));
#else
        btn_back_->setIcon(QIcon(":/icons/resources/imgs/ios7/left_50px.png"));
        btn_refresh_->setIcon(QIcon(":/icons/resources/imgs/ios7/refresh_50px.png"));
        btn_stop_->setIcon(QIcon(":/icons/resources/imgs/ios7/delete_50px.png"));
        btn_forward_->setIcon(QIcon(":/icons/resources/imgs/ios7/right_50px.png"));
        btn_home_->setIcon(QIcon(":/icons/resources/imgs/ios7/home_50px.png"));
        btn_favorites_->setIcon(QIcon(":/icons/resources/imgs/ios7/favorite_window_50px.png"));
        btn_history_->setIcon(QIcon(":/icons/resources/imgs/ios7/time_machine_50px.png"));
        btn_download_->setIcon(QIcon(":/icons/resources/imgs/ios7/download_50px.png"));
        btn_capture_->setIcon(QIcon(":/icons/resources/imgs/ios7/capture_50px.png"));
        btn_user_->setIcon(QIcon(":/icons/resources/imgs/ios7/account_50px.png"));
        btn_more_options_->setIcon(QIcon(":/icons/resources/imgs/ios7/circled_menu_50px.png"));
#endif

    }
    action_new_tab_->setIcon(QIcon(":/icons/resources/imgs/layout_64px.png"));
    action_new_window_->setIcon(QIcon(":/icons/resources/imgs/stack_64px.png"));
    action_new_inprivate_window_->setIcon(QIcon(":/icons/resources/imgs/frame_64px.png"));
    btn_zoom_out_->setIcon(QIcon(":/icons/resources/imgs/zoom_out_64px.png"));
    btn_zoom_in_->setIcon(QIcon(":/icons/resources/imgs/zoom_in_64px.png"));
    btn_fullscreen_->setIcon(QIcon(":/icons/resources/imgs/arrows_diagonals_64px.png"));
    action_favorates_->setIcon(QIcon(":/icons/resources/imgs/heart_64px.png"));
    action_history_->setIcon(QIcon(":/icons/resources/imgs/time_history_64px.png"));
    action_download_->setIcon(QIcon(":/icons/resources/imgs/download_64px.png"));
    action_print_->setIcon(QIcon(":/icons/resources/imgs/printer_64px.png"));
    action_capture_->setIcon(QIcon(":/icons/resources/imgs/cut_coupon_64px.png"));
    action_find_->setIcon(QIcon(":/icons/resources/imgs/search_64px.png"));
    action_more_tools_->setIcon(QIcon());
    action_task_mgr_->setIcon(QIcon(":/icons/resources/imgs/statistics_64px.png"));
    action_dev_tools_->setIcon(QIcon(":/icons/resources/imgs/hammer_64px.png"));
    action_settings_->setIcon(QIcon(":/icons/resources/imgs/settings_64px.png"));
    action_helps_->setIcon(QIcon(":/icons/resources/imgs/question_64px.png"));
    action_help_you_->setIcon(QIcon(":/icons/resources/imgs/question_64px.png"));
    action_feed_back_->setIcon(QIcon(":/icons/resources/imgs/bug_64px.png"));
    action_like_->setIcon(QIcon(":/icons/resources/imgs/good_64px.png"));
    action_about_qt_->setIcon(QIcon(":/icons/resources/imgs/qt_64px.png"));
    action_about_cef_->setIcon(QIcon(":/icons/resources/imgs/cef.png"));
    action_quit_->setIcon(QIcon());

    QSize iconSize(18,18);
    for(auto item : this->children()){
        if(item->isWidgetType() &&
                item->metaObject()->className() == QString("QToolButton"))
        {
            auto btn = qobject_cast<QToolButton*>(item);
            btn->setIconSize(iconSize);
        }
    }
//    btn_inprivate_->setIconSize(iconSize);

//    btn_back_->setIconSize(QSize(26,26));
//    btn_forward_->setIconSize(QSize(26,26));
}
