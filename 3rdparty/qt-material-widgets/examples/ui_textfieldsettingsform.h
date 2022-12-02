/********************************************************************************
** Form generated from reading UI file 'textfieldsettingsform.ui'
**
** Created by: Qt User Interface Compiler version 5.12.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TEXTFIELDSETTINGSFORM_H
#define UI_TEXTFIELDSETTINGSFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TextFieldSettingsForm
{
public:
    QWidget *formLayoutWidget;
    QFormLayout *formLayout;
    QLabel *disabledLabel;
    QCheckBox *disabledCheckBox;
    QLabel *textLabel;
    QLineEdit *textLineEdit;
    QLabel *placeholderLabel;
    QLineEdit *placeholderLineEdit;
    QLabel *labelLabel_2;
    QCheckBox *labelCheckBox;
    QLabel *labelLabel;
    QLineEdit *labelTextLineEdit;
    QLabel *useThemeColorsLabel;
    QCheckBox *useThemeColorsCheckBox;
    QLabel *textColorLabel;
    QHBoxLayout *horizontalLayout;
    QLineEdit *textColorLineEdit;
    QToolButton *textColorToolButton;
    QLabel *inkColorLabel;
    QHBoxLayout *horizontalLayout_4;
    QLineEdit *inkColorLineEdit;
    QToolButton *inkColorToolButton;
    QLabel *inputLineColorLabel;
    QHBoxLayout *horizontalLayout_6;
    QLineEdit *inputLineColorLineEdit;
    QToolButton *inputLineColorToolButton;
    QLabel *labelColorLabel;
    QHBoxLayout *horizontalLayout_7;
    QLineEdit *labelColorLineEdit;
    QToolButton *labelColorToolButton;
    QLabel *labelShowInputLine;
    QCheckBox *inputLineCheckBox;

    void setupUi(QWidget *TextFieldSettingsForm)
    {
        if (TextFieldSettingsForm->objectName().isEmpty())
            TextFieldSettingsForm->setObjectName(QString::fromUtf8("TextFieldSettingsForm"));
        TextFieldSettingsForm->resize(400, 416);
        formLayoutWidget = new QWidget(TextFieldSettingsForm);
        formLayoutWidget->setObjectName(QString::fromUtf8("formLayoutWidget"));
        formLayoutWidget->setGeometry(QRect(0, 0, 321, 351));
        formLayout = new QFormLayout(formLayoutWidget);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        formLayout->setContentsMargins(0, 0, 0, 0);
        disabledLabel = new QLabel(formLayoutWidget);
        disabledLabel->setObjectName(QString::fromUtf8("disabledLabel"));

        formLayout->setWidget(0, QFormLayout::LabelRole, disabledLabel);

        disabledCheckBox = new QCheckBox(formLayoutWidget);
        disabledCheckBox->setObjectName(QString::fromUtf8("disabledCheckBox"));

        formLayout->setWidget(0, QFormLayout::FieldRole, disabledCheckBox);

        textLabel = new QLabel(formLayoutWidget);
        textLabel->setObjectName(QString::fromUtf8("textLabel"));

        formLayout->setWidget(1, QFormLayout::LabelRole, textLabel);

        textLineEdit = new QLineEdit(formLayoutWidget);
        textLineEdit->setObjectName(QString::fromUtf8("textLineEdit"));

        formLayout->setWidget(1, QFormLayout::FieldRole, textLineEdit);

        placeholderLabel = new QLabel(formLayoutWidget);
        placeholderLabel->setObjectName(QString::fromUtf8("placeholderLabel"));

        formLayout->setWidget(2, QFormLayout::LabelRole, placeholderLabel);

        placeholderLineEdit = new QLineEdit(formLayoutWidget);
        placeholderLineEdit->setObjectName(QString::fromUtf8("placeholderLineEdit"));

        formLayout->setWidget(2, QFormLayout::FieldRole, placeholderLineEdit);

        labelLabel_2 = new QLabel(formLayoutWidget);
        labelLabel_2->setObjectName(QString::fromUtf8("labelLabel_2"));

        formLayout->setWidget(3, QFormLayout::LabelRole, labelLabel_2);

        labelCheckBox = new QCheckBox(formLayoutWidget);
        labelCheckBox->setObjectName(QString::fromUtf8("labelCheckBox"));

        formLayout->setWidget(3, QFormLayout::FieldRole, labelCheckBox);

        labelLabel = new QLabel(formLayoutWidget);
        labelLabel->setObjectName(QString::fromUtf8("labelLabel"));

        formLayout->setWidget(4, QFormLayout::LabelRole, labelLabel);

        labelTextLineEdit = new QLineEdit(formLayoutWidget);
        labelTextLineEdit->setObjectName(QString::fromUtf8("labelTextLineEdit"));

        formLayout->setWidget(4, QFormLayout::FieldRole, labelTextLineEdit);

        useThemeColorsLabel = new QLabel(formLayoutWidget);
        useThemeColorsLabel->setObjectName(QString::fromUtf8("useThemeColorsLabel"));

        formLayout->setWidget(5, QFormLayout::LabelRole, useThemeColorsLabel);

        useThemeColorsCheckBox = new QCheckBox(formLayoutWidget);
        useThemeColorsCheckBox->setObjectName(QString::fromUtf8("useThemeColorsCheckBox"));

        formLayout->setWidget(5, QFormLayout::FieldRole, useThemeColorsCheckBox);

        textColorLabel = new QLabel(formLayoutWidget);
        textColorLabel->setObjectName(QString::fromUtf8("textColorLabel"));

        formLayout->setWidget(6, QFormLayout::LabelRole, textColorLabel);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        textColorLineEdit = new QLineEdit(formLayoutWidget);
        textColorLineEdit->setObjectName(QString::fromUtf8("textColorLineEdit"));

        horizontalLayout->addWidget(textColorLineEdit);

        textColorToolButton = new QToolButton(formLayoutWidget);
        textColorToolButton->setObjectName(QString::fromUtf8("textColorToolButton"));

        horizontalLayout->addWidget(textColorToolButton);


        formLayout->setLayout(6, QFormLayout::FieldRole, horizontalLayout);

        inkColorLabel = new QLabel(formLayoutWidget);
        inkColorLabel->setObjectName(QString::fromUtf8("inkColorLabel"));

        formLayout->setWidget(7, QFormLayout::LabelRole, inkColorLabel);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        inkColorLineEdit = new QLineEdit(formLayoutWidget);
        inkColorLineEdit->setObjectName(QString::fromUtf8("inkColorLineEdit"));

        horizontalLayout_4->addWidget(inkColorLineEdit);

        inkColorToolButton = new QToolButton(formLayoutWidget);
        inkColorToolButton->setObjectName(QString::fromUtf8("inkColorToolButton"));

        horizontalLayout_4->addWidget(inkColorToolButton);


        formLayout->setLayout(7, QFormLayout::FieldRole, horizontalLayout_4);

        inputLineColorLabel = new QLabel(formLayoutWidget);
        inputLineColorLabel->setObjectName(QString::fromUtf8("inputLineColorLabel"));

        formLayout->setWidget(8, QFormLayout::LabelRole, inputLineColorLabel);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        inputLineColorLineEdit = new QLineEdit(formLayoutWidget);
        inputLineColorLineEdit->setObjectName(QString::fromUtf8("inputLineColorLineEdit"));

        horizontalLayout_6->addWidget(inputLineColorLineEdit);

        inputLineColorToolButton = new QToolButton(formLayoutWidget);
        inputLineColorToolButton->setObjectName(QString::fromUtf8("inputLineColorToolButton"));

        horizontalLayout_6->addWidget(inputLineColorToolButton);


        formLayout->setLayout(8, QFormLayout::FieldRole, horizontalLayout_6);

        labelColorLabel = new QLabel(formLayoutWidget);
        labelColorLabel->setObjectName(QString::fromUtf8("labelColorLabel"));

        formLayout->setWidget(9, QFormLayout::LabelRole, labelColorLabel);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        labelColorLineEdit = new QLineEdit(formLayoutWidget);
        labelColorLineEdit->setObjectName(QString::fromUtf8("labelColorLineEdit"));

        horizontalLayout_7->addWidget(labelColorLineEdit);

        labelColorToolButton = new QToolButton(formLayoutWidget);
        labelColorToolButton->setObjectName(QString::fromUtf8("labelColorToolButton"));

        horizontalLayout_7->addWidget(labelColorToolButton);


        formLayout->setLayout(9, QFormLayout::FieldRole, horizontalLayout_7);

        labelShowInputLine = new QLabel(formLayoutWidget);
        labelShowInputLine->setObjectName(QString::fromUtf8("labelShowInputLine"));

        formLayout->setWidget(10, QFormLayout::LabelRole, labelShowInputLine);

        inputLineCheckBox = new QCheckBox(formLayoutWidget);
        inputLineCheckBox->setObjectName(QString::fromUtf8("inputLineCheckBox"));

        formLayout->setWidget(10, QFormLayout::FieldRole, inputLineCheckBox);


        retranslateUi(TextFieldSettingsForm);

        QMetaObject::connectSlotsByName(TextFieldSettingsForm);
    } // setupUi

    void retranslateUi(QWidget *TextFieldSettingsForm)
    {
        TextFieldSettingsForm->setWindowTitle(QApplication::translate("TextFieldSettingsForm", "Form", nullptr));
        disabledLabel->setText(QApplication::translate("TextFieldSettingsForm", "Disabled", nullptr));
        textLabel->setText(QApplication::translate("TextFieldSettingsForm", "Text", nullptr));
        placeholderLabel->setText(QApplication::translate("TextFieldSettingsForm", "Placeholder", nullptr));
        labelLabel_2->setText(QApplication::translate("TextFieldSettingsForm", "Label", nullptr));
        labelLabel->setText(QApplication::translate("TextFieldSettingsForm", "Label text", nullptr));
        useThemeColorsLabel->setText(QApplication::translate("TextFieldSettingsForm", "Use theme colors", nullptr));
        textColorLabel->setText(QApplication::translate("TextFieldSettingsForm", "Text color", nullptr));
        textColorToolButton->setText(QApplication::translate("TextFieldSettingsForm", "...", nullptr));
        inkColorLabel->setText(QApplication::translate("TextFieldSettingsForm", "Ink color", nullptr));
        inkColorToolButton->setText(QApplication::translate("TextFieldSettingsForm", "...", nullptr));
        inputLineColorLabel->setText(QApplication::translate("TextFieldSettingsForm", "Input line color", nullptr));
        inputLineColorToolButton->setText(QApplication::translate("TextFieldSettingsForm", "...", nullptr));
        labelColorLabel->setText(QApplication::translate("TextFieldSettingsForm", "Label color", nullptr));
        labelColorToolButton->setText(QApplication::translate("TextFieldSettingsForm", "...", nullptr));
        labelShowInputLine->setText(QApplication::translate("TextFieldSettingsForm", "Input line", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TextFieldSettingsForm: public Ui_TextFieldSettingsForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TEXTFIELDSETTINGSFORM_H
