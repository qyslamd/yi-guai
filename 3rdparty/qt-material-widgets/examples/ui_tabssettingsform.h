/********************************************************************************
** Form generated from reading UI file 'tabssettingsform.ui'
**
** Created by: Qt User Interface Compiler version 5.12.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TABSSETTINGSFORM_H
#define UI_TABSSETTINGSFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TabsSettingsForm
{
public:
    QLabel *label;

    void setupUi(QWidget *TabsSettingsForm)
    {
        if (TabsSettingsForm->objectName().isEmpty())
            TabsSettingsForm->setObjectName(QString::fromUtf8("TabsSettingsForm"));
        TabsSettingsForm->resize(474, 387);
        label = new QLabel(TabsSettingsForm);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 70, 171, 16));

        retranslateUi(TabsSettingsForm);

        QMetaObject::connectSlotsByName(TabsSettingsForm);
    } // setupUi

    void retranslateUi(QWidget *TabsSettingsForm)
    {
        TabsSettingsForm->setWindowTitle(QApplication::translate("TabsSettingsForm", "Form", nullptr));
        label->setText(QApplication::translate("TabsSettingsForm", "TODO", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TabsSettingsForm: public Ui_TabsSettingsForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TABSSETTINGSFORM_H
