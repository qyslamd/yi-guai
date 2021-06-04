#include "SiteInfoPopup.h"
#include "ui_SiteInfoPopup.h"

#include <QEvent>
#include <QMouseEvent>
#include <QPropertyAnimation>
#include <QTimer>
#include <QUrl>

#include "utils/util_qt.h"

SiteInfoPopup::SiteInfoPopup(QWidget *parent)
    : PopupBase(parent)
    , ui(new Ui::SiteInfoPopup)
{
    ui->setupUi(contentFrame());
    resize(360, 44);
    animation = new QPropertyAnimation(this, "size");
    animation->setDuration(200);
    animation->setLoopCount(1);

    ui->frameSiteState->installEventFilter(this);

    connect(ui->btnClose, &QToolButton::clicked, this, &SiteInfoPopup::close);
    connect(ui->btnBack2General, &QToolButton::clicked, this, [this](){
        ui->stackedWidget->setCurrentWidget(ui->pageGeneral);
    });
    connect(ui->textBrowserSiteDesc, &QTextBrowser::anchorClicked, this, [this](const QUrl &link){
        hide();
        emit openUrl(link);
    });
}

SiteInfoPopup::~SiteInfoPopup()
{
    delete ui;
}

bool SiteInfoPopup::eventFilter(QObject *obj, QEvent *ev)
{
    if(obj == ui->frameSiteState){
        if(ev->type() == QEvent::MouseButtonRelease){
            auto mouseEvent = static_cast<QMouseEvent *>(ev);
            if(mouseEvent->button() == Qt::LeftButton){
                showSiteDesc();
            }
        }
    }
    return PopupBase::eventFilter(obj, ev);
}

void SiteInfoPopup::setDomain(const QString &domain)
{
    site_domain_ = domain;
    ui->labelTitle->setText(tr("View ") + site_domain_);
}

void SiteInfoPopup::setLevel(SecurityLevel level)
{
    level_ = level;
}

void SiteInfoPopup::showEvent(QShowEvent *ev)
{
    PopupBase::showEvent(ev);
    ui->stackedWidget->setCurrentWidget(ui->pageGeneral);

    QTimer::singleShot(200, this, [this](){
        animation->setStartValue(QSize(360, 44));
        animation->setEndValue(QSize(360,290));
        animation->start();
    });
}

void SiteInfoPopup::hideEvent(QHideEvent *ev)
{
    PopupBase::hideEvent(ev);
    resize(360, 44);
}

void SiteInfoPopup::showSiteDesc()
{
    ui->textBrowserSiteDesc->clear();
    switch(level_){
    case SecurityLevel::Http:
        break;
    case SecurityLevel::Https:
    {
        auto desc = UtilQt::readFileUtf8(":/dists/resources/txt/https.txt");
        ui->textBrowserSiteDesc->setHtml(QString::fromUtf8(desc));
    }
        break;
    case SecurityLevel::File:
        break;
    case SecurityLevel::LocalScheme:
        break;
    default:
        break;
    }
    ui->stackedWidget->setCurrentWidget(ui->pageSiteDesc);
}


