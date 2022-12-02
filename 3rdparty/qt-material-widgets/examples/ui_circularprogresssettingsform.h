/********************************************************************************
** Form generated from reading UI file 'circularprogresssettingsform.ui'
**
** Created by: Qt User Interface Compiler version 5.12.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CIRCULARPROGRESSSETTINGSFORM_H
#define UI_CIRCULARPROGRESSSETTINGSFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CircularProgressSettingsForm
{
public:
    QWidget *formLayoutWidget;
    QFormLayout *formLayout;
    QLabel *disabledLabel;
    QCheckBox *disabledCheckBox;
    QLabel *progressTypeLabel;
    QComboBox *progressTypeComboBox;
    QLabel *lineWidthLabel;
    QDoubleSpinBox *lineWidthDoubleSpinBox;
    QLabel *sizeLabel;
    QSpinBox *sizeSpinBox;
    QLabel *useThemeColorsLabel;
    QCheckBox *useThemeColorsCheckBox;
    QLabel *colorLabel;
    QHBoxLayout *horizontalLayout;
    QLineEdit *colorLineEdit;
    QToolButton *colorToolButton;
    QLabel *progressLabel;
    QSlider *progressSlider;

    void setupUi(QWidget *CircularProgressSettingsForm)
    {
        if (CircularProgressSettingsForm->objectName().isEmpty())
            CircularProgressSettingsForm->setObjectName(QString::fromUtf8("CircularProgressSettingsForm"));
        CircularProgressSettingsForm->resize(400, 300);
        formLayoutWidget = new QWidget(CircularProgressSettingsForm);
        formLayoutWidget->setObjectName(QString::fromUtf8("formLayoutWidget"));
        formLayoutWidget->setGeometry(QRect(0, 0, 221, 197));
        formLayout = new QFormLayout(formLayoutWidget);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        formLayout->setContentsMargins(0, 0, 0, 0);
        disabledLabel = new QLabel(formLayoutWidget);
        disabledLabel->setObjectName(QString::fromUtf8("disabledLabel"));

        formLayout->setWidget(0, QFormLayout::LabelRole, disabledLabel);

        disabledCheckBox = new QCheckBox(formLayoutWidget);
        disabledCheckBox->setObjectName(QString::fromUtf8("disabledCheckBox"));

        formLayout->setWidget(0, QFormLayout::FieldRole, disabledCheckBox);

        progressTypeLabel = new QLabel(formLayoutWidget);
        progressTypeLabel->setObjectName(QString::fromUtf8("progressTypeLabel"));

        formLayout->setWidget(1, QFormLayout::LabelRole, progressTypeLabel);

        progressTypeComboBox = new QComboBox(formLayoutWidget);
        progressTypeComboBox->addItem(QString());
        progressTypeComboBox->addItem(QString());
        progressTypeComboBox->setObjectName(QString::fromUtf8("progressTypeComboBox"));

        formLayout->setWidget(1, QFormLayout::FieldRole, progressTypeComboBox);

        lineWidthLabel = new QLabel(formLayoutWidget);
        lineWidthLabel->setObjectName(QString::fromUtf8("lineWidthLabel"));

        formLayout->setWidget(3, QFormLayout::LabelRole, lineWidthLabel);

        lineWidthDoubleSpinBox = new QDoubleSpinBox(formLayoutWidget);
        lineWidthDoubleSpinBox->setObjectName(QString::fromUtf8("lineWidthDoubleSpinBox"));

        formLayout->setWidget(3, QFormLayout::FieldRole, lineWidthDoubleSpinBox);

        sizeLabel = new QLabel(formLayoutWidget);
        sizeLabel->setObjectName(QString::fromUtf8("sizeLabel"));

        formLayout->setWidget(4, QFormLayout::LabelRole, sizeLabel);

        sizeSpinBox = new QSpinBox(formLayoutWidget);
        sizeSpinBox->setObjectName(QString::fromUtf8("sizeSpinBox"));

        formLayout->setWidget(4, QFormLayout::FieldRole, sizeSpinBox);

        useThemeColorsLabel = new QLabel(formLayoutWidget);
        useThemeColorsLabel->setObjectName(QString::fromUtf8("useThemeColorsLabel"));

        formLayout->setWidget(5, QFormLayout::LabelRole, useThemeColorsLabel);

        useThemeColorsCheckBox = new QCheckBox(formLayoutWidget);
        useThemeColorsCheckBox->setObjectName(QString::fromUtf8("useThemeColorsCheckBox"));

        formLayout->setWidget(5, QFormLayout::FieldRole, useThemeColorsCheckBox);

        colorLabel = new QLabel(formLayoutWidget);
        colorLabel->setObjectName(QString::fromUtf8("colorLabel"));

        formLayout->setWidget(6, QFormLayout::LabelRole, colorLabel);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        colorLineEdit = new QLineEdit(formLayoutWidget);
        colorLineEdit->setObjectName(QString::fromUtf8("colorLineEdit"));

        horizontalLayout->addWidget(colorLineEdit);

        colorToolButton = new QToolButton(formLayoutWidget);
        colorToolButton->setObjectName(QString::fromUtf8("colorToolButton"));

        horizontalLayout->addWidget(colorToolButton);


        formLayout->setLayout(6, QFormLayout::FieldRole, horizontalLayout);

        progressLabel = new QLabel(formLayoutWidget);
        progressLabel->setObjectName(QString::fromUtf8("progressLabel"));

        formLayout->setWidget(2, QFormLayout::LabelRole, progressLabel);

        progressSlider = new QSlider(formLayoutWidget);
        progressSlider->setObjectName(QString::fromUtf8("progressSlider"));
        progressSlider->setOrientation(Qt::Horizontal);

        formLayout->setWidget(2, QFormLayout::FieldRole, progressSlider);


        retranslateUi(CircularProgressSettingsForm);

        QMetaObject::connectSlotsByName(CircularProgressSettingsForm);
    } // setupUi

    void retranslateUi(QWidget *CircularProgressSettingsForm)
    {
        CircularProgressSettingsForm->setWindowTitle(QApplication::translate("CircularProgressSettingsForm", "Form", nullptr));
        disabledLabel->setText(QApplication::translate("CircularProgressSettingsForm", "Disabled", nullptr));
        progressTypeLabel->setText(QApplication::translate("CircularProgressSettingsForm", "Progress type", nullptr));
        progressTypeComboBox->setItemText(0, QApplication::translate("CircularProgressSettingsForm", "Determinate", nullptr));
        progressTypeComboBox->setItemText(1, QApplication::translate("CircularProgressSettingsForm", "Indeterminate", nullptr));

        lineWidthLabel->setText(QApplication::translate("CircularProgressSettingsForm", "Line width", nullptr));
        sizeLabel->setText(QApplication::translate("CircularProgressSettingsForm", "Size", nullptr));
        useThemeColorsLabel->setText(QApplication::translate("CircularProgressSettingsForm", "Use theme colors", nullptr));
        colorLabel->setText(QApplication::translate("CircularProgressSettingsForm", "Color", nullptr));
        colorToolButton->setText(QApplication::translate("CircularProgressSettingsForm", "...", nullptr));
        progressLabel->setText(QApplication::translate("CircularProgressSettingsForm", "Progress", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CircularProgressSettingsForm: public Ui_CircularProgressSettingsForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CIRCULARPROGRESSSETTINGSFORM_H
