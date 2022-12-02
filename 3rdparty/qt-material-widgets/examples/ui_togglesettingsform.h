/********************************************************************************
** Form generated from reading UI file 'togglesettingsform.ui'
**
** Created by: Qt User Interface Compiler version 5.12.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TOGGLESETTINGSFORM_H
#define UI_TOGGLESETTINGSFORM_H

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

class Ui_ToggleSettingsForm
{
public:
    QWidget *formLayoutWidget;
    QFormLayout *formLayout;
    QLabel *disabledLabel;
    QCheckBox *disabledCheckBox;
    QLabel *checkedLabel;
    QCheckBox *checkedCheckBox;
    QLabel *orientationLabel;
    QComboBox *orientationComboBox;
    QLabel *activeColorLabel;
    QLabel *inactiveColorLabel;
    QLabel *disabledColorLabel;
    QLabel *trackColorLabel;
    QHBoxLayout *horizontalLayout;
    QLineEdit *activeColorLineEdit;
    QToolButton *activeColorToolButton;
    QHBoxLayout *horizontalLayout_3;
    QLineEdit *inactiveColorLineEdit;
    QToolButton *inactiveColorToolButton;
    QHBoxLayout *horizontalLayout_4;
    QLineEdit *disabledColorLineEdit;
    QToolButton *disabledColorToolButton;
    QHBoxLayout *horizontalLayout_5;
    QLineEdit *trackColorLineEdit;
    QToolButton *trackColorToolButton;
    QLabel *useThemeColorsLabel;
    QCheckBox *useThemeColorsCheckBox;

    void setupUi(QWidget *ToggleSettingsForm)
    {
        if (ToggleSettingsForm->objectName().isEmpty())
            ToggleSettingsForm->setObjectName(QString::fromUtf8("ToggleSettingsForm"));
        ToggleSettingsForm->resize(482, 427);
        formLayoutWidget = new QWidget(ToggleSettingsForm);
        formLayoutWidget->setObjectName(QString::fromUtf8("formLayoutWidget"));
        formLayoutWidget->setGeometry(QRect(0, 0, 441, 341));
        formLayout = new QFormLayout(formLayoutWidget);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        formLayout->setContentsMargins(0, 0, 0, 0);
        disabledLabel = new QLabel(formLayoutWidget);
        disabledLabel->setObjectName(QString::fromUtf8("disabledLabel"));

        formLayout->setWidget(0, QFormLayout::LabelRole, disabledLabel);

        disabledCheckBox = new QCheckBox(formLayoutWidget);
        disabledCheckBox->setObjectName(QString::fromUtf8("disabledCheckBox"));

        formLayout->setWidget(0, QFormLayout::FieldRole, disabledCheckBox);

        checkedLabel = new QLabel(formLayoutWidget);
        checkedLabel->setObjectName(QString::fromUtf8("checkedLabel"));

        formLayout->setWidget(1, QFormLayout::LabelRole, checkedLabel);

        checkedCheckBox = new QCheckBox(formLayoutWidget);
        checkedCheckBox->setObjectName(QString::fromUtf8("checkedCheckBox"));

        formLayout->setWidget(1, QFormLayout::FieldRole, checkedCheckBox);

        orientationLabel = new QLabel(formLayoutWidget);
        orientationLabel->setObjectName(QString::fromUtf8("orientationLabel"));

        formLayout->setWidget(2, QFormLayout::LabelRole, orientationLabel);

        orientationComboBox = new QComboBox(formLayoutWidget);
        orientationComboBox->addItem(QString());
        orientationComboBox->addItem(QString());
        orientationComboBox->setObjectName(QString::fromUtf8("orientationComboBox"));

        formLayout->setWidget(2, QFormLayout::FieldRole, orientationComboBox);

        activeColorLabel = new QLabel(formLayoutWidget);
        activeColorLabel->setObjectName(QString::fromUtf8("activeColorLabel"));

        formLayout->setWidget(4, QFormLayout::LabelRole, activeColorLabel);

        inactiveColorLabel = new QLabel(formLayoutWidget);
        inactiveColorLabel->setObjectName(QString::fromUtf8("inactiveColorLabel"));

        formLayout->setWidget(5, QFormLayout::LabelRole, inactiveColorLabel);

        disabledColorLabel = new QLabel(formLayoutWidget);
        disabledColorLabel->setObjectName(QString::fromUtf8("disabledColorLabel"));

        formLayout->setWidget(6, QFormLayout::LabelRole, disabledColorLabel);

        trackColorLabel = new QLabel(formLayoutWidget);
        trackColorLabel->setObjectName(QString::fromUtf8("trackColorLabel"));

        formLayout->setWidget(7, QFormLayout::LabelRole, trackColorLabel);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        activeColorLineEdit = new QLineEdit(formLayoutWidget);
        activeColorLineEdit->setObjectName(QString::fromUtf8("activeColorLineEdit"));

        horizontalLayout->addWidget(activeColorLineEdit);

        activeColorToolButton = new QToolButton(formLayoutWidget);
        activeColorToolButton->setObjectName(QString::fromUtf8("activeColorToolButton"));

        horizontalLayout->addWidget(activeColorToolButton);


        formLayout->setLayout(4, QFormLayout::FieldRole, horizontalLayout);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        inactiveColorLineEdit = new QLineEdit(formLayoutWidget);
        inactiveColorLineEdit->setObjectName(QString::fromUtf8("inactiveColorLineEdit"));

        horizontalLayout_3->addWidget(inactiveColorLineEdit);

        inactiveColorToolButton = new QToolButton(formLayoutWidget);
        inactiveColorToolButton->setObjectName(QString::fromUtf8("inactiveColorToolButton"));

        horizontalLayout_3->addWidget(inactiveColorToolButton);


        formLayout->setLayout(5, QFormLayout::FieldRole, horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        disabledColorLineEdit = new QLineEdit(formLayoutWidget);
        disabledColorLineEdit->setObjectName(QString::fromUtf8("disabledColorLineEdit"));

        horizontalLayout_4->addWidget(disabledColorLineEdit);

        disabledColorToolButton = new QToolButton(formLayoutWidget);
        disabledColorToolButton->setObjectName(QString::fromUtf8("disabledColorToolButton"));

        horizontalLayout_4->addWidget(disabledColorToolButton);


        formLayout->setLayout(6, QFormLayout::FieldRole, horizontalLayout_4);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        trackColorLineEdit = new QLineEdit(formLayoutWidget);
        trackColorLineEdit->setObjectName(QString::fromUtf8("trackColorLineEdit"));

        horizontalLayout_5->addWidget(trackColorLineEdit);

        trackColorToolButton = new QToolButton(formLayoutWidget);
        trackColorToolButton->setObjectName(QString::fromUtf8("trackColorToolButton"));

        horizontalLayout_5->addWidget(trackColorToolButton);


        formLayout->setLayout(7, QFormLayout::FieldRole, horizontalLayout_5);

        useThemeColorsLabel = new QLabel(formLayoutWidget);
        useThemeColorsLabel->setObjectName(QString::fromUtf8("useThemeColorsLabel"));

        formLayout->setWidget(3, QFormLayout::LabelRole, useThemeColorsLabel);

        useThemeColorsCheckBox = new QCheckBox(formLayoutWidget);
        useThemeColorsCheckBox->setObjectName(QString::fromUtf8("useThemeColorsCheckBox"));

        formLayout->setWidget(3, QFormLayout::FieldRole, useThemeColorsCheckBox);


        retranslateUi(ToggleSettingsForm);

        QMetaObject::connectSlotsByName(ToggleSettingsForm);
    } // setupUi

    void retranslateUi(QWidget *ToggleSettingsForm)
    {
        ToggleSettingsForm->setWindowTitle(QApplication::translate("ToggleSettingsForm", "Form", nullptr));
        disabledLabel->setText(QApplication::translate("ToggleSettingsForm", "Disabled", nullptr));
        checkedLabel->setText(QApplication::translate("ToggleSettingsForm", "Checked", nullptr));
        orientationLabel->setText(QApplication::translate("ToggleSettingsForm", "Orientation", nullptr));
        orientationComboBox->setItemText(0, QApplication::translate("ToggleSettingsForm", "Horizontal", nullptr));
        orientationComboBox->setItemText(1, QApplication::translate("ToggleSettingsForm", "Vertical", nullptr));

        activeColorLabel->setText(QApplication::translate("ToggleSettingsForm", "Active color", nullptr));
        inactiveColorLabel->setText(QApplication::translate("ToggleSettingsForm", "Inactive color", nullptr));
        disabledColorLabel->setText(QApplication::translate("ToggleSettingsForm", "Disabled color", nullptr));
        trackColorLabel->setText(QApplication::translate("ToggleSettingsForm", "Track color", nullptr));
        activeColorToolButton->setText(QApplication::translate("ToggleSettingsForm", "...", nullptr));
        inactiveColorToolButton->setText(QApplication::translate("ToggleSettingsForm", "...", nullptr));
        disabledColorToolButton->setText(QApplication::translate("ToggleSettingsForm", "...", nullptr));
        trackColorToolButton->setText(QApplication::translate("ToggleSettingsForm", "...", nullptr));
        useThemeColorsLabel->setText(QApplication::translate("ToggleSettingsForm", "Use theme colors", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ToggleSettingsForm: public Ui_ToggleSettingsForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TOGGLESETTINGSFORM_H
