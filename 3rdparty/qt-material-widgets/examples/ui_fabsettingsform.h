/********************************************************************************
** Form generated from reading UI file 'fabsettingsform.ui'
**
** Created by: Qt User Interface Compiler version 5.12.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FABSETTINGSFORM_H
#define UI_FABSETTINGSFORM_H

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

class Ui_FloatingActionButtonSettingsForm
{
public:
    QWidget *formLayoutWidget;
    QFormLayout *formLayout;
    QLabel *disabledLabel;
    QCheckBox *disabledCheckBox;
    QLabel *cornerLabel;
    QComboBox *cornerComboBox;
    QLabel *buttonRoleLabel;
    QComboBox *buttonRoleComboBox;
    QLabel *horizontalOffsetLabel;
    QSpinBox *horizontalOffsetSpinBox;
    QLabel *verticalOffsetLabel;
    QSpinBox *verticalOffsetSpinBox;
    QLabel *miniLabel;
    QCheckBox *miniCheckBox;
    QLabel *rippleStyleLabel;
    QComboBox *rippleStyleComboBox;
    QLabel *useThemeColorsLabel;
    QCheckBox *useThemeColorsCheckBox;
    QLabel *backgroundColorLabel;
    QHBoxLayout *horizontalLayout;
    QLineEdit *backgroundColorLineEdit;
    QToolButton *backgroundColorToolButton;
    QLabel *foregroundColorLabel;
    QLabel *disabledBgColorLabel;
    QLabel *disabledFgColorLabel;
    QHBoxLayout *horizontalLayout_2;
    QLineEdit *foregroundColorLineEdit;
    QToolButton *foregroundColorToolButton;
    QHBoxLayout *horizontalLayout_3;
    QLineEdit *disabledBgColorLineEdit;
    QToolButton *disabledBgColorToolButton;
    QHBoxLayout *horizontalLayout_4;
    QLineEdit *disabledFgColorLineEdit;
    QToolButton *disabledFgColorToolButton;

    void setupUi(QWidget *FloatingActionButtonSettingsForm)
    {
        if (FloatingActionButtonSettingsForm->objectName().isEmpty())
            FloatingActionButtonSettingsForm->setObjectName(QString::fromUtf8("FloatingActionButtonSettingsForm"));
        FloatingActionButtonSettingsForm->resize(599, 418);
        formLayoutWidget = new QWidget(FloatingActionButtonSettingsForm);
        formLayoutWidget->setObjectName(QString::fromUtf8("formLayoutWidget"));
        formLayoutWidget->setGeometry(QRect(0, 0, 234, 391));
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

        cornerLabel = new QLabel(formLayoutWidget);
        cornerLabel->setObjectName(QString::fromUtf8("cornerLabel"));

        formLayout->setWidget(1, QFormLayout::LabelRole, cornerLabel);

        cornerComboBox = new QComboBox(formLayoutWidget);
        cornerComboBox->addItem(QString());
        cornerComboBox->addItem(QString());
        cornerComboBox->addItem(QString());
        cornerComboBox->addItem(QString());
        cornerComboBox->setObjectName(QString::fromUtf8("cornerComboBox"));

        formLayout->setWidget(1, QFormLayout::FieldRole, cornerComboBox);

        buttonRoleLabel = new QLabel(formLayoutWidget);
        buttonRoleLabel->setObjectName(QString::fromUtf8("buttonRoleLabel"));

        formLayout->setWidget(2, QFormLayout::LabelRole, buttonRoleLabel);

        buttonRoleComboBox = new QComboBox(formLayoutWidget);
        buttonRoleComboBox->addItem(QString());
        buttonRoleComboBox->addItem(QString());
        buttonRoleComboBox->addItem(QString());
        buttonRoleComboBox->setObjectName(QString::fromUtf8("buttonRoleComboBox"));

        formLayout->setWidget(2, QFormLayout::FieldRole, buttonRoleComboBox);

        horizontalOffsetLabel = new QLabel(formLayoutWidget);
        horizontalOffsetLabel->setObjectName(QString::fromUtf8("horizontalOffsetLabel"));

        formLayout->setWidget(3, QFormLayout::LabelRole, horizontalOffsetLabel);

        horizontalOffsetSpinBox = new QSpinBox(formLayoutWidget);
        horizontalOffsetSpinBox->setObjectName(QString::fromUtf8("horizontalOffsetSpinBox"));

        formLayout->setWidget(3, QFormLayout::FieldRole, horizontalOffsetSpinBox);

        verticalOffsetLabel = new QLabel(formLayoutWidget);
        verticalOffsetLabel->setObjectName(QString::fromUtf8("verticalOffsetLabel"));

        formLayout->setWidget(4, QFormLayout::LabelRole, verticalOffsetLabel);

        verticalOffsetSpinBox = new QSpinBox(formLayoutWidget);
        verticalOffsetSpinBox->setObjectName(QString::fromUtf8("verticalOffsetSpinBox"));

        formLayout->setWidget(4, QFormLayout::FieldRole, verticalOffsetSpinBox);

        miniLabel = new QLabel(formLayoutWidget);
        miniLabel->setObjectName(QString::fromUtf8("miniLabel"));

        formLayout->setWidget(5, QFormLayout::LabelRole, miniLabel);

        miniCheckBox = new QCheckBox(formLayoutWidget);
        miniCheckBox->setObjectName(QString::fromUtf8("miniCheckBox"));

        formLayout->setWidget(5, QFormLayout::FieldRole, miniCheckBox);

        rippleStyleLabel = new QLabel(formLayoutWidget);
        rippleStyleLabel->setObjectName(QString::fromUtf8("rippleStyleLabel"));

        formLayout->setWidget(6, QFormLayout::LabelRole, rippleStyleLabel);

        rippleStyleComboBox = new QComboBox(formLayoutWidget);
        rippleStyleComboBox->addItem(QString());
        rippleStyleComboBox->addItem(QString());
        rippleStyleComboBox->addItem(QString());
        rippleStyleComboBox->setObjectName(QString::fromUtf8("rippleStyleComboBox"));

        formLayout->setWidget(6, QFormLayout::FieldRole, rippleStyleComboBox);

        useThemeColorsLabel = new QLabel(formLayoutWidget);
        useThemeColorsLabel->setObjectName(QString::fromUtf8("useThemeColorsLabel"));

        formLayout->setWidget(7, QFormLayout::LabelRole, useThemeColorsLabel);

        useThemeColorsCheckBox = new QCheckBox(formLayoutWidget);
        useThemeColorsCheckBox->setObjectName(QString::fromUtf8("useThemeColorsCheckBox"));

        formLayout->setWidget(7, QFormLayout::FieldRole, useThemeColorsCheckBox);

        backgroundColorLabel = new QLabel(formLayoutWidget);
        backgroundColorLabel->setObjectName(QString::fromUtf8("backgroundColorLabel"));

        formLayout->setWidget(8, QFormLayout::LabelRole, backgroundColorLabel);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        backgroundColorLineEdit = new QLineEdit(formLayoutWidget);
        backgroundColorLineEdit->setObjectName(QString::fromUtf8("backgroundColorLineEdit"));
        backgroundColorLineEdit->setEnabled(false);

        horizontalLayout->addWidget(backgroundColorLineEdit);

        backgroundColorToolButton = new QToolButton(formLayoutWidget);
        backgroundColorToolButton->setObjectName(QString::fromUtf8("backgroundColorToolButton"));

        horizontalLayout->addWidget(backgroundColorToolButton);


        formLayout->setLayout(8, QFormLayout::FieldRole, horizontalLayout);

        foregroundColorLabel = new QLabel(formLayoutWidget);
        foregroundColorLabel->setObjectName(QString::fromUtf8("foregroundColorLabel"));

        formLayout->setWidget(9, QFormLayout::LabelRole, foregroundColorLabel);

        disabledBgColorLabel = new QLabel(formLayoutWidget);
        disabledBgColorLabel->setObjectName(QString::fromUtf8("disabledBgColorLabel"));

        formLayout->setWidget(10, QFormLayout::LabelRole, disabledBgColorLabel);

        disabledFgColorLabel = new QLabel(formLayoutWidget);
        disabledFgColorLabel->setObjectName(QString::fromUtf8("disabledFgColorLabel"));

        formLayout->setWidget(11, QFormLayout::LabelRole, disabledFgColorLabel);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        foregroundColorLineEdit = new QLineEdit(formLayoutWidget);
        foregroundColorLineEdit->setObjectName(QString::fromUtf8("foregroundColorLineEdit"));
        foregroundColorLineEdit->setEnabled(false);

        horizontalLayout_2->addWidget(foregroundColorLineEdit);

        foregroundColorToolButton = new QToolButton(formLayoutWidget);
        foregroundColorToolButton->setObjectName(QString::fromUtf8("foregroundColorToolButton"));

        horizontalLayout_2->addWidget(foregroundColorToolButton);


        formLayout->setLayout(9, QFormLayout::FieldRole, horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        disabledBgColorLineEdit = new QLineEdit(formLayoutWidget);
        disabledBgColorLineEdit->setObjectName(QString::fromUtf8("disabledBgColorLineEdit"));
        disabledBgColorLineEdit->setEnabled(false);

        horizontalLayout_3->addWidget(disabledBgColorLineEdit);

        disabledBgColorToolButton = new QToolButton(formLayoutWidget);
        disabledBgColorToolButton->setObjectName(QString::fromUtf8("disabledBgColorToolButton"));

        horizontalLayout_3->addWidget(disabledBgColorToolButton);


        formLayout->setLayout(10, QFormLayout::FieldRole, horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        disabledFgColorLineEdit = new QLineEdit(formLayoutWidget);
        disabledFgColorLineEdit->setObjectName(QString::fromUtf8("disabledFgColorLineEdit"));
        disabledFgColorLineEdit->setEnabled(false);

        horizontalLayout_4->addWidget(disabledFgColorLineEdit);

        disabledFgColorToolButton = new QToolButton(formLayoutWidget);
        disabledFgColorToolButton->setObjectName(QString::fromUtf8("disabledFgColorToolButton"));

        horizontalLayout_4->addWidget(disabledFgColorToolButton);


        formLayout->setLayout(11, QFormLayout::FieldRole, horizontalLayout_4);


        retranslateUi(FloatingActionButtonSettingsForm);

        QMetaObject::connectSlotsByName(FloatingActionButtonSettingsForm);
    } // setupUi

    void retranslateUi(QWidget *FloatingActionButtonSettingsForm)
    {
        FloatingActionButtonSettingsForm->setWindowTitle(QApplication::translate("FloatingActionButtonSettingsForm", "Form", nullptr));
        disabledLabel->setText(QApplication::translate("FloatingActionButtonSettingsForm", "Disabled", nullptr));
        cornerLabel->setText(QApplication::translate("FloatingActionButtonSettingsForm", "Corner", nullptr));
        cornerComboBox->setItemText(0, QApplication::translate("FloatingActionButtonSettingsForm", "Top Left", nullptr));
        cornerComboBox->setItemText(1, QApplication::translate("FloatingActionButtonSettingsForm", "Top Right", nullptr));
        cornerComboBox->setItemText(2, QApplication::translate("FloatingActionButtonSettingsForm", "Bottom Left", nullptr));
        cornerComboBox->setItemText(3, QApplication::translate("FloatingActionButtonSettingsForm", "Bottom Right", nullptr));

        buttonRoleLabel->setText(QApplication::translate("FloatingActionButtonSettingsForm", "Button role", nullptr));
        buttonRoleComboBox->setItemText(0, QApplication::translate("FloatingActionButtonSettingsForm", "Default", nullptr));
        buttonRoleComboBox->setItemText(1, QApplication::translate("FloatingActionButtonSettingsForm", "Primary", nullptr));
        buttonRoleComboBox->setItemText(2, QApplication::translate("FloatingActionButtonSettingsForm", "Secondary", nullptr));

        horizontalOffsetLabel->setText(QApplication::translate("FloatingActionButtonSettingsForm", "Horizontal offset", nullptr));
        verticalOffsetLabel->setText(QApplication::translate("FloatingActionButtonSettingsForm", "Vertical offset", nullptr));
        miniLabel->setText(QApplication::translate("FloatingActionButtonSettingsForm", "Mini", nullptr));
        rippleStyleLabel->setText(QApplication::translate("FloatingActionButtonSettingsForm", "Ripple style", nullptr));
        rippleStyleComboBox->setItemText(0, QApplication::translate("FloatingActionButtonSettingsForm", "Centered", nullptr));
        rippleStyleComboBox->setItemText(1, QApplication::translate("FloatingActionButtonSettingsForm", "Positioned", nullptr));
        rippleStyleComboBox->setItemText(2, QApplication::translate("FloatingActionButtonSettingsForm", "No Ripple", nullptr));

        useThemeColorsLabel->setText(QApplication::translate("FloatingActionButtonSettingsForm", "Use theme colors", nullptr));
        backgroundColorLabel->setText(QApplication::translate("FloatingActionButtonSettingsForm", "Background color", nullptr));
        backgroundColorToolButton->setText(QApplication::translate("FloatingActionButtonSettingsForm", "...", nullptr));
        foregroundColorLabel->setText(QApplication::translate("FloatingActionButtonSettingsForm", "Foreground color", nullptr));
        disabledBgColorLabel->setText(QApplication::translate("FloatingActionButtonSettingsForm", "Disabled bg color", nullptr));
        disabledFgColorLabel->setText(QApplication::translate("FloatingActionButtonSettingsForm", "Disabled fg color", nullptr));
        foregroundColorToolButton->setText(QApplication::translate("FloatingActionButtonSettingsForm", "...", nullptr));
        disabledBgColorToolButton->setText(QApplication::translate("FloatingActionButtonSettingsForm", "...", nullptr));
        disabledFgColorToolButton->setText(QApplication::translate("FloatingActionButtonSettingsForm", "...", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FloatingActionButtonSettingsForm: public Ui_FloatingActionButtonSettingsForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FABSETTINGSFORM_H
