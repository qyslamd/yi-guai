/********************************************************************************
** Form generated from reading UI file 'TabThumbnailWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.12.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TABTHUMBNAILWIDGET_H
#define UI_TABTHUMBNAILWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Tab_Thumbnail_Widget
{
public:
    QHBoxLayout *horizontalLayout;
    QFrame *frameShadow;
    QVBoxLayout *verticalLayout;
    QLabel *labelPix;
    QLabel *labelDesc;

    void setupUi(QWidget *Tab_Thumbnail_Widget)
    {
        if (Tab_Thumbnail_Widget->objectName().isEmpty())
            Tab_Thumbnail_Widget->setObjectName(QString::fromUtf8("Tab_Thumbnail_Widget"));
        Tab_Thumbnail_Widget->resize(324, 250);
        Tab_Thumbnail_Widget->setStyleSheet(QString::fromUtf8(".QWidget{\n"
"background-color:white;\n"
"}"));
        horizontalLayout = new QHBoxLayout(Tab_Thumbnail_Widget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(10, 1, 10, 10);
        frameShadow = new QFrame(Tab_Thumbnail_Widget);
        frameShadow->setObjectName(QString::fromUtf8("frameShadow"));
        frameShadow->setStyleSheet(QString::fromUtf8(".QFrame{\n"
"background-color:#E1FFFF;\n"
"border-radius:8px;\n"
"}"));
        frameShadow->setFrameShape(QFrame::StyledPanel);
        frameShadow->setFrameShadow(QFrame::Raised);
        verticalLayout = new QVBoxLayout(frameShadow);
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(9, 9, 9, 9);
        labelPix = new QLabel(frameShadow);
        labelPix->setObjectName(QString::fromUtf8("labelPix"));
        labelPix->setStyleSheet(QString::fromUtf8("border:1px solid black;"));
        labelPix->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(labelPix);

        labelDesc = new QLabel(frameShadow);
        labelDesc->setObjectName(QString::fromUtf8("labelDesc"));
        labelDesc->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(labelDesc);

        verticalLayout->setStretch(0, 5);

        horizontalLayout->addWidget(frameShadow);


        retranslateUi(Tab_Thumbnail_Widget);

        QMetaObject::connectSlotsByName(Tab_Thumbnail_Widget);
    } // setupUi

    void retranslateUi(QWidget *Tab_Thumbnail_Widget)
    {
        Tab_Thumbnail_Widget->setWindowTitle(QApplication::translate("Tab_Thumbnail_Widget", "Form", nullptr));
        labelPix->setText(QString());
        labelDesc->setText(QApplication::translate("Tab_Thumbnail_Widget", "https://www.baidu.com", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Tab_Thumbnail_Widget: public Ui_Tab_Thumbnail_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TABTHUMBNAILWIDGET_H
