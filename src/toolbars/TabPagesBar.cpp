#include "TabPagesBar.h"

#include <QtDebug>
#include <QHBoxLayout>
#include <QToolButton>
#include <QPainter>
#include <QMouseEvent>
#include <QHelpEvent>
#include <QToolTip>

#include "TabBar.h"
#include "utils/util_qt.h"

#ifdef Q_OS_WIN
#include <QtWin>
#endif

TabPagesBar::TabPagesBar(bool inprivate, QWidget *parent)
    : CaptionFrame(parent)
    , inprivate_(inprivate)
    , layout_(new QHBoxLayout)
    , btn_dock_tabs_(new QToolButton)
    , tab_bar_(new TabBar(inprivate))
    , btn_add_page_(new QToolButton)
{
    initUi();
}

TabPagesBar::~TabPagesBar()
{

}

int TabPagesBar::insertTab(int index, const QString &text)
{
    return tab_bar_->insertTab(index, text);
}

int TabPagesBar::count() const
{
    return tab_bar_->count();
}

int TabPagesBar::currentIndex() const
{
    return tab_bar_->currentIndex();
}

void TabPagesBar::moveTab(int from, int to)
{
    tab_bar_->moveTab(from, to);
}

void TabPagesBar::removeTab(int index)
{
    tab_bar_->removeTab(index);
}

void TabPagesBar::setTabIcon(int index, const QIcon &icon)
{
    tab_bar_->setTabIcon(index, icon);
}

void TabPagesBar::setTabText(int index, const QString &text)
{
    tab_bar_->setTabText(index, text);
}

QString TabPagesBar::tabText(int index) const
{
    return tab_bar_->tabText(index);
}

bool TabPagesBar::hitTestCaption(const QPoint &gPos)
{
    // 映射全局坐标当前区域中
    auto pos = mapFromGlobal(gPos);
    // 如果映射过来的坐标不在当前范围内，不能处理 HITCAPTION
    if(!this->rect().contains(pos))
    {
        return false;
    }
    // 如果在，且没有子窗口且不在绘制的按钮范围内，就是 HITCAPTION
    return !childAt(pos) && !windowBtnRect().contains(pos);
}

void TabPagesBar::setCurrentIndex(int index)
{
    tab_bar_->setCurrentIndex(index);
}

void TabPagesBar::onDwmColorChanged()
{
#ifdef Q_OS_WIN

#endif
}

void TabPagesBar::paintEvent(QPaintEvent *event)
{
    CaptionFrame::paintEvent(event);
}

void TabPagesBar::initUi()
{
    QColor activeColor(0x609DBF), inActiveColor = activeColor;    // CECECE E8E8E8
    inActiveColor.setAlphaF(0.7);
    if(inprivate_){
        activeColor = "#2E2F30";
        inActiveColor = activeColor;
        inActiveColor.setAlphaF(0.7);
    }else{
        if(UtilQt::dwmColorPrevalence()){
            activeColor = QtWin::realColorizationColor();
            activeColor.setAlphaF(1);
            inActiveColor = activeColor;
            inActiveColor.setAlphaF(0.7);
        }
    }
    setStyleSheet(QString(
                ".TabPagesBar{"
                "background-color:%1;"
                "}"
                ".TabPagesBar:!active{"
                "background-color:%2"
                "}")
                  .arg(activeColor.name(QColor::HexArgb))
                  .arg(inActiveColor.name(QColor::HexArgb))
                  );

    setFocusPolicy(Qt::TabFocus);
    setFocusProxy(tab_bar_);

    setLayout(layout_);
    layout_->setContentsMargins(6, 8, 0, 0);
    layout_->setSpacing(2);
    layout_->addSpacerItem(new QSpacerItem(6,10,QSizePolicy::Fixed));
    layout_->addWidget(btn_dock_tabs_);
    layout_->addWidget(tab_bar_);
    layout_->addWidget(btn_add_page_);
    layout_->addStretch();
    layout_->addSpacerItem(new QSpacerItem(rightReserved,10,QSizePolicy::Fixed));

    btn_dock_tabs_->setToolTip(tr("open vertical tabs"));
    btn_add_page_->setToolTip(tr("Add a tab page"));

    QSize iconSize(20,20);
    btn_dock_tabs_->setIconSize(iconSize);
    btn_add_page_->setIconSize(iconSize);
    btn_dock_tabs_->setIcon(QIcon(":/icons/resources/imgs/normal_pagelist_hide.png"));
    btn_add_page_->setIcon(QIcon(":/icons/resources/imgs/plus_48px.png"));

    connect(tab_bar_, &TabBar::currentChanged, this, &TabPagesBar::currentChanged);
    connect(tab_bar_, &TabBar::tabCloseRequested, this, &TabPagesBar::tabCloseRequested);
    connect(tab_bar_, &TabBar::tabMoved, this, &TabPagesBar::tabMoved);
    connect(tab_bar_, &TabBar::showPreview, this, &TabPagesBar::showPreview);
    connect(tab_bar_, &TabBar::menuTriggered, this, &TabPagesBar::tabbarMenuTriggered);

    connect(btn_add_page_, &QToolButton::clicked, this, &TabPagesBar::addPage);
    connect(btn_dock_tabs_, &QToolButton::clicked, this, &TabPagesBar::showDockPage);
    btn_dock_tabs_->hide();
}

CaptionFrame::CaptionFrame(QWidget *parent)
    : QFrame(parent)
{
    setMouseTracking(true);
}

int CaptionFrame::reservedWidth() const
{
    return 3 * BtnWidth+ 2 * BtnSpacing;
}

QRect CaptionFrame::windowBtnRect()
{
    auto rect1 = btnRect(CaptionButtons::Button_Mini);
    auto rect3 = btnRect(CaptionButtons::Button_Close);

    return QRect(rect1.topLeft().toPoint(),
                 rect3.bottomRight().toPoint());
}

bool CaptionFrame::event(QEvent *ev)
{
    if(ev->type() == QEvent::ToolTip){
        QHelpEvent *helpEvent = static_cast<QHelpEvent *>(ev);
        auto button = buttonAt(helpEvent->pos());
        if (button != CaptionButtons::Button_None) {
            QString str;
            switch (button) {
            case CaptionButtons::Button_Mini:
                str = tr("minimize");
                break;
            case CaptionButtons::Button_NormalMax:

                str = this->window()->isMaximized()
                        ? tr("restore")
                        : tr("maximize");
                break;
            case CaptionButtons::Button_Close:
                str = tr("close");
                break;
            default:
                Q_UNREACHABLE();
            }
            QToolTip::showText(helpEvent->globalPos(), str);
        } else {
            QToolTip::hideText();
            ev->ignore();
        }

        return true;
    }

    return QFrame::event(ev);
}

void CaptionFrame::paintEvent(QPaintEvent *event)
{
    QFrame::paintEvent(event);
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);
    drawButtons(&p);
}

void CaptionFrame::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        if(btnRect(CaptionButtons::Button_Mini).contains(event->pos()))
        {
            min_button_press_ = true;
        }else if(btnRect(CaptionButtons::Button_NormalMax).contains(event->pos()))
        {
            max_buttton_press_ = true;
        }else if(btnRect(CaptionButtons::Button_Close).contains(event->pos())){
            close_button_press_ = true;
        }else{
            return QFrame::mousePressEvent(event);
        }
    }
    QFrame::mousePressEvent(event);
}

void CaptionFrame::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        if(btnRect(CaptionButtons::Button_Mini).contains(event->pos())
                && min_button_press_)
        {
            emit minBtnClicked();
            min_button_press_ = false;
        }else if(btnRect(CaptionButtons::Button_NormalMax).contains(event->pos())
                 && max_buttton_press_)
        {
            emit normalMaxBtnClicked();
            max_buttton_press_ = false;
        }else if(btnRect(CaptionButtons::Button_Close).contains(event->pos())
                 && close_button_press_){
            emit closeBtnClicked();
            close_button_press_ = false;
        }else{
            return QFrame::mouseReleaseEvent(event);
        }
    }
    QFrame::mouseReleaseEvent(event);
}

void CaptionFrame::mouseMoveEvent(QMouseEvent *event)
{
    clearButtonHover();
    auto pos = event->pos();
    if(btnRect(CaptionButtons::Button_Mini).contains(pos)){
        min_button_hover_ = true;
        update();
    }else if(btnRect(CaptionButtons::Button_NormalMax).contains(pos))
    {
        max_button_hover_ = true;
        update();
    }else if(btnRect(CaptionButtons::Button_Close).contains(pos))
    {
        close_button_hover_ = true;
        update();
    }
    QFrame::mouseMoveEvent(event);
}

void CaptionFrame::leaveEvent(QEvent *event)
{
    clearButtonHover();
    QFrame::leaveEvent(event);
}

QRectF CaptionFrame::btnRect(CaptionFrame::CaptionButtons button)
{
    QRect captionRect{0, 1, width(), height()};
    int y = captionRect.y();
    QRectF rectClose(captionRect.x() + captionRect.width() - BtnWidth,
                     y,
                     BtnWidth,
                     BtnHeight);
    QRectF rectMax(rectClose.x() - BtnSpacing - BtnWidth,
                   y,
                   BtnWidth,
                   BtnHeight);
    QRectF rectMini(rectMax.x() - BtnSpacing - BtnWidth,
                    y,
                    BtnWidth,
                    BtnHeight);

    switch (button) {
    case CaptionButtons::Button_Close:
        return rectClose;
        case CaptionButtons::Button_NormalMax:
        return rectMax;
    case CaptionButtons::Button_Mini:
        return rectMini;
    default:
        break;
    }
    return QRect();

}

QPixmap CaptionFrame::btnPixmap(CaptionFrame::CaptionButtons button, const QColor &color)
{
    QSize size(10, 10);
    QPixmap pix(size);
    pix.fill(Qt::transparent);
    QSizeF sizeF(size.width() *1.0,  size.height() * 1.0);
    QPainter p(&pix);
    switch (button) {
    case CaptionButtons::Button_Mini:
    {
        p.save();
        QPen pen(QBrush(color), 1.0);
        p.setPen(pen);

        p.drawLine(QPointF(0, sizeF.height() / 2),
                   QPointF(sizeF.width(), sizeF.height() / 2));

        p.restore();
    }
        break;
    case CaptionButtons::Button_NormalMax:
    {
        // max
        if(!window()->isMaximized()){
            p.save();
            QPen pen(QBrush(color), 1.0);
            p.setPen(pen);

            p.drawRect(QRectF(0.0, 0.0, sizeF.width() - 1.0, sizeF.height() - 1.0));

            p.restore();
        }else
            // restore
        {
            p.save();
            p.setRenderHint(QPainter::Antialiasing);
            QPen pen(QBrush(color), 1.0);
            p.setPen(pen);

            QRectF rect(0,0,size.width(), size.height());
            QRectF r1(rect.x(),
                      rect.y() + rect.height() / 4,
                      rect.width() * 3 / 4,
                      rect.width() * 3 / 4);

            p.drawRect(r1);

            QVector<QLineF> lines;
            QLineF line1(QPointF(rect.x() + rect.width() * 1 / 4, rect.y() + rect.width() * 1 / 4),
                         QPointF(rect.x() + rect.width() * 1 / 4, rect.y())
                         );
            QLineF line2(QPointF(rect.x() + rect.width() * 1 / 4, rect.y()),
                         QPointF(rect.x() + rect.width(), rect.y())
                         );
            QLineF line3(QPointF(rect.x() + rect.width(), rect.y()),
                         QPointF(rect.x() + rect.width(), rect.y() + rect.width() * 3 / 4)
                         );
            QLineF line4(QPointF(rect.x() + rect.width(), rect.y() + rect.width() * 3 / 4),
                         QPointF(rect.x() + rect.width() * 3 / 4, rect.y() + rect.width() * 3 / 4)
                         );

            lines.append(line1);
            lines.append(line2);
            lines.append(line3);
            lines.append(line4);

            p.drawLines(lines);

            p.restore();
        }

    }
        break;
    case CaptionButtons::Button_Close:
    {
        p.save();
        p.setRenderHint(QPainter::Antialiasing);
        QPen pen(QBrush(color), 1.0);
        p.setPen(pen);

        QRectF rect(0,0,size.width(), size.height());
        p.drawLine(QPointF(rect.x(), rect.y()),
                   QPointF(rect.x() + rect.width(), rect.y() + rect.height())
                   );
        p.drawLine(QPointF(rect.x() + rect.width(), rect.y()),
                   QPointF(rect.x(), rect.y() + rect.height())
                   );

        p.restore();
    }
        break;
    default:
        break;
    }
    return pix;
}

QColor CaptionFrame::getIconColor(bool isClose)
{
    if(!isClose){
        QColor color(Qt::black);
        if(UtilQt::dwmColorPrevalence() && this->isActiveWindow()){
            color = UtilQt::getForgroundColor(QtWin::colorizationColor());
        }else if(inprivate_){
            color =  QColor("#DEDEDE");
        }

        return color;
    }else{
        return Qt::white;
    }
}

CaptionFrame::CaptionButtons CaptionFrame::buttonAt(const QPoint &pos)
{
    if(btnRect(CaptionButtons::Button_Mini).contains(pos)){
        return CaptionButtons::Button_Mini;
    }else if(btnRect(CaptionButtons::Button_NormalMax).contains(pos)){
        return CaptionButtons::Button_NormalMax;
    }else if(btnRect(CaptionButtons::Button_Close).contains(pos)){
        return CaptionButtons::Button_Close;
    }
    return CaptionButtons::Button_None;
}

void CaptionFrame::drawButtons(QPainter *p)
{
    p->save();
    QRectF rectMini = btnRect(CaptionButtons::Button_Mini);
    QRectF rectMax = btnRect(CaptionButtons::Button_NormalMax);
    QRectF rectClose = btnRect(CaptionButtons::Button_Close);
    if(min_button_hover_){
        QColor color(0xB9B9B9);
        color.setAlphaF(0.7);
        p->fillRect(rectMini, color);
    }
    if(max_button_hover_){
        QColor color(0xB9B9B9);
        color.setAlphaF(0.7);
        p->fillRect(rectMax, color);
    }
    if(close_button_hover_){
        p->fillRect(rectClose, QColor(0xE81123));
    }
    // mini
    QPixmap pix = btnPixmap(CaptionButtons::Button_Mini, getIconColor());
    p->drawPixmap(QPointF(rectMini.x() + (rectMini.width() - pix.width()) / 2,
                         rectMini.y() + (rectMini.height() - pix.height()) / 2),
                 pix);
    // normal max
    pix = btnPixmap(CaptionButtons::Button_NormalMax, getIconColor());
    p->drawPixmap(QPointF(rectMax.x() + (rectMax.width() - pix.width()) / 2,
                         rectMax.y() + (rectMax.height() - pix.height()) / 2),
                 pix);
    // close
    if(close_button_hover_){
        pix = btnPixmap(CaptionButtons::Button_Close, Qt::white);
    }else{
        pix = btnPixmap(CaptionButtons::Button_Close, getIconColor());
    }
    p->drawPixmap(QPointF(rectClose.x() + (rectClose.width() - pix.width()) / 2,
                         rectClose.y() + (rectClose.height() - pix.height()) / 2),
                 pix);

    p->restore();
}

void CaptionFrame::clearButtonHover()
{
    min_button_hover_ = false;
    max_button_hover_ = false;
    close_button_hover_ = false;
    update();
}