/********************************************************************************
** Form generated from reading UI file 'radiobuttonsettingsform.ui'
**
** Created by: Qt User Interface Compiler version 5.12.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RADIOBUTTONSETTINGSFORM_H
#define UI_RADIOBUTTONSETTINGSFORM_H

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

class Ui_RadioButtonSettingsForm
{
public:
    QWidget *formLayoutWidget;
    QFormLayout *formLayout;
    QLabel *disabledLabel;
    QCheckBox *disabledCheckBox;
    QLabel *labelTextLabel_2;
    QLineEdit *labelTextLineEdit_2;
    QLabel *useThemeColorsLabel_3;
    QCheckBox *useThemeColorsCheckBox_3;
    QLabel *checkedColorLabel_3;
    QLabel *uncheckedColorLabel_3;
    QLabel *textColorLabel_3;
    QLabel *disabledColorLabel_3;
    QLabel *labelPositionLabel_3;
    QComboBox *labelPositionComboBox_2;
    QHBoxLayout *horizontalLayout_5;
    QLineEdit *checkedColorLineEdit_2;
    QToolButton *checkedColorToolButton_2;
    QHBoxLayout *horizontalLayout_6;
    QLineEdit *uncheckedColorLineEdit_2;
    QToolButton *uncheckedColorToolButton_2;
    QHBoxLayout *horizontalLayout_7;
    QLineEdit *textColorLineEdit_2;
    QToolButton *textColorToolButton_2;
    QHBoxLayout *horizontalLayout_8;
    QLineEdit *disabledColorLineEdit_2;
    QToolButton *disabledColorToolButton_2;

    void setupUi(QWidget *RadioButtonSettingsForm)
    {
        if (RadioButtonSettingsForm->objectName().isEmpty())
            RadioButtonSettingsForm->setObjectName(QString::fromUtf8("RadioButtonSettingsForm"));
        RadioButtonSettingsForm->resize(602, 439);
        formLayoutWidget = new QWidget(RadioButtonSettingsForm);
        formLayoutWidget->setObjectName(QString::fromUtf8("formLayoutWidget"));
        formLayoutWidget->setGeometry(QRect(0, 0, 241, 228));
        formLayout = new QFormLayout(formLayoutWidget);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        formLayout->setContentsMargins(0, 0, 0, 0);
        disabledLabel = new QLabel(formLayoutWidget);
        disabledLabel->setObjectName(QString::fromUtf8("disabledLabel"));

        formLayout->setWidget(0, QFormLayout::LabelRole, disabledLabel);

        disabledCheckBox = new QCheckBox(formLayoutWidget);
        disabledCheckBox->setObjectName(QString::fromUtf8("disabledCheckBox"));

        formLayout->setWidget(0, QFormLayout::FieldRole, disabledCheckBox);

        labelTextLabel_2 = new QLabel(formLayoutWidget);
        labelTextLabel_2->setObjectName(QString::fromUtf8("labelTextLabel_2"));

        formLayout->setWidget(2, QFormLayout::LabelRole, labelTextLabel_2);

        labelTextLineEdit_2 = new QLineEdit(formLayoutWidget);
        labelTextLineEdit_2->setObjectName(QString::fromUtf8("labelTextLineEdit_2"));

        formLayout->setWidget(2, QFormLayout::FieldRole, labelTextLineEdit_2);

        useThemeColorsLabel_3 = new QLabel(formLayoutWidget);
        useThemeColorsLabel_3->setObjectName(QString::fromUtf8("useThemeColorsLabel_3"));

        formLayout->setWidget(3, QFormLayout::LabelRole, useThemeColorsLabel_3);

        useThemeColorsCheckBox_3 = new QCheckBox(formLayoutWidget);
        useThemeColorsCheckBox_3->setObjectName(QString::fromUtf8("useThemeColorsCheckBox_3"));

        formLayout->setWidget(3, QFormLayout::FieldRole, useThemeColorsCheckBox_3);

        checkedColorLabel_3 = new QLabel(formLayoutWidget);
        checkedColorLabel_3->setObjectName(QString::fromUtf8("checkedColorLabel_3"));

        formLayout->setWidget(4, QFormLayout::LabelRole, checkedColorLabel_3);

        uncheckedColorLabel_3 = new QLabel(formLayoutWidget);
        uncheckedColorLabel_3->setObjectName(QString::fromUtf8("uncheckedColorLabel_3"));

        formLayout->setWidget(5, QFormLayout::LabelRole, uncheckedColorLabel_3);

        textColorLabel_3 = new QLabel(formLayoutWidget);
        textColorLabel_3->setObjectName(QString::fromUtf8("textColorLabel_3"));

        formLayout->setWidget(6, QFormLayout::LabelRole, textColorLabel_3);

        disabledColorLabel_3 = new QLabel(formLayoutWidget);
        disabledColorLabel_3->setObjectName(QString::fromUtf8("disabledColorLabel_3"));

        formLayout->setWidget(7, QFormLayout::LabelRole, disabledColorLabel_3);

        labelPositionLabel_3 = new QLabel(formLayoutWidget);
        labelPositionLabel_3->setObjectName(QString::fromUtf8("labelPositionLabel_3"));

        formLayout->setWidget(1, QFormLayout::LabelRole, labelPositionLabel_3);

        labelPositionComboBox_2 = new QComboBox(formLayoutWidget);
        labelPositionComboBox_2->addItem(QString());
        labelPositionComboBox_2->addItem(QString());
        labelPositionComboBox_2->setObjectName(QString::fromUtf8("labelPositionComboBox_2"));

        formLayout->setWidget(1, QFormLayout::FieldRole, labelPositionComboBox_2);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        checkedColorLineEdit_2 = new QLineEdit(formLayoutWidget);
        checkedColorLineEdit_2->setObjectName(QString::fromUtf8("checkedColorLineEdit_2"));

        horizontalLayout_5->addWidget(checkedColorLineEdit_2);

        checkedColorToolButton_2 = new QToolButton(formLayoutWidget);
        checkedColorToolButton_2->setObjectName(QString::fromUtf8("checkedColorToolButton_2"));

        horizontalLayout_5->addWidget(checkedColorToolButton_2);


        formLayout->setLayout(4, QFormLayout::FieldRole, horizontalLayout_5);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        uncheckedColorLineEdit_2 = new QLineEdit(formLayoutWidget);
        uncheckedColorLineEdit_2->setObjectName(QString::fromUtf8("uncheckedColorLineEdit_2"));

        horizontalLayout_6->addWidget(uncheckedColorLineEdit_2);

        uncheckedColorToolButton_2 = new QToolButton(formLayoutWidget);
        uncheckedColorToolButton_2->setObjectName(QString::fromUtf8("uncheckedColorToolButton_2"));

        horizontalLayout_6->addWidget(uncheckedColorToolButton_2);


        formLayout->setLayout(5, QFormLayout::FieldRole, horizontalLayout_6);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        textColorLineEdit_2 = new QLineEdit(formLayoutWidget);
        textColorLineEdit_2->setObjectName(QString::fromUtf8("textColorLineEdit_2"));

        horizontalLayout_7->addWidget(textColorLineEdit_2);

        textColorToolButton_2 = new QToolButton(formLayoutWidget);
        textColorToolButton_2->setObjectName(QString::fromUtf8("textColorToolButton_2"));

        horizontalLayout_7->addWidget(textColorToolButton_2);


        formLayout->setLayout(6, QFormLayout::FieldRole, horizontalLayout_7);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        disabledColorLineEdit_2 = new QLineEdit(formLayoutWidget);
        disabledColorLineEdit_2->setObjectName(QString::fromUtf8("disabledColorLineEdit_2"));

        horizontalLayout_8->addWidget(disabledColorLineEdit_2);

        disabledColorToolButton_2 = new QToolButton(formLayoutWidget);
        disabledColorToolButton_2->setObjectName(QString::fromUtf8("disabledColorToolButton_2"));

        horizontalLayout_8->addWidget(disabledColorToolButton_2);


        formLayout->setLayout(7, QFormLayout::FieldRole, horizontalLayout_8);


        retranslateUi(RadioButtonSettingsForm);

        QMetaObject::connectSlotsByName(RadioButtonSettingsForm);
    } // setupUi

    void retranslateUi(QWidget *RadioButtonSettingsForm)
    {
        RadioButtonSettingsForm->setWindowTitle(QApplication::translate("RadioButtonSettingsForm", "Form", nullptr));
        disabledLabel->setText(QApplication::translate("RadioButtonSettingsForm", "Disabled", nullptr));
        labelTextLabel_2->setText(QApplication::translate("RadioButtonSettingsForm", "Label text", nullptr));
        useThemeColorsLabel_3->setText(QApplication::translate("RadioButtonSettingsForm", "Use theme colors", nullptr));
        checkedColorLabel_3->setText(QApplication::translate("RadioButtonSettingsForm", "Checked color", nullptr));
        uncheckedColorLabel_3->setText(QApplication::translate("RadioButtonSettingsForm", "Unchecked color", nullptr));
        textColorLabel_3->setText(QApplication::translate("RadioButtonSettingsForm", "Text color", nullptr));
        disabledColorLabel_3->setText(QApplication::translate("RadioButtonSettingsForm", "Disabled color", nullptr));
        labelPositionLabel_3->setText(QApplication::translate("RadioButtonSettingsForm", "Label position", nullptr));
        labelPositionComboBox_2->setItemText(0, QApplication::translate("RadioButtonSettingsForm", "Left", nullptr));
        labelPositionComboBox_2->setItemText(1, QApplication::translate("RadioButtonSettingsForm", "Right", nullptr));

        checkedColorToolButton_2->setText(QApplication::translate("RadioButtonSettingsForm", "...", nullptr));
        uncheckedColorToolButton_2->setText(QApplication::translate("RadioButtonSettingsForm", "...", nullptr));
        textColorToolButton_2->setText(QApplication::translate("RadioButtonSettingsForm", "...", nullptr));
        disabledColorToolButton_2->setText(QApplication::translate("RadioButtonSettingsForm", "...", nullptr));
    } // retranslateUi

};

namespace Ui {
    class RadioButtonSettingsForm: public Ui_RadioButtonSettingsForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RADIOBUTTONSETTINGSFORM_H
