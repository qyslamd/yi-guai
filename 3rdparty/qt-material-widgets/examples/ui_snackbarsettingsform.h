/********************************************************************************
** Form generated from reading UI file 'snackbarsettingsform.ui'
**
** Created by: Qt User Interface Compiler version 5.12.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SNACKBARSETTINGSFORM_H
#define UI_SNACKBARSETTINGSFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SnackbarSettingsForm
{
public:
    QPushButton *showSnackbarButton;
    QLabel *label;

    void setupUi(QWidget *SnackbarSettingsForm)
    {
        if (SnackbarSettingsForm->objectName().isEmpty())
            SnackbarSettingsForm->setObjectName(QString::fromUtf8("SnackbarSettingsForm"));
        SnackbarSettingsForm->resize(599, 418);
        showSnackbarButton = new QPushButton(SnackbarSettingsForm);
        showSnackbarButton->setObjectName(QString::fromUtf8("showSnackbarButton"));
        showSnackbarButton->setGeometry(QRect(20, 60, 141, 22));
        label = new QLabel(SnackbarSettingsForm);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(20, 30, 171, 16));

        retranslateUi(SnackbarSettingsForm);

        QMetaObject::connectSlotsByName(SnackbarSettingsForm);
    } // setupUi

    void retranslateUi(QWidget *SnackbarSettingsForm)
    {
        SnackbarSettingsForm->setWindowTitle(QApplication::translate("SnackbarSettingsForm", "Form", nullptr));
        showSnackbarButton->setText(QApplication::translate("SnackbarSettingsForm", "Show snackbar", nullptr));
        label->setText(QApplication::translate("SnackbarSettingsForm", "TODO", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SnackbarSettingsForm: public Ui_SnackbarSettingsForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SNACKBARSETTINGSFORM_H
