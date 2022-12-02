/********************************************************************************
** Form generated from reading UI file 'SiteInfoPopup.ui'
**
** Created by: Qt User Interface Compiler version 5.12.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SITEINFOPOPUP_H
#define UI_SITEINFOPOPUP_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SiteInfoPopup
{
public:
    QVBoxLayout *verticalLayout;
    QFrame *frameTitle;
    QHBoxLayout *horizontalLayout;
    QLabel *labelTitle;
    QSpacerItem *horizontalSpacer;
    QToolButton *btnClose;
    QStackedWidget *stackedWidget;
    QWidget *pageGeneral;
    QFrame *frameSiteState;
    QHBoxLayout *horizontalLayout_2;
    QLabel *labelIcon;
    QLabel *labelDesc;
    QLabel *label_3;
    QWidget *pageSiteDesc;
    QTextBrowser *textBrowserSiteDesc;
    QToolButton *btnBack2General;
    QToolButton *btnCertificate;

    void setupUi(QWidget *SiteInfoPopup)
    {
        if (SiteInfoPopup->objectName().isEmpty())
            SiteInfoPopup->setObjectName(QString::fromUtf8("SiteInfoPopup"));
        SiteInfoPopup->resize(360, 271);
        SiteInfoPopup->setWindowTitle(QString::fromUtf8("Form"));
        verticalLayout = new QVBoxLayout(SiteInfoPopup);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(1, 1, 1, 1);
        frameTitle = new QFrame(SiteInfoPopup);
        frameTitle->setObjectName(QString::fromUtf8("frameTitle"));
        frameTitle->setStyleSheet(QString::fromUtf8(".QFrame{\n"
"border:none;\n"
"border-bottom:1px solid #DCDCDC;\n"
"}\n"
".QFrame:hover{\n"
"background-color:#EBEBEB;\n"
"border-radius:4px;\n"
"}"));
        frameTitle->setFrameShape(QFrame::StyledPanel);
        frameTitle->setFrameShadow(QFrame::Raised);
        horizontalLayout = new QHBoxLayout(frameTitle);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        labelTitle = new QLabel(frameTitle);
        labelTitle->setObjectName(QString::fromUtf8("labelTitle"));
        labelTitle->setStyleSheet(QString::fromUtf8("font-weight:bold;"));
        labelTitle->setText(QString::fromUtf8("TextLabel"));

        horizontalLayout->addWidget(labelTitle);

        horizontalSpacer = new QSpacerItem(237, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        btnClose = new QToolButton(frameTitle);
        btnClose->setObjectName(QString::fromUtf8("btnClose"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/resources/imgs/general_close.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnClose->setIcon(icon);

        horizontalLayout->addWidget(btnClose);


        verticalLayout->addWidget(frameTitle);

        stackedWidget = new QStackedWidget(SiteInfoPopup);
        stackedWidget->setObjectName(QString::fromUtf8("stackedWidget"));
        pageGeneral = new QWidget();
        pageGeneral->setObjectName(QString::fromUtf8("pageGeneral"));
        frameSiteState = new QFrame(pageGeneral);
        frameSiteState->setObjectName(QString::fromUtf8("frameSiteState"));
        frameSiteState->setGeometry(QRect(5, 1, 341, 44));
        frameSiteState->setStyleSheet(QString::fromUtf8(".QFrame:hover{\n"
"background-color:#EBEBEB;\n"
"border-radius:4px;\n"
"}"));
        frameSiteState->setFrameShape(QFrame::StyledPanel);
        frameSiteState->setFrameShadow(QFrame::Raised);
        horizontalLayout_2 = new QHBoxLayout(frameSiteState);
        horizontalLayout_2->setSpacing(8);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        labelIcon = new QLabel(frameSiteState);
        labelIcon->setObjectName(QString::fromUtf8("labelIcon"));
        labelIcon->setMinimumSize(QSize(24, 24));
        labelIcon->setMaximumSize(QSize(24, 24));
        labelIcon->setPixmap(QPixmap(QString::fromUtf8(":/icons/resources/imgs/lock_100px.png")));
        labelIcon->setScaledContents(true);

        horizontalLayout_2->addWidget(labelIcon);

        labelDesc = new QLabel(frameSiteState);
        labelDesc->setObjectName(QString::fromUtf8("labelDesc"));

        horizontalLayout_2->addWidget(labelDesc);

        label_3 = new QLabel(frameSiteState);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setMinimumSize(QSize(24, 24));
        label_3->setMaximumSize(QSize(24, 24));
        label_3->setPixmap(QPixmap(QString::fromUtf8(":/icons/resources/imgs/forward_64px.png")));
        label_3->setScaledContents(true);

        horizontalLayout_2->addWidget(label_3);

        stackedWidget->addWidget(pageGeneral);
        pageSiteDesc = new QWidget();
        pageSiteDesc->setObjectName(QString::fromUtf8("pageSiteDesc"));
        textBrowserSiteDesc = new QTextBrowser(pageSiteDesc);
        textBrowserSiteDesc->setObjectName(QString::fromUtf8("textBrowserSiteDesc"));
        textBrowserSiteDesc->setGeometry(QRect(26, 39, 301, 172));
        textBrowserSiteDesc->setFrameShape(QFrame::NoFrame);
        textBrowserSiteDesc->setReadOnly(true);
        textBrowserSiteDesc->setOpenLinks(false);
        btnBack2General = new QToolButton(pageSiteDesc);
        btnBack2General->setObjectName(QString::fromUtf8("btnBack2General"));
        btnBack2General->setGeometry(QRect(10, 10, 23, 22));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/icons/resources/imgs/arrow_back_left2_64px.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnBack2General->setIcon(icon1);
        btnCertificate = new QToolButton(pageSiteDesc);
        btnCertificate->setObjectName(QString::fromUtf8("btnCertificate"));
        btnCertificate->setGeometry(QRect(270, 10, 23, 22));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/icons/resources/imgs/certificate_48px.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnCertificate->setIcon(icon2);
        stackedWidget->addWidget(pageSiteDesc);

        verticalLayout->addWidget(stackedWidget);


        retranslateUi(SiteInfoPopup);

        stackedWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(SiteInfoPopup);
    } // setupUi

    void retranslateUi(QWidget *SiteInfoPopup)
    {
        btnClose->setText(QString());
        labelIcon->setText(QString());
        labelDesc->setText(QApplication::translate("SiteInfoPopup", "\351\223\276\346\216\245\345\256\211\345\205\250", nullptr));
        label_3->setText(QString());
        textBrowserSiteDesc->setHtml(QApplication::translate("SiteInfoPopup", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'SimSun'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p></body></html>", nullptr));
        btnBack2General->setText(QString());
        btnCertificate->setText(QString());
        Q_UNUSED(SiteInfoPopup);
    } // retranslateUi

};

namespace Ui {
    class SiteInfoPopup: public Ui_SiteInfoPopup {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SITEINFOPOPUP_H
