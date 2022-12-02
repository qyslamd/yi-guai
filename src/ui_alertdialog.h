/********************************************************************************
** Form generated from reading UI file 'alertdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.12.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ALERTDIALOG_H
#define UI_ALERTDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_AlertDialog
{
public:
    QHBoxLayout *horizontalLayout;
    QFrame *frameShadow;
    QVBoxLayout *verticalLayout_2;
    QFrame *frameTop;
    QVBoxLayout *verticalLayout;
    QLabel *labelTitle;
    QLabel *labelMsg;
    QFrame *frameBottom;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer;
    QPushButton *btnOK;

    void setupUi(QDialog *AlertDialog)
    {
        if (AlertDialog->objectName().isEmpty())
            AlertDialog->setObjectName(QString::fromUtf8("AlertDialog"));
        AlertDialog->resize(366, 179);
        AlertDialog->setStyleSheet(QString::fromUtf8("font-family: \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));
        horizontalLayout = new QHBoxLayout(AlertDialog);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        frameShadow = new QFrame(AlertDialog);
        frameShadow->setObjectName(QString::fromUtf8("frameShadow"));
        frameShadow->setStyleSheet(QString::fromUtf8("QFrame#frameShadow{\n"
"background-color:white;\n"
"border-radius:8px;\n"
"}"));
        frameShadow->setFrameShape(QFrame::StyledPanel);
        frameShadow->setFrameShadow(QFrame::Raised);
        verticalLayout_2 = new QVBoxLayout(frameShadow);
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        frameTop = new QFrame(frameShadow);
        frameTop->setObjectName(QString::fromUtf8("frameTop"));
        frameTop->setFrameShape(QFrame::StyledPanel);
        frameTop->setFrameShadow(QFrame::Raised);
        verticalLayout = new QVBoxLayout(frameTop);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        labelTitle = new QLabel(frameTop);
        labelTitle->setObjectName(QString::fromUtf8("labelTitle"));
        labelTitle->setStyleSheet(QString::fromUtf8("font: 10pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));

        verticalLayout->addWidget(labelTitle);

        labelMsg = new QLabel(frameTop);
        labelMsg->setObjectName(QString::fromUtf8("labelMsg"));
        labelMsg->setStyleSheet(QString::fromUtf8("font-size:8pt;"));
        labelMsg->setWordWrap(true);

        verticalLayout->addWidget(labelMsg);

        verticalLayout->setStretch(1, 2);

        verticalLayout_2->addWidget(frameTop);

        frameBottom = new QFrame(frameShadow);
        frameBottom->setObjectName(QString::fromUtf8("frameBottom"));
        frameBottom->setStyleSheet(QString::fromUtf8("QFrame#frameBottom{\n"
"background-color:#FBFBFB;\n"
"}"));
        frameBottom->setFrameShape(QFrame::StyledPanel);
        frameBottom->setFrameShadow(QFrame::Raised);
        horizontalLayout_2 = new QHBoxLayout(frameBottom);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalSpacer = new QSpacerItem(244, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        btnOK = new QPushButton(frameBottom);
        btnOK->setObjectName(QString::fromUtf8("btnOK"));
        btnOK->setMinimumSize(QSize(75, 30));
        btnOK->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"border:none;\n"
"border-top-right-radius:8px;\n"
"border-bottom-left-radius:8px;\n"
"background-color:#36C190;\n"
"color:white;\n"
"}\n"
"\n"
"QPushButton:hover{\n"
"background-color:#5F9EA0;\n"
"border:1px solid #36C190;\n"
"}"));

        horizontalLayout_2->addWidget(btnOK);


        verticalLayout_2->addWidget(frameBottom);

        verticalLayout_2->setStretch(0, 2);

        horizontalLayout->addWidget(frameShadow);


        retranslateUi(AlertDialog);

        QMetaObject::connectSlotsByName(AlertDialog);
    } // setupUi

    void retranslateUi(QDialog *AlertDialog)
    {
        AlertDialog->setWindowTitle(QApplication::translate("AlertDialog", "Dialog", nullptr));
        labelTitle->setText(QApplication::translate("AlertDialog", "TextLabel", nullptr));
        labelMsg->setText(QApplication::translate("AlertDialog", "TextLabel", nullptr));
        btnOK->setText(QApplication::translate("AlertDialog", "\347\241\256\345\256\232", nullptr));
    } // retranslateUi

};

namespace Ui {
    class AlertDialog: public Ui_AlertDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ALERTDIALOG_H
