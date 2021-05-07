#include "navibar.h"
#include "addressbar.h"
#include "utils/util_qt.h"

#include <QHBoxLayout>
#include <QToolButton>
#include <QPainter>
#include <QMenu>
#include <QWidgetAction>
#include <QShowEvent>

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
    , menu_more_options_(new QMenu)
    , action_new_tab_(new QAction)
    , action_new_window_(new QAction)
    , action_new_inprivate_window_(new QAction)
    , action_zoom_(new QWidgetAction(this))
    , action_favorates_(new QAction)
    , action_history_(new QAction)
    , action_download_(new QAction)
    , action_print_(new QAction)
    , action_capture_(new QAction)
    , action_find_(new QAction)
    , action_more_tools_(new QAction)
    , action_settings_(new QAction)
    , action_help_(new QAction)
    , action_quit_(new QAction)

{
    layout_->addWidget(btn_back_);
    layout_->addWidget(btn_forward_);
    layout_->addWidget(btn_refresh_);
    layout_->addWidget(btn_stop_);
    layout_->addWidget(btn_home_);
    layout_->addWidget(address_bar_);
    layout_->addWidget(frame_extensions_);
    layout_->addWidget(frame_tools_);
    layout_->addWidget(btn_user_);
    layout_->addWidget(btn_more_options_);

    setLayout(layout_);
    layout_->setContentsMargins(4,4,4,5);

    action_new_tab_->setText(tr("create new tab page"));
    action_new_tab_->setShortcut(QKeySequence("Ctrl+T"));
    action_new_tab_->setIcon(QIcon());
    action_new_window_->setText(tr("create new window"));
    action_new_window_->setShortcut(QKeySequence("Ctrl+N"));
    action_new_window_->setIcon(QIcon());
    action_new_inprivate_window_->setText(tr("create new inprivate window"));
    action_new_inprivate_window_->setIcon(QIcon());
    action_favorates_->setText(tr("favorates"));
    action_favorates_->setIcon(QIcon());
    action_history_->setText(tr("histories"));
    action_history_->setIcon(QIcon());
    action_download_->setText(tr("downloads"));
    action_download_->setIcon(QIcon());
    action_print_->setText(tr("print"));
    action_print_->setShortcut(QKeySequence("Ctrl+P"));
    action_print_->setIcon(QIcon());
    action_capture_->setText(tr("capture"));
    action_capture_->setIcon(QIcon());
    action_find_->setText(tr("find in this page"));
    action_find_->setIcon(QIcon());
    action_more_tools_->setText(tr("more tools"));
    action_more_tools_->setIcon(QIcon());
    action_settings_->setText(tr("settings"));
    action_settings_->setIcon(QIcon());
    action_help_->setText(tr("help and feedback"));
    action_help_->setIcon(QIcon());
    action_quit_->setText(tr("quit app"));
    action_quit_->setIcon(QIcon());

    menu_more_options_->installEventFilter(this);
    menu_more_options_->addAction(action_new_tab_);
    menu_more_options_->addAction(action_new_window_);
    menu_more_options_->addAction(action_new_inprivate_window_);
    menu_more_options_->addAction(action_zoom_);
    menu_more_options_->addAction(action_favorates_);
    menu_more_options_->addAction(action_history_);
    menu_more_options_->addAction(action_download_);
    menu_more_options_->addAction(action_print_);
    menu_more_options_->addAction(action_capture_);
    menu_more_options_->addAction(action_find_);
    menu_more_options_->addAction(action_more_tools_);
    menu_more_options_->addAction(action_settings_);
    menu_more_options_->addAction(action_help_);
    menu_more_options_->addAction(action_quit_);

    btn_more_options_->setObjectName("NaviBarOptionBtn");
    btn_more_options_->setMenu(menu_more_options_);
    btn_more_options_->setPopupMode(QToolButton::InstantPopup);

    initSignals();

    setAppearance();
}

void NaviBar::setSpacing(int spacing)
{
    if(spacing < 0){
       layout_->setSpacing(0);
    }else{
       layout_->setSpacing(spacing);
    }
}

void NaviBar::setAddress(const QString &url)
{
    address_bar_->setText(url);
}

void NaviBar::setLoadingState(bool isLoading, bool canGoBack, bool canGoForward)
{
    btn_back_->setEnabled(canGoBack);
    btn_forward_->setEnabled(canGoForward);
    btn_refresh_->setVisible(!isLoading);
    btn_stop_->setVisible(isLoading);
}

bool NaviBar::eventFilter(QObject *obj, QEvent *ev)
{
    if(menu_more_options_ == obj){
        if(ev->type() == QEvent::Show){
            auto pos = menu_more_options_->pos();
            pos.setX(pos.x() - menu_more_options_->width());
            pos.setX(pos.x() + btn_more_options_->width());
            menu_more_options_->move(pos);
        }
    }
    return QFrame::eventFilter(obj, ev);
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
    connect(btn_back_, &QToolButton::clicked, this, [this]()
    {
       emit naviBarCmd(NaviBarCmd::Back, QVariant());
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
    connect(action_new_tab_, &QAction::triggered, this, [this]()
    {
       emit naviBarCmd(NaviBarCmd::NewTabPage, QVariant());
    });
    connect(action_new_window_, &QAction::triggered, this, [this]()
    {
       emit naviBarCmd(NaviBarCmd::NewWindow, QVariant());
    });
    connect(action_new_inprivate_window_, &QAction::triggered, this, [this]()
    {
       emit naviBarCmd(NaviBarCmd::NewInprivateWindow, QVariant());
    });
}

void NaviBar::setAppearance()
{
    btn_back_->setIcon(QIcon(":/icons/resources/normal_back.png"));
    btn_refresh_->setIcon(QIcon(":/icons/resources/normal_refresh.png"));
    btn_stop_->setIcon(QIcon(":/icons/resources/window_close_light_2x.png"));
    btn_forward_->setIcon(QIcon(":/icons/resources/normal_forward.png"));
    btn_home_->setIcon(QIcon(":/icons/resources/home_blabk_64px.png"));
    btn_user_->setIcon(QIcon(":/icons/resources/user_64px.png"));
    btn_more_options_->setIcon(QIcon(":/icons/resources/normal_more.png"));

    QSize iconSize(26,26);
    for(auto item : this->children()){
        if(item->isWidgetType() &&
                item->metaObject()->className() == QString("QToolButton"))
        {
            auto btn = qobject_cast<QToolButton*>(item);
            btn->setIconSize(iconSize);
        }
    }
}
