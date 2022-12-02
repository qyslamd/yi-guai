/********************************************************************************
** Form generated from reading UI file 'dialogsettingsform.ui'
**
** Created by: Qt User Interface Compiler version 5.12.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOGSETTINGSFORM_H
#define UI_DIALOGSETTINGSFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DialogSettingsForm
{
public:
    QLabel *label;
    QPushButton *showDialogButton;

    void setupUi(QWidget *DialogSettingsForm)
    {
        if (DialogSettingsForm->objectName().isEmpty())
            DialogSettingsForm->setObjectName(QString::fromUtf8("DialogSettingsForm"));
        DialogSettingsForm->resize(474, 387);
        label = new QLabel(DialogSettingsForm);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 70, 171, 16));
        showDialogButton = new QPushButton(DialogSettingsForm);
        showDialogButton->setObjectName(QString::fromUtf8("showDialogButton"));
        showDialogButton->setGeometry(QRect(50, 20, 101, 22));

        retranslateUi(DialogSettingsForm);

        QMetaObject::connectSlotsByName(DialogSettingsForm);
    } // setupUi

    void retranslateUi(QWidget *DialogSettingsForm)
    {
        DialogSettingsForm->setWindowTitle(QApplication::translate("DialogSettingsForm", "Form", nullptr));
        label->setText(QApplication::translate("DialogSettingsForm", "TODO", nullptr));
        showDialogButton->setText(QApplication::translate("DialogSettingsForm", "Show dialog", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DialogSettingsForm: public Ui_DialogSettingsForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOGSETTINGSFORM_H
