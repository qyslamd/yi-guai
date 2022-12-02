/********************************************************************************
** Form generated from reading UI file 'popup_browser.ui'
**
** Created by: Qt User Interface Compiler version 5.12.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_POPUP_BROWSER_H
#define UI_POPUP_BROWSER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PopupBrowser
{
public:
    QVBoxLayout *verticalLayout;
    QLineEdit *lineEdit;

    void setupUi(QWidget *PopupBrowser)
    {
        if (PopupBrowser->objectName().isEmpty())
            PopupBrowser->setObjectName(QString::fromUtf8("PopupBrowser"));
        PopupBrowser->resize(416, 326);
        verticalLayout = new QVBoxLayout(PopupBrowser);
        verticalLayout->setSpacing(3);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 2, 0, 0);
        lineEdit = new QLineEdit(PopupBrowser);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setMinimumSize(QSize(0, 30));
        lineEdit->setStyleSheet(QString::fromUtf8("QLineEdit{\n"
"border:none;\n"
"border-bottom:1px solid #CCCCCC;\n"
"}\n"
"QLineEdit:hover{\n"
"border:1px solid #CCCCCC;\n"
"}\n"
"QLineEdit:focus{\n"
"border:1px solid #A3CFEC;\n"
"}"));

        verticalLayout->addWidget(lineEdit);


        retranslateUi(PopupBrowser);

        QMetaObject::connectSlotsByName(PopupBrowser);
    } // setupUi

    void retranslateUi(QWidget *PopupBrowser)
    {
        PopupBrowser->setWindowTitle(QApplication::translate("PopupBrowser", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class PopupBrowser: public Ui_PopupBrowser {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_POPUP_BROWSER_H
