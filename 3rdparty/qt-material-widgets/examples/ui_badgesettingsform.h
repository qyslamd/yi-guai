/********************************************************************************
** Form generated from reading UI file 'badgesettingsform.ui'
**
** Created by: Qt User Interface Compiler version 5.12.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BADGESETTINGSFORM_H
#define UI_BADGESETTINGSFORM_H

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

class Ui_BadgeSettingsForm
{
public:
    QWidget *formLayoutWidget;
    QFormLayout *formLayout;
    QLabel *disabledLabel;
    QCheckBox *disabledCheckBox;
    QLabel *useThemeColorsLabel;
    QCheckBox *useThemeColorsCheckBox;
    QLabel *backgroundColorLabel;
    QLabel *textColorLabel;
    QLabel *horizontalOffsetLabel;
    QSpinBox *horizontalOffsetSpinBox;
    QLabel *verticalOffsetLabel;
    QSpinBox *verticalOffsetSpinBox;
    QLabel *typeLabel;
    QComboBox *typeComboBox;
    QHBoxLayout *horizontalLayout;
    QLineEdit *backgroundColorLineEdit;
    QToolButton *backgroundColorToolButton;
    QHBoxLayout *horizontalLayout_2;
    QLineEdit *textColorLineEdit;
    QToolButton *textColorToolButton;

    void setupUi(QWidget *BadgeSettingsForm)
    {
        if (BadgeSettingsForm->objectName().isEmpty())
            BadgeSettingsForm->setObjectName(QString::fromUtf8("BadgeSettingsForm"));
        BadgeSettingsForm->resize(400, 300);
        formLayoutWidget = new QWidget(BadgeSettingsForm);
        formLayoutWidget->setObjectName(QString::fromUtf8("formLayoutWidget"));
        formLayoutWidget->setGeometry(QRect(0, 0, 201, 231));
        formLayout = new QFormLayout(formLayoutWidget);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        formLayout->setContentsMargins(0, 0, 0, 0);
        disabledLabel = new QLabel(formLayoutWidget);
        disabledLabel->setObjectName(QString::fromUtf8("disabledLabel"));

        formLayout->setWidget(0, QFormLayout::LabelRole, disabledLabel);

        disabledCheckBox = new QCheckBox(formLayoutWidget);
        disabledCheckBox->setObjectName(QString::fromUtf8("disabledCheckBox"));

        formLayout->setWidget(0, QFormLayout::FieldRole, disabledCheckBox);

        useThemeColorsLabel = new QLabel(formLayoutWidget);
        useThemeColorsLabel->setObjectName(QString::fromUtf8("useThemeColorsLabel"));

        formLayout->setWidget(1, QFormLayout::LabelRole, useThemeColorsLabel);

        useThemeColorsCheckBox = new QCheckBox(formLayoutWidget);
        useThemeColorsCheckBox->setObjectName(QString::fromUtf8("useThemeColorsCheckBox"));

        formLayout->setWidget(1, QFormLayout::FieldRole, useThemeColorsCheckBox);

        backgroundColorLabel = new QLabel(formLayoutWidget);
        backgroundColorLabel->setObjectName(QString::fromUtf8("backgroundColorLabel"));

        formLayout->setWidget(2, QFormLayout::LabelRole, backgroundColorLabel);

        textColorLabel = new QLabel(formLayoutWidget);
        textColorLabel->setObjectName(QString::fromUtf8("textColorLabel"));

        formLayout->setWidget(3, QFormLayout::LabelRole, textColorLabel);

        horizontalOffsetLabel = new QLabel(formLayoutWidget);
        horizontalOffsetLabel->setObjectName(QString::fromUtf8("horizontalOffsetLabel"));

        formLayout->setWidget(4, QFormLayout::LabelRole, horizontalOffsetLabel);

        horizontalOffsetSpinBox = new QSpinBox(formLayoutWidget);
        horizontalOffsetSpinBox->setObjectName(QString::fromUtf8("horizontalOffsetSpinBox"));

        formLayout->setWidget(4, QFormLayout::FieldRole, horizontalOffsetSpinBox);

        verticalOffsetLabel = new QLabel(formLayoutWidget);
        verticalOffsetLabel->setObjectName(QString::fromUtf8("verticalOffsetLabel"));

        formLayout->setWidget(5, QFormLayout::LabelRole, verticalOffsetLabel);

        verticalOffsetSpinBox = new QSpinBox(formLayoutWidget);
        verticalOffsetSpinBox->setObjectName(QString::fromUtf8("verticalOffsetSpinBox"));

        formLayout->setWidget(5, QFormLayout::FieldRole, verticalOffsetSpinBox);

        typeLabel = new QLabel(formLayoutWidget);
        typeLabel->setObjectName(QString::fromUtf8("typeLabel"));

        formLayout->setWidget(6, QFormLayout::LabelRole, typeLabel);

        typeComboBox = new QComboBox(formLayoutWidget);
        typeComboBox->addItem(QString());
        typeComboBox->addItem(QString());
        typeComboBox->setObjectName(QString::fromUtf8("typeComboBox"));

        formLayout->setWidget(6, QFormLayout::FieldRole, typeComboBox);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        backgroundColorLineEdit = new QLineEdit(formLayoutWidget);
        backgroundColorLineEdit->setObjectName(QString::fromUtf8("backgroundColorLineEdit"));
        backgroundColorLineEdit->setEnabled(false);

        horizontalLayout->addWidget(backgroundColorLineEdit);

        backgroundColorToolButton = new QToolButton(formLayoutWidget);
        backgroundColorToolButton->setObjectName(QString::fromUtf8("backgroundColorToolButton"));

        horizontalLayout->addWidget(backgroundColorToolButton);


        formLayout->setLayout(2, QFormLayout::FieldRole, horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        textColorLineEdit = new QLineEdit(formLayoutWidget);
        textColorLineEdit->setObjectName(QString::fromUtf8("textColorLineEdit"));
        textColorLineEdit->setEnabled(false);

        horizontalLayout_2->addWidget(textColorLineEdit);

        textColorToolButton = new QToolButton(formLayoutWidget);
        textColorToolButton->setObjectName(QString::fromUtf8("textColorToolButton"));

        horizontalLayout_2->addWidget(textColorToolButton);


        formLayout->setLayout(3, QFormLayout::FieldRole, horizontalLayout_2);


        retranslateUi(BadgeSettingsForm);

        QMetaObject::connectSlotsByName(BadgeSettingsForm);
    } // setupUi

    void retranslateUi(QWidget *BadgeSettingsForm)
    {
        BadgeSettingsForm->setWindowTitle(QApplication::translate("BadgeSettingsForm", "Form", nullptr));
        disabledLabel->setText(QApplication::translate("BadgeSettingsForm", "Disabled", nullptr));
        useThemeColorsLabel->setText(QApplication::translate("BadgeSettingsForm", "Use theme colors", nullptr));
        backgroundColorLabel->setText(QApplication::translate("BadgeSettingsForm", "Background color", nullptr));
        textColorLabel->setText(QApplication::translate("BadgeSettingsForm", "Text color", nullptr));
        horizontalOffsetLabel->setText(QApplication::translate("BadgeSettingsForm", "Horizontal offset", nullptr));
        verticalOffsetLabel->setText(QApplication::translate("BadgeSettingsForm", "Vertical offset", nullptr));
        typeLabel->setText(QApplication::translate("BadgeSettingsForm", "Type", nullptr));
        typeComboBox->setItemText(0, QApplication::translate("BadgeSettingsForm", "Text", nullptr));
        typeComboBox->setItemText(1, QApplication::translate("BadgeSettingsForm", "Icon", nullptr));

        backgroundColorToolButton->setText(QApplication::translate("BadgeSettingsForm", "...", nullptr));
        textColorToolButton->setText(QApplication::translate("BadgeSettingsForm", "...", nullptr));
    } // retranslateUi

};

namespace Ui {
    class BadgeSettingsForm: public Ui_BadgeSettingsForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BADGESETTINGSFORM_H
