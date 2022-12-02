/********************************************************************************
** Form generated from reading UI file 'scrollbarsettingsform.ui'
**
** Created by: Qt User Interface Compiler version 5.12.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SCROLLBARSETTINGSFORM_H
#define UI_SCROLLBARSETTINGSFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ScrollBarSettingsForm
{
public:
    QLabel *label;

    void setupUi(QWidget *ScrollBarSettingsForm)
    {
        if (ScrollBarSettingsForm->objectName().isEmpty())
            ScrollBarSettingsForm->setObjectName(QString::fromUtf8("ScrollBarSettingsForm"));
        ScrollBarSettingsForm->resize(474, 387);
        label = new QLabel(ScrollBarSettingsForm);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 70, 171, 16));

        retranslateUi(ScrollBarSettingsForm);

        QMetaObject::connectSlotsByName(ScrollBarSettingsForm);
    } // setupUi

    void retranslateUi(QWidget *ScrollBarSettingsForm)
    {
        ScrollBarSettingsForm->setWindowTitle(QApplication::translate("ScrollBarSettingsForm", "Form", nullptr));
        label->setText(QApplication::translate("ScrollBarSettingsForm", "TODO", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ScrollBarSettingsForm: public Ui_ScrollBarSettingsForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SCROLLBARSETTINGSFORM_H
