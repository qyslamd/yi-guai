/********************************************************************************
** Form generated from reading UI file 'avatarsettingsform.ui'
**
** Created by: Qt User Interface Compiler version 5.12.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AVATARSETTINGSFORM_H
#define UI_AVATARSETTINGSFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AvatarSettingsForm
{
public:
    QWidget *formLayoutWidget;
    QFormLayout *formLayout;
    QLabel *useThemeColorsLabel;
    QCheckBox *useThemeColorsCheckBox;
    QLabel *textColorLabel;
    QHBoxLayout *horizontalLayout_4;
    QLineEdit *textColorLineEdit;
    QToolButton *textColorToolButton;
    QLabel *backgroundColorLabel;
    QHBoxLayout *horizontalLayout_5;
    QLineEdit *backgroundColorLineEdit;
    QToolButton *backgroundColorToolButton;
    QLabel *sizeLabel;
    QSpinBox *sizeSpinBox;
    QLabel *typeLabel;
    QComboBox *typeComboBox;
    QLabel *disabledLabel;
    QCheckBox *disabledCheckBox;

    void setupUi(QWidget *AvatarSettingsForm)
    {
        if (AvatarSettingsForm->objectName().isEmpty())
            AvatarSettingsForm->setObjectName(QString::fromUtf8("AvatarSettingsForm"));
        AvatarSettingsForm->resize(400, 300);
        formLayoutWidget = new QWidget(AvatarSettingsForm);
        formLayoutWidget->setObjectName(QString::fromUtf8("formLayoutWidget"));
        formLayoutWidget->setGeometry(QRect(0, 0, 221, 231));
        formLayout = new QFormLayout(formLayoutWidget);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        formLayout->setContentsMargins(0, 0, 0, 0);
        useThemeColorsLabel = new QLabel(formLayoutWidget);
        useThemeColorsLabel->setObjectName(QString::fromUtf8("useThemeColorsLabel"));

        formLayout->setWidget(1, QFormLayout::LabelRole, useThemeColorsLabel);

        useThemeColorsCheckBox = new QCheckBox(formLayoutWidget);
        useThemeColorsCheckBox->setObjectName(QString::fromUtf8("useThemeColorsCheckBox"));

        formLayout->setWidget(1, QFormLayout::FieldRole, useThemeColorsCheckBox);

        textColorLabel = new QLabel(formLayoutWidget);
        textColorLabel->setObjectName(QString::fromUtf8("textColorLabel"));

        formLayout->setWidget(2, QFormLayout::LabelRole, textColorLabel);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        textColorLineEdit = new QLineEdit(formLayoutWidget);
        textColorLineEdit->setObjectName(QString::fromUtf8("textColorLineEdit"));
        textColorLineEdit->setEnabled(false);

        horizontalLayout_4->addWidget(textColorLineEdit);

        textColorToolButton = new QToolButton(formLayoutWidget);
        textColorToolButton->setObjectName(QString::fromUtf8("textColorToolButton"));

        horizontalLayout_4->addWidget(textColorToolButton);


        formLayout->setLayout(2, QFormLayout::FieldRole, horizontalLayout_4);

        backgroundColorLabel = new QLabel(formLayoutWidget);
        backgroundColorLabel->setObjectName(QString::fromUtf8("backgroundColorLabel"));

        formLayout->setWidget(3, QFormLayout::LabelRole, backgroundColorLabel);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        backgroundColorLineEdit = new QLineEdit(formLayoutWidget);
        backgroundColorLineEdit->setObjectName(QString::fromUtf8("backgroundColorLineEdit"));
        backgroundColorLineEdit->setEnabled(false);

        horizontalLayout_5->addWidget(backgroundColorLineEdit);

        backgroundColorToolButton = new QToolButton(formLayoutWidget);
        backgroundColorToolButton->setObjectName(QString::fromUtf8("backgroundColorToolButton"));

        horizontalLayout_5->addWidget(backgroundColorToolButton);


        formLayout->setLayout(3, QFormLayout::FieldRole, horizontalLayout_5);

        sizeLabel = new QLabel(formLayoutWidget);
        sizeLabel->setObjectName(QString::fromUtf8("sizeLabel"));

        formLayout->setWidget(4, QFormLayout::LabelRole, sizeLabel);

        sizeSpinBox = new QSpinBox(formLayoutWidget);
        sizeSpinBox->setObjectName(QString::fromUtf8("sizeSpinBox"));

        formLayout->setWidget(4, QFormLayout::FieldRole, sizeSpinBox);

        typeLabel = new QLabel(formLayoutWidget);
        typeLabel->setObjectName(QString::fromUtf8("typeLabel"));

        formLayout->setWidget(5, QFormLayout::LabelRole, typeLabel);

        typeComboBox = new QComboBox(formLayoutWidget);
        typeComboBox->addItem(QString());
        typeComboBox->addItem(QString());
        typeComboBox->addItem(QString());
        typeComboBox->setObjectName(QString::fromUtf8("typeComboBox"));

        formLayout->setWidget(5, QFormLayout::FieldRole, typeComboBox);

        disabledLabel = new QLabel(formLayoutWidget);
        disabledLabel->setObjectName(QString::fromUtf8("disabledLabel"));

        formLayout->setWidget(0, QFormLayout::LabelRole, disabledLabel);

        disabledCheckBox = new QCheckBox(formLayoutWidget);
        disabledCheckBox->setObjectName(QString::fromUtf8("disabledCheckBox"));

        formLayout->setWidget(0, QFormLayout::FieldRole, disabledCheckBox);


        retranslateUi(AvatarSettingsForm);

        QMetaObject::connectSlotsByName(AvatarSettingsForm);
    } // setupUi

    void retranslateUi(QWidget *AvatarSettingsForm)
    {
        AvatarSettingsForm->setWindowTitle(QApplication::translate("AvatarSettingsForm", "Form", nullptr));
        useThemeColorsLabel->setText(QApplication::translate("AvatarSettingsForm", "Use theme colors", nullptr));
        textColorLabel->setText(QApplication::translate("AvatarSettingsForm", "Text color", nullptr));
        textColorToolButton->setText(QApplication::translate("AvatarSettingsForm", "...", nullptr));
        backgroundColorLabel->setText(QApplication::translate("AvatarSettingsForm", "Background color", nullptr));
        backgroundColorToolButton->setText(QApplication::translate("AvatarSettingsForm", "...", nullptr));
        sizeLabel->setText(QApplication::translate("AvatarSettingsForm", "Size", nullptr));
        typeLabel->setText(QApplication::translate("AvatarSettingsForm", "Type", nullptr));
        typeComboBox->setItemText(0, QApplication::translate("AvatarSettingsForm", "Letter", nullptr));
        typeComboBox->setItemText(1, QApplication::translate("AvatarSettingsForm", "Image", nullptr));
        typeComboBox->setItemText(2, QApplication::translate("AvatarSettingsForm", "Icon", nullptr));

        disabledLabel->setText(QApplication::translate("AvatarSettingsForm", "Disabled", nullptr));
    } // retranslateUi

};

namespace Ui {
    class AvatarSettingsForm: public Ui_AvatarSettingsForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AVATARSETTINGSFORM_H
