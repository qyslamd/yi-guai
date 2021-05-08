#include "addressbar.h"
#include "utils/util_qt.h"

#include <QHBoxLayout>
#include <QToolButton>
#include <QAction>
#include <QtDebug>
#include <QEvent>
#include <QPainter>
#include <QPainterPath>
#include <QCursor>
#include <QGraphicsDropShadowEffect>

AddressBar::AddressBar(QWidget *parent)
    : QLineEdit(parent)
    , btn_site_info_(new QAction)
    , btn_find_hint_(new QAction)
    , btn_zoom_hint_(new QAction)
    , btn_mark_site_(new QAction)
{   
    initUi();
    setAppearance();
    fuckButton();
}

bool AddressBar::eventFilter(QObject *obj, QEvent *ev)
{
    if(obj == internal_btn_siteInfo_){
        auto w = internal_btn_siteInfo_;
        if(ev->type() == QEvent::Paint){
            auto cursorPos = QCursor::pos();
            auto pos = mapFromGlobal(cursorPos);
            if( w->geometry().contains(pos)){
                QPainter p( w);
                p.save();
                p.setRenderHint(QPainter::Antialiasing);
                QPainterPath path;
                path.addRoundedRect(QRectF(0,0,w->width(), w->height()),4,4);
                p.fillPath(path,QColor(220,220,220));
                p.restore();
            }
        }
    }

    return QLineEdit::eventFilter(obj, ev);
}

QRect AddressBar::gGeometryBtnSiteInfo() const
{
    if(internal_btn_siteInfo_){
        auto pos = mapToGlobal(internal_btn_siteInfo_->pos());
        return QRect(pos.x(),
                     pos.y(),
                     internal_btn_siteInfo_->width(),
                     internal_btn_siteInfo_->height());
    }
    return QRect();
}

void AddressBar::initUi()
{
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    shadow->setOffset(0, 0);
    shadow->setColor(QColor(38, 78, 119, 127));
    shadow->setBlurRadius(3);
    setGraphicsEffect(shadow);

    btn_site_info_->setToolTip(tr("view website infomation"));
    addAction(btn_site_info_, QLineEdit::LeadingPosition);
    addAction(btn_mark_site_, QLineEdit::TrailingPosition);

//    addAction(btn_find_hint_, QLineEdit::TrailingPosition);
//    addAction(btn_zoom_hint_, QLineEdit::TrailingPosition);

    connect(btn_site_info_, &QAction::triggered, this, &AddressBar::viewSiteInfo);
}

void AddressBar::setAppearance()
{
    setMinimumHeight(30);
    btn_site_info_->setIcon(QIcon(":/icons/resources/site_safe_26px.png"));
    btn_mark_site_->setIcon(QIcon(":/icons/resources/normal_mark.png"));
}

void AddressBar::fuckButton()
{
    foreach(auto w , btn_site_info_->associatedWidgets()){
        if(w->metaObject()->superClass()->className() == QString("QToolButton"))
        {
            internal_btn_siteInfo_ = qobject_cast<QToolButton *>(w);
            if(internal_btn_siteInfo_){
                internal_btn_siteInfo_->installEventFilter(this);
            }
        }
    }
}
