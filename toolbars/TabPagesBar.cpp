#include "TabPagesBar.h"

#include <QHBoxLayout>
#include <QToolButton>

#include "TabBar.h"
#include "utils/util_qt.h"

#ifdef Q_OS_WIN
#include <QtWin>
#endif

TabPagesBar::TabPagesBar(bool inprivate, QWidget *parent)
    : QFrame(parent)
    , inprivate_(inprivate)
    , layout_(new QHBoxLayout)
    , btn_dock_tabs_(new QToolButton)
    , tab_bar_(new TabBar(inprivate))
    , btn_add_page_(new QToolButton)
    , btn_test_(new QToolButton)
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

void TabPagesBar::setCurrentIndex(int index)
{
    tab_bar_->setCurrentIndex(index);
}

void TabPagesBar::onDwmColorChanged()
{
#ifdef Q_OS_WIN

#endif
}

void TabPagesBar::initUi()
{
    QColor activeColor("#CECECE"), inActiveColor = activeColor;    // CECECE E8E8E8
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
    setStyleSheet(QString(".TabPagesBar{background-color:%1;}.TabPagesBar:!active{background-color:%2}")
                  .arg(activeColor.name(QColor::HexArgb))
                  .arg(inActiveColor.name(QColor::HexArgb))
                  );

    setLayout(layout_);
    layout_->setContentsMargins(6, 6, 0, 0);
    layout_->setSpacing(2);
    layout_->addSpacerItem(new QSpacerItem(6,10,QSizePolicy::Fixed));
    layout_->addWidget(btn_dock_tabs_);
    layout_->addWidget(tab_bar_);
    layout_->addWidget(btn_add_page_);
    layout_->addStretch();
    layout_->addWidget(btn_test_);
    layout_->addSpacerItem(new QSpacerItem(180,10,QSizePolicy::Fixed));

    btn_dock_tabs_->setToolTip(tr("open vertical tabs"));
    btn_add_page_->setToolTip(tr("Add a tab page"));

    QSize iconSize(24,24);
    btn_dock_tabs_->setIconSize(iconSize);
    btn_add_page_->setIconSize(iconSize);
    btn_test_->setIconSize(iconSize);
    btn_dock_tabs_->setIcon(QIcon(":/icons/resources/imgs/normal_pagelist_hide.png"));
    btn_add_page_->setIcon(QIcon(":/icons/resources/imgs/plus_64px.png"));
    btn_test_->setIcon(QIcon(":/icons/resources/imgs/view_64px.png"));

    connect(tab_bar_, &TabBar::currentChanged, this, &TabPagesBar::currentChanged);
    connect(tab_bar_, &TabBar::tabCloseRequested, this, &TabPagesBar::tabCloseRequested);
    connect(tab_bar_, &TabBar::tabMoved, this, &TabPagesBar::tabMoved);
    connect(tab_bar_, &TabBar::showPreview, this, &TabPagesBar::showPreview);

    connect(btn_add_page_, &QToolButton::clicked, this, &TabPagesBar::addPage);
    connect(btn_dock_tabs_, &QToolButton::clicked, this, &TabPagesBar::showDockPage);
    connect(btn_test_, &QToolButton::clicked, this, &TabPagesBar::testBtnClicked);
}
