#include "popup_browser.h"
#include "ui_popup_browser.h"
#include "popups/SiteInfoPopup.h"
#include <QAction>
#include <QToolButton>
#include <QtDebug>
#include <QGraphicsDropShadowEffect>


#include "cef_qwidget.h"

PopupBrowser::PopupBrowser(CefQWidget *browser, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PopupBrowser)
    , action_site_info_(new QAction(this))
    , site_info_widget_(new SiteInfoPopup(this))
    , browser_(browser)
{
    ui->setupUi(this);

    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    shadow->setOffset(0, 0);
    shadow->setColor(QColor(38, 78, 119, 127));
    shadow->setBlurRadius(3);
    ui->lineEdit->setGraphicsEffect(shadow);
    ui->lineEdit->setReadOnly(true);

    ui->verticalLayout->addWidget(browser_);

    action_site_info_->setIcon(QIcon(":/icons/resources/imgs/ios7/protect_50px.png"));
    ui->lineEdit->addAction(action_site_info_, QLineEdit::LeadingPosition);
    connect(action_site_info_, &QAction::triggered, this, &PopupBrowser::onSiteInfoClicked);

    site_info_widget_->hide();


    for(auto w : action_site_info_->associatedWidgets()){
        if(w->metaObject()->superClass()->className() == QString("QToolButton"))
        {
            internal_btn_siteInfo_ = qobject_cast<QToolButton *>(w);
//            if(internal_btn_siteInfo_){
//                internal_btn_siteInfo_->installEventFilter(this);
//            }
        }
    }

    connect(browser_, &CefQWidget::browserTitleChange, this, &PopupBrowser::onBrowserTitleChange);
    connect(browser_, &CefQWidget::browserAddressChange, this, &PopupBrowser::onBrowserAddrChange);
    connect(browser_, &CefQWidget::browserFaviconChange, this, &PopupBrowser::onBrowserFavicon);
}

PopupBrowser::~PopupBrowser()
{
    qInfo()<<__FUNCTION__;
    delete ui;
}

void PopupBrowser::onSiteInfoClicked()
{
    QPoint pos;
    if(internal_btn_siteInfo_){
        pos = ui->lineEdit->mapFromGlobal(internal_btn_siteInfo_->pos());
        pos.ry() += internal_btn_siteInfo_->height();
    }else{
        pos = mapToGlobal(ui->lineEdit->pos());
        pos.ry() += ui->lineEdit->height();
    }

    site_info_widget_->move(pos);
    site_info_widget_->show();
}

void PopupBrowser::onBrowserTitleChange(const QString &txt)
{
    setWindowTitle(txt);
}

void PopupBrowser::onBrowserAddrChange(const QString &txt)
{
    ui->lineEdit->setText(txt);
}

void PopupBrowser::onBrowserFavicon(const QPixmap &pix)
{
    setWindowIcon(QIcon(pix));
}

