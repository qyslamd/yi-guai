/********************************************************************************
** Form generated from reading UI file 'PopupBase.ui'
**
** Created by: Qt User Interface Compiler version 5.12.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_POPUPBASE_H
#define UI_POPUPBASE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PopupBase
{
public:
    QVBoxLayout *verticalLayout;
    QFrame *frameShadow;

    void setupUi(QWidget *PopupBase)
    {
        if (PopupBase->objectName().isEmpty())
            PopupBase->setObjectName(QString::fromUtf8("PopupBase"));
        PopupBase->resize(402, 391);
        verticalLayout = new QVBoxLayout(PopupBase);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(20, 0, 20, 20);
        frameShadow = new QFrame(PopupBase);
        frameShadow->setObjectName(QString::fromUtf8("frameShadow"));
        frameShadow->setStyleSheet(QString::fromUtf8("QFrame#frameShadow{\n"
"border:none;\n"
"border:1px solid #F5F5F5;\n"
"border-radius:8px;\n"
"background-color:white;\n"
"}"));
        frameShadow->setFrameShape(QFrame::StyledPanel);
        frameShadow->setFrameShadow(QFrame::Raised);

        verticalLayout->addWidget(frameShadow);


        retranslateUi(PopupBase);

        QMetaObject::connectSlotsByName(PopupBase);
    } // setupUi

    void retranslateUi(QWidget *PopupBase)
    {
        PopupBase->setWindowTitle(QApplication::translate("PopupBase", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class PopupBase: public Ui_PopupBase {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_POPUPBASE_H
