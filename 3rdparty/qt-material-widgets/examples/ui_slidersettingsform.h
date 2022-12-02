/********************************************************************************
** Form generated from reading UI file 'slidersettingsform.ui'
**
** Created by: Qt User Interface Compiler version 5.12.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SLIDERSETTINGSFORM_H
#define UI_SLIDERSETTINGSFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SliderSettingsForm
{
public:
    QWidget *formLayoutWidget;
    QFormLayout *formLayout;
    QLabel *disabledLabel;
    QCheckBox *disabledCheckBox;
    QLabel *valueLabel;
    QLineEdit *valueLineEdit;
    QLabel *orientationLabel;
    QComboBox *orientationComboBox;
    QLabel *invertedLabel;
    QCheckBox *invertedCheckBox;
    QLabel *label;

    void setupUi(QWidget *SliderSettingsForm)
    {
        if (SliderSettingsForm->objectName().isEmpty())
            SliderSettingsForm->setObjectName(QString::fromUtf8("SliderSettingsForm"));
        SliderSettingsForm->resize(474, 387);
        formLayoutWidget = new QWidget(SliderSettingsForm);
        formLayoutWidget->setObjectName(QString::fromUtf8("formLayoutWidget"));
        formLayoutWidget->setGeometry(QRect(0, 0, 361, 111));
        formLayout = new QFormLayout(formLayoutWidget);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        formLayout->setContentsMargins(0, 0, 0, 0);
        disabledLabel = new QLabel(formLayoutWidget);
        disabledLabel->setObjectName(QString::fromUtf8("disabledLabel"));

        formLayout->setWidget(0, QFormLayout::LabelRole, disabledLabel);

        disabledCheckBox = new QCheckBox(formLayoutWidget);
        disabledCheckBox->setObjectName(QString::fromUtf8("disabledCheckBox"));

        formLayout->setWidget(0, QFormLayout::FieldRole, disabledCheckBox);

        valueLabel = new QLabel(formLayoutWidget);
        valueLabel->setObjectName(QString::fromUtf8("valueLabel"));

        formLayout->setWidget(1, QFormLayout::LabelRole, valueLabel);

        valueLineEdit = new QLineEdit(formLayoutWidget);
        valueLineEdit->setObjectName(QString::fromUtf8("valueLineEdit"));

        formLayout->setWidget(1, QFormLayout::FieldRole, valueLineEdit);

        orientationLabel = new QLabel(formLayoutWidget);
        orientationLabel->setObjectName(QString::fromUtf8("orientationLabel"));

        formLayout->setWidget(2, QFormLayout::LabelRole, orientationLabel);

        orientationComboBox = new QComboBox(formLayoutWidget);
        orientationComboBox->addItem(QString());
        orientationComboBox->addItem(QString());
        orientationComboBox->setObjectName(QString::fromUtf8("orientationComboBox"));

        formLayout->setWidget(2, QFormLayout::FieldRole, orientationComboBox);

        invertedLabel = new QLabel(formLayoutWidget);
        invertedLabel->setObjectName(QString::fromUtf8("invertedLabel"));

        formLayout->setWidget(3, QFormLayout::LabelRole, invertedLabel);

        invertedCheckBox = new QCheckBox(formLayoutWidget);
        invertedCheckBox->setObjectName(QString::fromUtf8("invertedCheckBox"));

        formLayout->setWidget(3, QFormLayout::FieldRole, invertedCheckBox);

        label = new QLabel(SliderSettingsForm);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(20, 130, 171, 16));

        retranslateUi(SliderSettingsForm);

        QMetaObject::connectSlotsByName(SliderSettingsForm);
    } // setupUi

    void retranslateUi(QWidget *SliderSettingsForm)
    {
        SliderSettingsForm->setWindowTitle(QApplication::translate("SliderSettingsForm", "Form", nullptr));
        disabledLabel->setText(QApplication::translate("SliderSettingsForm", "Disabled", nullptr));
        valueLabel->setText(QApplication::translate("SliderSettingsForm", "Value", nullptr));
        orientationLabel->setText(QApplication::translate("SliderSettingsForm", "Orientation", nullptr));
        orientationComboBox->setItemText(0, QApplication::translate("SliderSettingsForm", "Horizontal", nullptr));
        orientationComboBox->setItemText(1, QApplication::translate("SliderSettingsForm", "Vertical", nullptr));

        invertedLabel->setText(QApplication::translate("SliderSettingsForm", "Inverted", nullptr));
        label->setText(QApplication::translate("SliderSettingsForm", "TODO", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SliderSettingsForm: public Ui_SliderSettingsForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SLIDERSETTINGSFORM_H
