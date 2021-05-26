#include "NavigateBar.h"
#include "AddressBar.h"
#include "utils/util_qt.h"

#include <QtDebug>
#include <QHBoxLayout>
#include <QToolButton>
#include <QLabel>
#include <QPainter>
#include <QMenu>
#include <QWidgetAction>
#include <QShowEvent>
#include <QStyle>

NaviBar::NaviBar(QWidget *parent)
    : QFrame(parent)
    , layout_(new QHBoxLayout)
    , btn_back_(new QToolButton)
    , btn_refresh_(new QToolButton)
    , btn_stop_(new QToolButton)
    , btn_forward_(new QToolButton)
    , btn_home_(new QToolButton)
    , btn_user_(new QToolButton)
    , btn_more_options_(new QToolButton)
    , address_bar_(new AddressBar)
    , frame_extensions_(new QFrame)
    , frame_tools_(new QFrame)
    , btn_bookmarks_(new QToolButton)
    , btn_history_(new QToolButton)
    , btn_download_(new QToolButton)
    , btn_capture_(new QToolButton)
    , menu_more_options_(new QMenu(this))
    , menu_more_tools_(new QMenu)
    , menu_help_(new QMenu)
    , action_new_tab_(new QAction)
    , action_new_window_(new QAction(this))
    , action_new_inprivate_window_(new QAction)
    , action_zoom_(new QWidgetAction(this))
    , frame_zoom_(new QFrame)
    , label_zoom_(new QLabel(tr("Zoom")))
    , label_zoom_value_(new QLabel)
    , btn_zoom_out_(new QToolButton)
    , btn_zoom_in_(new QToolButton)
    , btn_fullscreen_(new QToolButton)
    , action_favorates_(new QAction)
    , action_history_(new QAction)
    , action_download_(new QAction)
    , action_print_(new QAction)
    , action_capture_(new QAction)
    , action_find_(new QAction)
    , action_more_tools_(new QAction)
    , action_task_mgr_(new QAction)
    , action_dev_tools_(new QAction)
    , action_settings_(new QAction)
    , action_helps_(new QAction)
    , action_help_you_(new QAction)
    , action_feed_back_(new QAction)
    , action_like_(new QAction)
    , action_about_qt_(new QAction)
    , action_about_cef_(new QAction)
    , action_quit_(new QAction)

{
    btn_bookmarks_->setCheckable(true);
    btn_bookmarks_->setToolTip(tr("bookmarks"));
    btn_history_->setCheckable(true);
    btn_history_->setToolTip(tr("histories"));
    btn_download_->setCheckable(true);
    btn_download_->setToolTip(tr("downlaods"));
    btn_capture_->setCheckable(true);
    btn_capture_->setToolTip(tr("screen shot"));
    btn_user_->setCheckable(true);
    btn_user_->setToolTip(tr("show user information"));

    layout_->addWidget(btn_back_);
    layout_->addWidget(btn_forward_);
    layout_->addWidget(btn_refresh_);
    layout_->addWidget(btn_stop_);
    layout_->addWidget(btn_home_);
    layout_->addSpacerItem(new QSpacerItem(6,10,QSizePolicy::Fixed));
    layout_->addWidget(address_bar_);
    layout_->addSpacerItem(new QSpacerItem(6,10,QSizePolicy::Fixed));
    layout_->addWidget(frame_extensions_);
    layout_->addWidget(frame_tools_);
    layout_->addWidget(btn_bookmarks_);
    layout_->addWidget(btn_history_);
    layout_->addWidget(btn_download_);
    layout_->addWidget(btn_capture_);
    layout_->addWidget(btn_user_);
    layout_->addWidget(btn_more_options_);

    setLayout(layout_);
    layout_->setContentsMargins(4,4,4,5);
    layout_->setSpacing(0);

    action_new_tab_->setText(tr("create new tab page"));
    action_new_tab_->setShortcut(QKeySequence("Ctrl+T"));
    action_new_tab_->setIcon(QIcon(":/icons/resources/imgs/layout_64px.png"));
    action_new_window_->setText(tr("create new window"));
    action_new_window_->setShortcut(QKeySequence("Ctrl+N"));
    action_new_window_->setIcon(QIcon(":/icons/resources/imgs/stack_64px.png"));
    action_new_inprivate_window_->setText(tr("create new inprivate window"));
    action_new_inprivate_window_->setIcon(QIcon(":/icons/resources/imgs/frame_64px.png"));
    action_new_inprivate_window_->setShortcut(QKeySequence("Ctrl+Shift+N"));
    action_favorates_->setText(tr("favorates"));
    action_favorates_->setIcon(QIcon(":/icons/resources/imgs/heart_64px.png"));
    action_history_->setText(tr("histories"));
    action_history_->setIcon(QIcon(":/icons/resources/imgs/time_history_64px.png"));
    action_history_->setShortcut(QKeySequence("Ctrl+H"));
    action_download_->setText(tr("downloads"));
    action_download_->setIcon(QIcon(":/icons/resources/imgs/download_64px.png"));
    action_download_->setShortcut(QKeySequence("Ctrl+J"));
    action_print_->setText(tr("print"));
    action_print_->setShortcut(QKeySequence("Ctrl+P"));
    action_print_->setIcon(QIcon(":/icons/resources/imgs/printer_64px.png"));
    action_capture_->setText(tr("capture"));
    action_capture_->setIcon(QIcon(":/icons/resources/imgs/cut_coupon_64px.png"));
    action_find_->setText(tr("find in this page"));
    action_find_->setIcon(QIcon(":/icons/resources/imgs/search_64px.png"));
    action_find_->setShortcut(QKeySequence("Ctrl+F"));
    action_more_tools_->setText(tr("more tools"));
    action_more_tools_->setIcon(QIcon());
    action_task_mgr_->setText(tr("browser task manager"));
    action_task_mgr_->setIcon(QIcon(":/icons/resources/imgs/statistics_64px.png"));
    action_dev_tools_->setText(tr("developer tool"));
    action_dev_tools_->setIcon(QIcon(":/icons/resources/imgs/hammer_64px.png"));
    action_settings_->setText(tr("settings"));
    action_settings_->setIcon(QIcon(":/icons/resources/imgs/settings_64px.png"));
    action_helps_->setText(tr("help and feedback"));
    action_helps_->setIcon(QIcon(":/icons/resources/imgs/question_64px.png"));
    action_help_you_->setText(tr("help"));
    action_help_you_->setIcon(QIcon(":/icons/resources/imgs/question_64px.png"));
    action_feed_back_->setText(tr("send feed back"));
    action_feed_back_->setIcon(QIcon(":/icons/resources/imgs/bug_64px.png"));
    action_like_->setText(tr("give a like"));
    action_like_->setIcon(QIcon(":/icons/resources/imgs/good_64px.png"));
    action_about_qt_->setText(tr("about qt"));
    action_about_qt_->setIcon(QIcon(":/icons/resources/imgs/qt_64px.png"));
    action_about_cef_->setText(tr("about cef"));
    action_about_cef_->setIcon(QIcon(":/icons/resources/imgs/cef.png"));
    action_quit_->setText(tr("quit app"));
    action_quit_->setIcon(QIcon());

    frame_zoom_->setFrameShape(QFrame::NoFrame);
    frame_zoom_->setObjectName("ZoomBarFrame");
    btn_zoom_out_->setObjectName("ZoomOutToolButton");
    btn_zoom_out_->setIcon(QIcon(":/icons/resources/imgs/zoom_out_64px.png"));
    label_zoom_value_->setObjectName("ZoomValueLabel");
    label_zoom_value_->setAlignment(Qt::AlignCenter);
    label_zoom_value_->setText("100%");
    label_zoom_value_->setMaximumWidth(42);
    btn_zoom_in_->setObjectName("ZoomInToolButton");
    btn_zoom_in_->setIcon(QIcon(":/icons/resources/imgs/zoom_in_64px.png"));
    btn_fullscreen_->setObjectName("ZoomBarFullscrnButton");
    btn_fullscreen_->setIcon(QIcon(":/icons/resources/imgs/arrows_diagonals_64px.png"));

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
    frame_zoom_->setLayout(zoom_layout);
    action_zoom_->setDefaultWidget(frame_zoom_);

    menu_more_options_->installEventFilter(this);
    menu_more_options_->addAction(action_new_tab_);
    menu_more_options_->addAction(action_new_window_);
    menu_more_options_->addAction(action_new_inprivate_window_);
    menu_more_options_->addAction(action_zoom_);
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

    initSignals();
    setAppearance();
}

void NaviBar::setAddress(const QString &url)
{
    address_bar_->setText(url);
    address_bar_->setCursorPosition(0);
}

void NaviBar::setLoadingState(bool isLoading, bool canGoBack, bool canGoForward)
{
    btn_back_->setEnabled(canGoBack);
    btn_forward_->setEnabled(canGoForward);
    btn_refresh_->setVisible(!isLoading);
    btn_stop_->setVisible(isLoading);
}

void NaviBar::setFocus(bool focus)
{
    if(focus){
        address_bar_->clearFocus();
    }
}

bool NaviBar::eventFilter(QObject *obj, QEvent *ev)
{
    if(menu_more_options_ == obj){
        if(ev->type() == QEvent::Show){
            auto pos = mapToGlobal(btn_more_options_->pos());

            pos.setX(pos.x() - menu_more_options_->width());
            pos.setX(pos.x() + btn_more_options_->width());
            pos.setY(pos.y() + btn_more_options_->height());
            menu_more_options_->move(pos);
        }
    }
    return QFrame::eventFilter(obj, ev);
}

void NaviBar::onHistoryPopupVisibleChange(bool visible)
{
    btn_history_->setChecked(visible);
}

void NaviBar::onUserInfoPopupVisibleChange(bool visible)
{
    btn_user_->setChecked(visible);
}

void NaviBar::paintEvent(QPaintEvent *event)
{
    QFrame::paintEvent(event);

    QPainter p(this);
    p.save();
    qreal penWidth = 0.5f;
    QColor color((QLatin1String("#D2D2D2")));
    p.setPen(QPen(QBrush(color), 0.5f));
    p.drawLine(0.0, height() - penWidth, width(), height() - penWidth);
    p.restore();
}

void NaviBar::initSignals()
{
    connect(address_bar_, &AddressBar::returnPressed, this, [this]()
    {
        emit naviBarCmd(NaviBarCmd::Navigate, address_bar_->text());
    });
    connect(address_bar_, &AddressBar::viewSiteInfo, this, [this]()
    {
        auto rect = address_bar_->gGeometryBtnSiteInfo();
        emit naviBarCmd(NaviBarCmd::ViewSiteInfo, rect);
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
    connect(btn_bookmarks_, &QToolButton::clicked, this, [this]()
    {
        auto pos = mapToGlobal(btn_bookmarks_->geometry().bottomRight());
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
        emit naviBarCmd(NaviBarCmd::History, QVariant());
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

void NaviBar::setAppearance()
{
#if 0
    btn_back_->setIcon(QIcon(":/icons/resources/imgs/arrow_left2_64px.png"));
    btn_refresh_->setIcon(QIcon(":/icons/resources/imgs/reload_64px.png"));
    btn_stop_->setIcon(QIcon(":/icons/resources/imgs/CLOSE3_64px.png"));
    btn_forward_->setIcon(QIcon(":/icons/resources/imgs/arrow_right2_64px.png"));
    btn_home_->setIcon(QIcon(":/icons/resources/imgs/home_64px.png"));
    btn_history_->setIcon(QIcon(":/icons/resources/imgs/time_history_64px.png"));
    btn_user_->setIcon(QIcon(":/icons/resources/imgs/user_64px.png"));
    btn_more_options_->setIcon(QIcon(":/icons/resources/imgs/more_horizontal_64px.png"));
#else
    btn_back_->setIcon(QIcon(":/icons/resources/newIcons/back_64px_1128270_easyicon.net.png"));
    btn_refresh_->setIcon(QIcon(":/icons/resources/newIcons/Refresh_64px.png"));
    btn_stop_->setIcon(QIcon(":/icons/resources/newIcons/delete_64px_1128279_easyicon.net.png"));
    btn_forward_->setIcon(QIcon(":/icons/resources/newIcons/forward_64px_1128288_easyicon.net.png"));
    btn_home_->setIcon(QIcon(":/icons/resources/newIcons/home_80px.png"));
    btn_bookmarks_->setIcon(QIcon(":/icons/resources/newIcons/mark_as_favorite_128px.png"));
    btn_history_->setIcon(QIcon(":/icons/resources/newIcons/history2.png"));
    btn_download_->setIcon(QIcon(":/icons/resources/newIcons/downloads_48px.png"));
    btn_capture_->setIcon(QIcon(":/icons/resources/newIcons/screenshot_64px.png"));
    btn_user_->setIcon(QIcon(":/icons/resources/newIcons/male_64px.png"));
    btn_more_options_->setIcon(QIcon(":/icons/resources/newIcons/more_260.95774647887px_1201158_easyicon.net.png"));
#endif
    QSize iconSize(24,24);
    QSize btnSize(42,30);
    for(auto item : this->children()){
        if(item->isWidgetType() &&
                item->metaObject()->className() == QString("QToolButton"))
        {
            auto btn = qobject_cast<QToolButton*>(item);
            btn->setMinimumSize(btnSize);
            btn->setIconSize(iconSize);
        }
    }
}
