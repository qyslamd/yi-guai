#include "SiteInfoPopup.h"
#include <QWidgetAction>
#include <QHBoxLayout>
#include <QLabel>
#include <QToolButton>

SiteInfoPopup::SiteInfoPopup(QWidget *parent)
    : QMenu(parent)
{
    action_title_ = new QWidgetAction(this);

    QWidget *titleW = new QWidget(this);
    QHBoxLayout * titleLayout = new QHBoxLayout(titleW);

    label_title_ = new QLabel(this);
    QToolButton *btnClose = new QToolButton(this);
    btnClose->setIcon(QIcon(":/icons/resources/close_24px.png"));
    connect(btnClose, &QToolButton::clicked, this, &SiteInfoPopup::hide);

    titleLayout->addWidget(label_title_);
    titleLayout->addStretch();
    titleLayout->addWidget(btnClose);
    titleW->setLayout(titleLayout);

    action_title_->setDefaultWidget(titleW);
    addAction(action_title_);
}

SiteInfoPopup::~SiteInfoPopup()
{

}

void SiteInfoPopup::setTitle(const QString &title)
{
    label_title_->setText(title);
}

void SiteInfoPopup::showEvent(QShowEvent *event)
{

}
