/********************************************************************************
** Form generated from reading UI file 'InprivatePopup.ui'
**
** Created by: Qt User Interface Compiler version 5.12.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_INPRIVATEPOPUP_H
#define UI_INPRIVATEPOPUP_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_InprivatePopup
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label;
    QSpacerItem *horizontalSpacer;
    QHBoxLayout *horizontalLayout;
    QLabel *label_3;
    QLabel *labelHint;
    QPushButton *btnCloseAll;

    void setupUi(QWidget *InprivatePopup)
    {
        if (InprivatePopup->objectName().isEmpty())
            InprivatePopup->setObjectName(QString::fromUtf8("InprivatePopup"));
        InprivatePopup->resize(300, 160);
        verticalLayout = new QVBoxLayout(InprivatePopup);
        verticalLayout->setSpacing(4);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(12, 9, 12, 9);
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label = new QLabel(InprivatePopup);
        label->setObjectName(QString::fromUtf8("label"));
        label->setStyleSheet(QString::fromUtf8("font-size:14pt;\n"
"font-weight:bold;"));

        horizontalLayout_2->addWidget(label);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(12);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(10, -1, -1, -1);
        label_3 = new QLabel(InprivatePopup);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setMinimumSize(QSize(64, 64));
        label_3->setMaximumSize(QSize(64, 64));
        label_3->setPixmap(QPixmap(QString::fromUtf8(":/icons/resources/imgs/gray/private_64px.png")));
        label_3->setScaledContents(true);

        horizontalLayout->addWidget(label_3);

        labelHint = new QLabel(InprivatePopup);
        labelHint->setObjectName(QString::fromUtf8("labelHint"));

        horizontalLayout->addWidget(labelHint);


        verticalLayout->addLayout(horizontalLayout);

        btnCloseAll = new QPushButton(InprivatePopup);
        btnCloseAll->setObjectName(QString::fromUtf8("btnCloseAll"));

        verticalLayout->addWidget(btnCloseAll);

        verticalLayout->setStretch(1, 4);

        retranslateUi(InprivatePopup);

        QMetaObject::connectSlotsByName(InprivatePopup);
    } // setupUi

    void retranslateUi(QWidget *InprivatePopup)
    {
        InprivatePopup->setWindowTitle(QApplication::translate("InprivatePopup", "Frame", nullptr));
        label->setText(QApplication::translate("InprivatePopup", "Inprivate browse", nullptr));
        label_3->setText(QString());
        labelHint->setText(QApplication::translate("InprivatePopup", "TextLabel", nullptr));
        btnCloseAll->setText(QApplication::translate("InprivatePopup", "Close all", nullptr));
    } // retranslateUi

};

namespace Ui {
    class InprivatePopup: public Ui_InprivatePopup {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_INPRIVATEPOPUP_H
