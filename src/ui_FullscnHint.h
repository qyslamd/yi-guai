/********************************************************************************
** Form generated from reading UI file 'FullscnHint.ui'
**
** Created by: Qt User Interface Compiler version 5.12.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FULLSCNHINT_H
#define UI_FULLSCNHINT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>

QT_BEGIN_NAMESPACE

class Ui_FullscnHint
{
public:
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QLabel *label_2;
    QLabel *labelKeyStr;
    QLabel *label_3;
    QSpacerItem *horizontalSpacer_2;

    void setupUi(QFrame *FullscnHint)
    {
        if (FullscnHint->objectName().isEmpty())
            FullscnHint->setObjectName(QString::fromUtf8("FullscnHint"));
        FullscnHint->resize(350, 50);
        FullscnHint->setStyleSheet(QString::fromUtf8("QFrame#FullscnHint{\n"
"border-radius:12px;\n"
"background-color: #515459;\n"
"}\n"
"\n"
""));
        horizontalLayout = new QHBoxLayout(FullscnHint);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(61, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        label_2 = new QLabel(FullscnHint);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setStyleSheet(QString::fromUtf8("color:white;"));

        horizontalLayout->addWidget(label_2);

        labelKeyStr = new QLabel(FullscnHint);
        labelKeyStr->setObjectName(QString::fromUtf8("labelKeyStr"));
        labelKeyStr->setStyleSheet(QString::fromUtf8("border:2px solid white;\n"
"border-radius:4px;\n"
"padding:4px;\n"
"color:white;"));
        labelKeyStr->setText(QString::fromUtf8("F11"));
        labelKeyStr->setAlignment(Qt::AlignCenter);

        horizontalLayout->addWidget(labelKeyStr);

        label_3 = new QLabel(FullscnHint);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setStyleSheet(QString::fromUtf8("color:white;"));

        horizontalLayout->addWidget(label_3);

        horizontalSpacer_2 = new QSpacerItem(61, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);


        retranslateUi(FullscnHint);

        QMetaObject::connectSlotsByName(FullscnHint);
    } // setupUi

    void retranslateUi(QFrame *FullscnHint)
    {
        FullscnHint->setWindowTitle(QApplication::translate("FullscnHint", "Frame", nullptr));
        label_2->setText(QApplication::translate("FullscnHint", "Press", nullptr));
        label_3->setText(QApplication::translate("FullscnHint", "to quit full screen", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FullscnHint: public Ui_FullscnHint {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FULLSCNHINT_H
