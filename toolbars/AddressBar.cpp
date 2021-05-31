#include "AddressBar.h"

#include <QCompleter>
#include <QHBoxLayout>
#include <QToolButton>
#include <QAction>
#include <QtDebug>
#include <QEvent>
#include <QPainter>
#include <QPainterPath>
#include <QCursor>
#include <QMouseEvent>
#include <QTimer>
#include <QGraphicsDropShadowEffect>
#include <QStringListModel>

#include "utils/util_qt.h"
#include "managers/AddrInputManager.h"

AddressBar::AddressBar(QWidget *parent)
    : QLineEdit(parent)
    , completer_(new QCompleter(QStringList(), this))
    , model_(new QStringListModel(this))
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

bool AddressBar::event(QEvent *ev)
{
    if(ev->type()== QEvent::FocusIn)
    {
        auto focusEvent = static_cast<QFocusEvent *>(ev);
        if(focusEvent->reason() == Qt::MouseFocusReason){
            QTimer::singleShot(0, this, &QLineEdit::selectAll);
        }
    }
    return QLineEdit::event(ev);
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

void AddressBar::setInprivate(bool inprivate)
{
    if(!inprivate){
        btn_site_info_->setIcon(QIcon(":/icons/resources/imgs/alert_circle_64px.png"));
        btn_mark_site_->setIcon(QIcon(":/icons/resources/imgs/star_64px.png"));
    }else{
        btn_site_info_->setIcon(QIcon(":/icons/resources/imgs/info_white_48px.png"));
        btn_mark_site_->setIcon(QIcon(":/icons/resources/imgs/star_white_48px.png"));
    }
}

void AddressBar::mousePressEvent(QMouseEvent *event)
{
    QLineEdit::mousePressEvent(event);
}

void AddressBar::initUi()
{
    setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    shadow->setOffset(0, 0);
    shadow->setColor(QColor(38, 78, 119, 127));
    shadow->setBlurRadius(3);
    setGraphicsEffect(shadow);

    completer_->setModel(model_);
    model_->setStringList(AddrInputMgr::Instance().inputList());
    setCompleter(completer_);
    connect(this, &QLineEdit::editingFinished, this, &AddressBar::onEditingFinishsed);

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
    btn_site_info_->setIcon(QIcon(":/icons/resources/imgs/alert_circle_64px.png"));
    btn_mark_site_->setIcon(QIcon(":/icons/resources/imgs/star_64px.png"));
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

void AddressBar::onEditingFinishsed()
{
    auto text = this->text();
    if(QString::compare(text, QString("")) != 0)
    {
        const auto list = AddrInputMgr::Instance().inputList();
        if(!list.contains(text, Qt::CaseInsensitive))
        {
            AddrInputMgr::Instance().addRecord(text);
            model_->setStringList(AddrInputMgr::Instance().inputList());
        }
    }
}
