/********************************************************************************
** Form generated from reading UI file 'progresssettingsform.ui'
**
** Created by: Qt User Interface Compiler version 5.12.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PROGRESSSETTINGSFORM_H
#define UI_PROGRESSSETTINGSFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QSlider>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ProgressSettingsForm
{
public:
    QWidget *formLayoutWidget;
    QFormLayout *formLayout;
    QLabel *disabledLabel;
    QCheckBox *disabledCheckBox;
    QLabel *progressTypeLabel;
    QComboBox *progressTypeComboBox;
    QLabel *progressLabel;
    QSlider *progressSlider;
    QLabel *useThemeColorsLabel;
    QCheckBox *useThemeColorsCheckBox;
    QLabel *colorLabel;
    QHBoxLayout *horizontalLayout;
    QLineEdit *progressColorLineEdit;
    QToolButton *progressColorToolButton;
    QLabel *backgroundColorLabel;
    QHBoxLayout *horizontalLayout_2;
    QLineEdit *backgroundColorLineEdit;
    QToolButton *backgroundColorToolButton;

    void setupUi(QWidget *ProgressSettingsForm)
    {
        if (ProgressSettingsForm->objectName().isEmpty())
            ProgressSettingsForm->setObjectName(QString::fromUtf8("ProgressSettingsForm"));
        ProgressSettingsForm->resize(400, 300);
        formLayoutWidget = new QWidget(ProgressSettingsForm);
        formLayoutWidget->setObjectName(QString::fromUtf8("formLayoutWidget"));
        formLayoutWidget->setGeometry(QRect(0, 0, 240, 197));
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

        progressLabel = new QLabel(formLayoutWidget);
        progressLabel->setObjectName(QString::fromUtf8("progressLabel"));

        formLayout->setWidget(2, QFormLayout::LabelRole, progressLabel);

        progressSlider = new QSlider(formLayoutWidget);
        progressSlider->setObjectName(QString::fromUtf8("progressSlider"));
        progressSlider->setOrientation(Qt::Horizontal);

        formLayout->setWidget(2, QFormLayout::FieldRole, progressSlider);

        useThemeColorsLabel = new QLabel(formLayoutWidget);
        useThemeColorsLabel->setObjectName(QString::fromUtf8("useThemeColorsLabel"));

        formLayout->setWidget(3, QFormLayout::LabelRole, useThemeColorsLabel);

        useThemeColorsCheckBox = new QCheckBox(formLayoutWidget);
        useThemeColorsCheckBox->setObjectName(QString::fromUtf8("useThemeColorsCheckBox"));

        formLayout->setWidget(3, QFormLayout::FieldRole, useThemeColorsCheckBox);

        colorLabel = new QLabel(formLayoutWidget);
        colorLabel->setObjectName(QString::fromUtf8("colorLabel"));

        formLayout->setWidget(4, QFormLayout::LabelRole, colorLabel);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        progressColorLineEdit = new QLineEdit(formLayoutWidget);
        progressColorLineEdit->setObjectName(QString::fromUtf8("progressColorLineEdit"));

        horizontalLayout->addWidget(progressColorLineEdit);

        progressColorToolButton = new QToolButton(formLayoutWidget);
        progressColorToolButton->setObjectName(QString::fromUtf8("progressColorToolButton"));

        horizontalLayout->addWidget(progressColorToolButton);


        formLayout->setLayout(4, QFormLayout::FieldRole, horizontalLayout);

        backgroundColorLabel = new QLabel(formLayoutWidget);
        backgroundColorLabel->setObjectName(QString::fromUtf8("backgroundColorLabel"));

        formLayout->setWidget(5, QFormLayout::LabelRole, backgroundColorLabel);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        backgroundColorLineEdit = new QLineEdit(formLayoutWidget);
        backgroundColorLineEdit->setObjectName(QString::fromUtf8("backgroundColorLineEdit"));

        horizontalLayout_2->addWidget(backgroundColorLineEdit);

        backgroundColorToolButton = new QToolButton(formLayoutWidget);
        backgroundColorToolButton->setObjectName(QString::fromUtf8("backgroundColorToolButton"));

        horizontalLayout_2->addWidget(backgroundColorToolButton);


        formLayout->setLayout(5, QFormLayout::FieldRole, horizontalLayout_2);


        retranslateUi(ProgressSettingsForm);

        QMetaObject::connectSlotsByName(ProgressSettingsForm);
    } // setupUi

    void retranslateUi(QWidget *ProgressSettingsForm)
    {
        ProgressSettingsForm->setWindowTitle(QApplication::translate("ProgressSettingsForm", "Form", nullptr));
        disabledLabel->setText(QApplication::translate("ProgressSettingsForm", "Disabled", nullptr));
        progressTypeLabel->setText(QApplication::translate("ProgressSettingsForm", "Progress type", nullptr));
        progressTypeComboBox->setItemText(0, QApplication::translate("ProgressSettingsForm", "Determinate", nullptr));
        progressTypeComboBox->setItemText(1, QApplication::translate("ProgressSettingsForm", "Indeterminate", nullptr));

        progressLabel->setText(QApplication::translate("ProgressSettingsForm", "Progress", nullptr));
        useThemeColorsLabel->setText(QApplication::translate("ProgressSettingsForm", "Use theme colors", nullptr));
        colorLabel->setText(QApplication::translate("ProgressSettingsForm", "Progress color", nullptr));
        progressColorToolButton->setText(QApplication::translate("ProgressSettingsForm", "...", nullptr));
        backgroundColorLabel->setText(QApplication::translate("ProgressSettingsForm", "Background color", nullptr));
        backgroundColorToolButton->setText(QApplication::translate("ProgressSettingsForm", "...", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ProgressSettingsForm: public Ui_ProgressSettingsForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PROGRESSSETTINGSFORM_H
