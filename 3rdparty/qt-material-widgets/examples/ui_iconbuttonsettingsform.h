/********************************************************************************
** Form generated from reading UI file 'iconbuttonsettingsform.ui'
**
** Created by: Qt User Interface Compiler version 5.12.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ICONBUTTONSETTINGSFORM_H
#define UI_ICONBUTTONSETTINGSFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_IconButtonSettingsForm
{
public:
    QWidget *formLayoutWidget;
    QFormLayout *formLayout;
    QLabel *disabledLabel;
    QCheckBox *disabledCheckBox;
    QLabel *sizeLabel;
    QSpinBox *sizeSpinBox;
    QLabel *useThemeColorsLabel;
    QCheckBox *useThemeColorsCheckBox;
    QLabel *colorLabel;
    QHBoxLayout *horizontalLayout_2;
    QLineEdit *colorLineEdit;
    QToolButton *colorToolButton;
    QLabel *disabledColorLabel;
    QHBoxLayout *horizontalLayout_4;
    QLineEdit *disabledColorLineEdit;
    QToolButton *disabledColorToolButton;

    void setupUi(QWidget *IconButtonSettingsForm)
    {
        if (IconButtonSettingsForm->objectName().isEmpty())
            IconButtonSettingsForm->setObjectName(QString::fromUtf8("IconButtonSettingsForm"));
        IconButtonSettingsForm->resize(599, 418);
        formLayoutWidget = new QWidget(IconButtonSettingsForm);
        formLayoutWidget->setObjectName(QString::fromUtf8("formLayoutWidget"));
        formLayoutWidget->setGeometry(QRect(0, 0, 274, 371));
        formLayout = new QFormLayout(formLayoutWidget);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        formLayout->setFormAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        formLayout->setContentsMargins(0, 0, 0, 0);
        disabledLabel = new QLabel(formLayoutWidget);
        disabledLabel->setObjectName(QString::fromUtf8("disabledLabel"));

        formLayout->setWidget(0, QFormLayout::LabelRole, disabledLabel);

        disabledCheckBox = new QCheckBox(formLayoutWidget);
        disabledCheckBox->setObjectName(QString::fromUtf8("disabledCheckBox"));

        formLayout->setWidget(0, QFormLayout::FieldRole, disabledCheckBox);

        sizeLabel = new QLabel(formLayoutWidget);
        sizeLabel->setObjectName(QString::fromUtf8("sizeLabel"));

        formLayout->setWidget(1, QFormLayout::LabelRole, sizeLabel);

        sizeSpinBox = new QSpinBox(formLayoutWidget);
        sizeSpinBox->setObjectName(QString::fromUtf8("sizeSpinBox"));

        formLayout->setWidget(1, QFormLayout::FieldRole, sizeSpinBox);

        useThemeColorsLabel = new QLabel(formLayoutWidget);
        useThemeColorsLabel->setObjectName(QString::fromUtf8("useThemeColorsLabel"));

        formLayout->setWidget(2, QFormLayout::LabelRole, useThemeColorsLabel);

        useThemeColorsCheckBox = new QCheckBox(formLayoutWidget);
        useThemeColorsCheckBox->setObjectName(QString::fromUtf8("useThemeColorsCheckBox"));

        formLayout->setWidget(2, QFormLayout::FieldRole, useThemeColorsCheckBox);

        colorLabel = new QLabel(formLayoutWidget);
        colorLabel->setObjectName(QString::fromUtf8("colorLabel"));

        formLayout->setWidget(3, QFormLayout::LabelRole, colorLabel);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        colorLineEdit = new QLineEdit(formLayoutWidget);
        colorLineEdit->setObjectName(QString::fromUtf8("colorLineEdit"));
        colorLineEdit->setEnabled(false);

        horizontalLayout_2->addWidget(colorLineEdit);

        colorToolButton = new QToolButton(formLayoutWidget);
        colorToolButton->setObjectName(QString::fromUtf8("colorToolButton"));

        horizontalLayout_2->addWidget(colorToolButton);


        formLayout->setLayout(3, QFormLayout::FieldRole, horizontalLayout_2);

        disabledColorLabel = new QLabel(formLayoutWidget);
        disabledColorLabel->setObjectName(QString::fromUtf8("disabledColorLabel"));

        formLayout->setWidget(4, QFormLayout::LabelRole, disabledColorLabel);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        disabledColorLineEdit = new QLineEdit(formLayoutWidget);
        disabledColorLineEdit->setObjectName(QString::fromUtf8("disabledColorLineEdit"));
        disabledColorLineEdit->setEnabled(false);

        horizontalLayout_4->addWidget(disabledColorLineEdit);

        disabledColorToolButton = new QToolButton(formLayoutWidget);
        disabledColorToolButton->setObjectName(QString::fromUtf8("disabledColorToolButton"));

        horizontalLayout_4->addWidget(disabledColorToolButton);


        formLayout->setLayout(4, QFormLayout::FieldRole, horizontalLayout_4);


        retranslateUi(IconButtonSettingsForm);

        QMetaObject::connectSlotsByName(IconButtonSettingsForm);
    } // setupUi

    void retranslateUi(QWidget *IconButtonSettingsForm)
    {
        IconButtonSettingsForm->setWindowTitle(QApplication::translate("IconButtonSettingsForm", "Form", nullptr));
        disabledLabel->setText(QApplication::translate("IconButtonSettingsForm", "Disabled", nullptr));
        sizeLabel->setText(QApplication::translate("IconButtonSettingsForm", "Size", nullptr));
        useThemeColorsLabel->setText(QApplication::translate("IconButtonSettingsForm", "Use theme colors", nullptr));
        colorLabel->setText(QApplication::translate("IconButtonSettingsForm", "Color", nullptr));
        colorToolButton->setText(QApplication::translate("IconButtonSettingsForm", "...", nullptr));
        disabledColorLabel->setText(QApplication::translate("IconButtonSettingsForm", "Disabled color", nullptr));
        disabledColorToolButton->setText(QApplication::translate("IconButtonSettingsForm", "...", nullptr));
    } // retranslateUi

};

namespace Ui {
    class IconButtonSettingsForm: public Ui_IconButtonSettingsForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ICONBUTTONSETTINGSFORM_H
