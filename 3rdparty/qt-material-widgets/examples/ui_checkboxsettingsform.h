/********************************************************************************
** Form generated from reading UI file 'checkboxsettingsform.ui'
**
** Created by: Qt User Interface Compiler version 5.12.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CHECKBOXSETTINGSFORM_H
#define UI_CHECKBOXSETTINGSFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CheckBoxSettingsForm
{
public:
    QWidget *formLayoutWidget;
    QFormLayout *formLayout;
    QLabel *disabledLabel;
    QCheckBox *disabledCheckBox;
    QLabel *labelPositionLabel;
    QComboBox *labelPositionComboBox;
    QLabel *labelTextLabel;
    QLineEdit *labelTextLineEdit;
    QLabel *checkedLabel;
    QCheckBox *checkedCheckBox;
    QLabel *useThemeColorsLabel;
    QCheckBox *useThemeColorsCheckBox;
    QLabel *checkedColorLabel;
    QHBoxLayout *horizontalLayout;
    QLineEdit *checkedColorLineEdit;
    QToolButton *checkedColorToolButton;
    QLabel *uncheckedColorLabel;
    QHBoxLayout *horizontalLayout_2;
    QLineEdit *uncheckedColorLineEdit;
    QToolButton *uncheckedColorToolButton;
    QLabel *textColorLabel;
    QLabel *disabledColorLabel;
    QHBoxLayout *horizontalLayout_3;
    QLineEdit *textColorLineEdit;
    QToolButton *textColorToolButton;
    QHBoxLayout *horizontalLayout_4;
    QLineEdit *disabledColorLineEdit;
    QToolButton *disabledColorToolButton;

    void setupUi(QWidget *CheckBoxSettingsForm)
    {
        if (CheckBoxSettingsForm->objectName().isEmpty())
            CheckBoxSettingsForm->setObjectName(QString::fromUtf8("CheckBoxSettingsForm"));
        CheckBoxSettingsForm->resize(400, 300);
        formLayoutWidget = new QWidget(CheckBoxSettingsForm);
        formLayoutWidget->setObjectName(QString::fromUtf8("formLayoutWidget"));
        formLayoutWidget->setGeometry(QRect(0, 0, 301, 281));
        formLayout = new QFormLayout(formLayoutWidget);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        formLayout->setContentsMargins(0, 0, 0, 0);
        disabledLabel = new QLabel(formLayoutWidget);
        disabledLabel->setObjectName(QString::fromUtf8("disabledLabel"));

        formLayout->setWidget(0, QFormLayout::LabelRole, disabledLabel);

        disabledCheckBox = new QCheckBox(formLayoutWidget);
        disabledCheckBox->setObjectName(QString::fromUtf8("disabledCheckBox"));

        formLayout->setWidget(0, QFormLayout::FieldRole, disabledCheckBox);

        labelPositionLabel = new QLabel(formLayoutWidget);
        labelPositionLabel->setObjectName(QString::fromUtf8("labelPositionLabel"));

        formLayout->setWidget(1, QFormLayout::LabelRole, labelPositionLabel);

        labelPositionComboBox = new QComboBox(formLayoutWidget);
        labelPositionComboBox->addItem(QString());
        labelPositionComboBox->addItem(QString());
        labelPositionComboBox->setObjectName(QString::fromUtf8("labelPositionComboBox"));

        formLayout->setWidget(1, QFormLayout::FieldRole, labelPositionComboBox);

        labelTextLabel = new QLabel(formLayoutWidget);
        labelTextLabel->setObjectName(QString::fromUtf8("labelTextLabel"));

        formLayout->setWidget(2, QFormLayout::LabelRole, labelTextLabel);

        labelTextLineEdit = new QLineEdit(formLayoutWidget);
        labelTextLineEdit->setObjectName(QString::fromUtf8("labelTextLineEdit"));

        formLayout->setWidget(2, QFormLayout::FieldRole, labelTextLineEdit);

        checkedLabel = new QLabel(formLayoutWidget);
        checkedLabel->setObjectName(QString::fromUtf8("checkedLabel"));

        formLayout->setWidget(3, QFormLayout::LabelRole, checkedLabel);

        checkedCheckBox = new QCheckBox(formLayoutWidget);
        checkedCheckBox->setObjectName(QString::fromUtf8("checkedCheckBox"));

        formLayout->setWidget(3, QFormLayout::FieldRole, checkedCheckBox);

        useThemeColorsLabel = new QLabel(formLayoutWidget);
        useThemeColorsLabel->setObjectName(QString::fromUtf8("useThemeColorsLabel"));

        formLayout->setWidget(4, QFormLayout::LabelRole, useThemeColorsLabel);

        useThemeColorsCheckBox = new QCheckBox(formLayoutWidget);
        useThemeColorsCheckBox->setObjectName(QString::fromUtf8("useThemeColorsCheckBox"));

        formLayout->setWidget(4, QFormLayout::FieldRole, useThemeColorsCheckBox);

        checkedColorLabel = new QLabel(formLayoutWidget);
        checkedColorLabel->setObjectName(QString::fromUtf8("checkedColorLabel"));

        formLayout->setWidget(5, QFormLayout::LabelRole, checkedColorLabel);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        checkedColorLineEdit = new QLineEdit(formLayoutWidget);
        checkedColorLineEdit->setObjectName(QString::fromUtf8("checkedColorLineEdit"));

        horizontalLayout->addWidget(checkedColorLineEdit);

        checkedColorToolButton = new QToolButton(formLayoutWidget);
        checkedColorToolButton->setObjectName(QString::fromUtf8("checkedColorToolButton"));

        horizontalLayout->addWidget(checkedColorToolButton);


        formLayout->setLayout(5, QFormLayout::FieldRole, horizontalLayout);

        uncheckedColorLabel = new QLabel(formLayoutWidget);
        uncheckedColorLabel->setObjectName(QString::fromUtf8("uncheckedColorLabel"));

        formLayout->setWidget(6, QFormLayout::LabelRole, uncheckedColorLabel);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        uncheckedColorLineEdit = new QLineEdit(formLayoutWidget);
        uncheckedColorLineEdit->setObjectName(QString::fromUtf8("uncheckedColorLineEdit"));

        horizontalLayout_2->addWidget(uncheckedColorLineEdit);

        uncheckedColorToolButton = new QToolButton(formLayoutWidget);
        uncheckedColorToolButton->setObjectName(QString::fromUtf8("uncheckedColorToolButton"));

        horizontalLayout_2->addWidget(uncheckedColorToolButton);


        formLayout->setLayout(6, QFormLayout::FieldRole, horizontalLayout_2);

        textColorLabel = new QLabel(formLayoutWidget);
        textColorLabel->setObjectName(QString::fromUtf8("textColorLabel"));

        formLayout->setWidget(7, QFormLayout::LabelRole, textColorLabel);

        disabledColorLabel = new QLabel(formLayoutWidget);
        disabledColorLabel->setObjectName(QString::fromUtf8("disabledColorLabel"));

        formLayout->setWidget(8, QFormLayout::LabelRole, disabledColorLabel);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        textColorLineEdit = new QLineEdit(formLayoutWidget);
        textColorLineEdit->setObjectName(QString::fromUtf8("textColorLineEdit"));

        horizontalLayout_3->addWidget(textColorLineEdit);

        textColorToolButton = new QToolButton(formLayoutWidget);
        textColorToolButton->setObjectName(QString::fromUtf8("textColorToolButton"));

        horizontalLayout_3->addWidget(textColorToolButton);


        formLayout->setLayout(7, QFormLayout::FieldRole, horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        disabledColorLineEdit = new QLineEdit(formLayoutWidget);
        disabledColorLineEdit->setObjectName(QString::fromUtf8("disabledColorLineEdit"));

        horizontalLayout_4->addWidget(disabledColorLineEdit);

        disabledColorToolButton = new QToolButton(formLayoutWidget);
        disabledColorToolButton->setObjectName(QString::fromUtf8("disabledColorToolButton"));

        horizontalLayout_4->addWidget(disabledColorToolButton);


        formLayout->setLayout(8, QFormLayout::FieldRole, horizontalLayout_4);


        retranslateUi(CheckBoxSettingsForm);

        QMetaObject::connectSlotsByName(CheckBoxSettingsForm);
    } // setupUi

    void retranslateUi(QWidget *CheckBoxSettingsForm)
    {
        CheckBoxSettingsForm->setWindowTitle(QApplication::translate("CheckBoxSettingsForm", "Form", nullptr));
        disabledLabel->setText(QApplication::translate("CheckBoxSettingsForm", "Disabled", nullptr));
        labelPositionLabel->setText(QApplication::translate("CheckBoxSettingsForm", "Label position", nullptr));
        labelPositionComboBox->setItemText(0, QApplication::translate("CheckBoxSettingsForm", "Left", nullptr));
        labelPositionComboBox->setItemText(1, QApplication::translate("CheckBoxSettingsForm", "Right", nullptr));

        labelTextLabel->setText(QApplication::translate("CheckBoxSettingsForm", "Label text", nullptr));
        checkedLabel->setText(QApplication::translate("CheckBoxSettingsForm", "Checked", nullptr));
        useThemeColorsLabel->setText(QApplication::translate("CheckBoxSettingsForm", "Use theme colors", nullptr));
        checkedColorLabel->setText(QApplication::translate("CheckBoxSettingsForm", "Checked color", nullptr));
        checkedColorToolButton->setText(QApplication::translate("CheckBoxSettingsForm", "...", nullptr));
        uncheckedColorLabel->setText(QApplication::translate("CheckBoxSettingsForm", "Unchecked color", nullptr));
        uncheckedColorToolButton->setText(QApplication::translate("CheckBoxSettingsForm", "...", nullptr));
        textColorLabel->setText(QApplication::translate("CheckBoxSettingsForm", "Text color", nullptr));
        disabledColorLabel->setText(QApplication::translate("CheckBoxSettingsForm", "Disabled color", nullptr));
        textColorToolButton->setText(QApplication::translate("CheckBoxSettingsForm", "...", nullptr));
        disabledColorToolButton->setText(QApplication::translate("CheckBoxSettingsForm", "...", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CheckBoxSettingsForm: public Ui_CheckBoxSettingsForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHECKBOXSETTINGSFORM_H
