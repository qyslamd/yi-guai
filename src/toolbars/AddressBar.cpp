#include "AddressBar.h"

#include <QFocusEvent>
#include <QBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QToolButton>
#include <QGraphicsDropShadowEffect>
#include <QStringListModel>
#include <QCompleter>
#include <QTimer>

#include "utils/util_qt.h"
#include "browser/CefManager.h"
#include "managers/BookmarkManager.h"

AddressBar::AddressBar(bool inprivate, QWidget *parent)
    : QFrame(parent)
    , inprivate_(inprivate)
{
    initUi();
}

bool AddressBar::eventFilter(QObject *obj, QEvent *ev)
{
    if(obj == line_edit_addr_){
        auto type = ev->type();
        if(type == QEvent::FocusIn){
            auto focusEvent = static_cast<QFocusEvent *>(ev);
            if(focusEvent->reason() == Qt::MouseFocusReason){
                QTimer::singleShot(0, line_edit_addr_, &QLineEdit::selectAll);
            }
            line_edit_addr_->setPlaceholderText("");
            update();
        }else if(type == QEvent::FocusOut){
            line_edit_addr_->setPlaceholderText(tr("search or input a web address"));
            update();
        }
    }
    return QFrame::eventFilter(obj, ev);
}

QRect AddressBar::gGeometryBtnSiteInfo() const
{
    auto pos = mapToGlobal(btn_site_info_->pos());
    return QRect(pos.x(),
                 pos.y(),
                 btn_site_info_->width(),
                 btn_site_info_->height());
}

QRect AddressBar::gGeometryBtnAddBkmk() const
{
    auto pos = mapToGlobal(btn_add_favorite_->pos());
    return QRect(pos.x(),
                 pos.y(),
                 btn_add_favorite_->width(),
                 btn_add_favorite_->height());
}

QRect AddressBar::gGeometryBtnZoom() const
{
    auto pos = mapToGlobal(btn_zoom_hint_->pos());
    return QRect(pos.x(),
                 pos.y(),
                 btn_zoom_hint_->width(),
                 btn_zoom_hint_->height());
}

QPoint AddressBar::findButtonGlobalPos() const
{
    if(btn_find_hint_){
        return btn_find_hint_->mapToGlobal(btn_find_hint_->rect().bottomRight());
    }
    return QPoint();
}

void AddressBar::initUi()
{
    using ShaowEffect = QGraphicsDropShadowEffect;
    ShaowEffect *shadow = new ShaowEffect(this);
    shadow->setOffset(0, 0);
    shadow->setColor(Qt::gray);
    shadow->setBlurRadius(6);
    setGraphicsEffect(shadow);

    layout_ = new QHBoxLayout;
    layout_->setContentsMargins(2, 1, 2, 2);
    layout_->setSpacing(1);
    btn_site_info_ = new QPushButton;
    btn_site_info_->setToolTip(tr("view website infomation"));
    connect(btn_site_info_, &QPushButton::clicked, this, &AddressBar::viewSiteInfo);

    line_edit_addr_ = new QLineEdit;
    line_edit_addr_->installEventFilter(this);
    line_edit_addr_->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    connect(line_edit_addr_, &QLineEdit::returnPressed, this, &AddressBar::returnPressed);
    connect(line_edit_addr_, &QLineEdit::editingFinished,this, &AddressBar::onEditingFinished);
    connect(line_edit_addr_, &QLineEdit::textEdited, this, &AddressBar::onTextEdited);
    connect(line_edit_addr_, &QLineEdit::textEdited, this, &AddressBar::textEdited);

    btn_zoom_hint_ = new QToolButton;
    connect(btn_zoom_hint_, &QToolButton::clicked, this, &AddressBar::showZoomBar);
    btn_add_favorite_ = new QToolButton;
    btn_add_favorite_->setCheckable(true);
    btn_add_favorite_->setToolTip(tr("mark to favorite"));
    connect(btn_add_favorite_, &QToolButton::clicked, this, &AddressBar::addFavorite);
    connect(btn_add_favorite_, &QToolButton::toggled, this, [this](bool checked)
    {
        if(checked){
            btn_add_favorite_->setIcon(QIcon(":/icons/resources/imgs/colorful/star_filled_64px.png"));
        }else{
            if(!inprivate_){
                btn_add_favorite_->setIcon(QIcon(":/icons/resources/imgs/light/star_64px.png"));
            }else{
                btn_add_favorite_->setIcon(QIcon(":/icons/resources/imgs/dark/star_64px.png"));
            }
        }
    });

    layout_->addWidget(btn_site_info_);
    layout_->addWidget(line_edit_addr_);
    layout_->addWidget(btn_zoom_hint_);
    layout_->addWidget(btn_add_favorite_);
    setLayout(layout_);

    setFocusProxy(line_edit_addr_);

    completer_ = new QCompleter(QStringList(), this);
    line_edit_addr_->setCompleter(completer_);

    if(!inprivate_){
        btn_site_info_->setIcon(QIcon(":/icons/resources/imgs/alert_circle_64px.png"));
        btn_add_favorite_->setIcon(QIcon(":/icons/resources/imgs/light/star_64px.png"));
    }else{
        btn_site_info_->setIcon(QIcon(":/icons/resources/imgs/info_white_48px.png"));
        btn_add_favorite_->setIcon(QIcon(":/icons/resources/imgs/star_white_48px.png"));
    }
}

void AddressBar::setZoomLevelValue(double value)
{
    btn_zoom_hint_->setVisible(value != 0.0);

    if(value >= 0.0){
        if(inprivate_){
            btn_zoom_hint_->setIcon(QIcon(":/icons/resources/imgs/zoom_in_white_48px.png"));
        }else{
            btn_zoom_hint_->setIcon(QIcon(":/icons/resources/imgs/zoom_in2_64px.png"));
        }
    }else{
        if(inprivate_){
            btn_zoom_hint_->setIcon(QIcon(":/icons/resources/imgs/zoom_out_white_48px.png"));
        }else{
            btn_zoom_hint_->setIcon(QIcon(":/icons/resources/imgs/zoom_out2_64px.png"));
        }
    }
    auto zoomLevel = CefManager::Instance().zoom_map.value(static_cast<int>(value));
    btn_zoom_hint_->setToolTip(tr("zoomlevel:%1").arg(zoomLevel));
}

void AddressBar::setText(const QString &text, bool edited)
{
    line_edit_addr_->setText(text);
    btn_site_info_->setEnabled(!edited);
    btn_add_favorite_->setVisible(!edited);
    if(!edited){
        if(!inprivate_){
            btn_site_info_->setIcon(QIcon(":/icons/resources/imgs/alert_circle_64px.png"));
        }else{
            btn_site_info_->setIcon(QIcon(":/icons/resources/imgs/info_white_48px.png"));
        }
    }    
}

void AddressBar::setCursorPosition(int pos)
{
    line_edit_addr_->setCursorPosition(pos);
}

QString AddressBar::text()
{
    return line_edit_addr_->text();
}

void AddressBar::onEditingFinished()
{
}

void AddressBar::onTextEdited(const QString &)
{
    btn_site_info_->setEnabled(false);
    btn_add_favorite_->setVisible(false);
    bool isUrl =  false;
    QIcon icon;
    if(inprivate_){
        icon = isUrl ? QIcon(":/icons/resources/imgs/dark/globe_earth_64px.png")
                     : QIcon(":/icons/resources/imgs/dark/search_64px.png");
    }else{
        icon = isUrl ? QIcon(":/icons/resources/imgs/light/globe_earth_64px.png")
                     : QIcon(":/icons/resources/imgs/light/search_64px.png");
    }
    btn_site_info_->setIcon(icon);
}
