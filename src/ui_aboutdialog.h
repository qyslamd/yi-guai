/********************************************************************************
** Form generated from reading UI file 'aboutdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.12.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ABOUTDIALOG_H
#define UI_ABOUTDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_AboutDialog
{
public:
    QHBoxLayout *horizontalLayout;
    QFrame *frameShadow;
    QFrame *frame;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label;
    QToolButton *toolButton;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QSpacerItem *verticalSpacer;
    QLabel *descLabel;
    QLabel *copyrightLabel;
    QLabel *termsLabel;

    void setupUi(QDialog *AboutDialog)
    {
        if (AboutDialog->objectName().isEmpty())
            AboutDialog->setObjectName(QString::fromUtf8("AboutDialog"));
        AboutDialog->resize(851, 562);
        AboutDialog->setStyleSheet(QString::fromUtf8(""));
        horizontalLayout = new QHBoxLayout(AboutDialog);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        frameShadow = new QFrame(AboutDialog);
        frameShadow->setObjectName(QString::fromUtf8("frameShadow"));
        frameShadow->setStyleSheet(QString::fromUtf8("QFrame#frameShadow{\n"
"background-color: rgba(211,211,211,50%);\n"
"}"));
        frameShadow->setFrameShape(QFrame::StyledPanel);
        frameShadow->setFrameShadow(QFrame::Raised);
        frame = new QFrame(frameShadow);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setGeometry(QRect(90, 90, 631, 327));
        frame->setStyleSheet(QString::fromUtf8("QFrame#frame{\n"
"border-radius:8px;\n"
"background-color:white;\n"
"}"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        verticalLayout = new QVBoxLayout(frame);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label = new QLabel(frame);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout_2->addWidget(label);

        toolButton = new QToolButton(frame);
        toolButton->setObjectName(QString::fromUtf8("toolButton"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/resources/imgs/ios7/delete_50px.png"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton->setIcon(icon);

        horizontalLayout_2->addWidget(toolButton);


        verticalLayout->addLayout(horizontalLayout_2);

        label_2 = new QLabel(frame);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setMinimumSize(QSize(128, 128));
        label_2->setPixmap(QPixmap(QString::fromUtf8(":/icons/resources/imgs/colorful/corgi_96px.png")));
        label_2->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(label_2);

        label_3 = new QLabel(frame);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setStyleSheet(QString::fromUtf8("font-weight:bold;\n"
"font-family:\"\345\276\256\350\275\257\351\233\205\351\273\221\";\n"
"font-size:12pt;"));
        label_3->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(label_3);

        label_4 = new QLabel(frame);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(label_4);

        verticalSpacer = new QSpacerItem(20, 35, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        descLabel = new QLabel(frame);
        descLabel->setObjectName(QString::fromUtf8("descLabel"));
        descLabel->setText(QString::fromUtf8("<html><head/><body><p>\346\255\244\346\265\217\350\247\210\345\231\250\345\237\272\344\272\216 <a href=\"https://www.chromium.org/\"><span style=\" text-decoration: underline; color:#007af4;\">Chromium</span></a>\357\274\214<a href=\"https://bitbucket.org/chromiumembedded/\"><span style=\" text-decoration: underline; color:#007af4;\">Cef</span></a>\357\274\214<a href=\"https://www.qt.io/\"><span style=\" text-decoration: underline; color:#007af4;\">Qt</span></a>\345\274\200\346\272\220\351\241\271\347\233\256\345\217\212\345\205\266\345\256\203<a href=\"about://credits\"><span style=\" text-decoration: underline; color:#007af4;\">\345\274\200\346\272\220\350\275\257\344\273\266</span></a>\343\200\202</p></body></html>"));
        descLabel->setTextFormat(Qt::RichText);
        descLabel->setAlignment(Qt::AlignCenter);
        descLabel->setWordWrap(true);
        descLabel->setOpenExternalLinks(false);

        verticalLayout->addWidget(descLabel);

        copyrightLabel = new QLabel(frame);
        copyrightLabel->setObjectName(QString::fromUtf8("copyrightLabel"));
        copyrightLabel->setStyleSheet(QString::fromUtf8("font-size:11pt;\n"
"color:#999999;"));
        copyrightLabel->setText(QString::fromUtf8("Copyright 2019-2021 Zhouyohu. All rights reserved."));
        copyrightLabel->setAlignment(Qt::AlignCenter);
        copyrightLabel->setWordWrap(true);

        verticalLayout->addWidget(copyrightLabel);

        termsLabel = new QLabel(frame);
        termsLabel->setObjectName(QString::fromUtf8("termsLabel"));
        termsLabel->setText(QString::fromUtf8("<html><head/><body><p><a href=\"deepblue://terms\"><span style=\" text-decoration: underline; color:#007af4;\">\344\275\277\347\224\250\346\235\241\346\254\276</span></a> - <a href=\"deepblue://privacy\"><span style=\" text-decoration: underline; color:#007af4;\">\351\232\220\347\247\201\347\224\263\346\230\216</span></a></p></body></html>"));
        termsLabel->setTextFormat(Qt::RichText);
        termsLabel->setAlignment(Qt::AlignCenter);
        termsLabel->setOpenExternalLinks(false);

        verticalLayout->addWidget(termsLabel);


        horizontalLayout->addWidget(frameShadow);


        retranslateUi(AboutDialog);

        QMetaObject::connectSlotsByName(AboutDialog);
    } // setupUi

    void retranslateUi(QDialog *AboutDialog)
    {
        AboutDialog->setWindowTitle(QApplication::translate("AboutDialog", "Form", nullptr));
        label->setText(QApplication::translate("AboutDialog", "About", nullptr));
        toolButton->setText(QString());
        label_2->setText(QString());
        label_3->setText(QApplication::translate("AboutDialog", "YiGuai browser 0.0.0.1(build:0001)32bit", nullptr));
        label_4->setText(QApplication::translate("AboutDialog", "cef xx.xx.xx + chrormium-86.0.3945.130,Qt 5.12.4", nullptr));
    } // retranslateUi

};

namespace Ui {
    class AboutDialog: public Ui_AboutDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ABOUTDIALOG_H
