/********************************************************************************
** Form generated from reading UI file 'appbarsettingsform.ui'
**
** Created by: Qt User Interface Compiler version 5.12.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_APPBARSETTINGSFORM_H
#define UI_APPBARSETTINGSFORM_H

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

class Ui_AppBarSettingsForm
{
public:
    QWidget *formLayoutWidget;
    QFormLayout *formLayout;
    QLabel *useThemeColorsLabel;
    QCheckBox *useThemeColorsCheckBox;
    QLabel *backgroundColorLabel;
    QHBoxLayout *horizontalLayout_5;
    QLineEdit *backgroundColorLineEdit;
    QToolButton *backgroundColorToolButton;

    void setupUi(QWidget *AppBarSettingsForm)
    {
        if (AppBarSettingsForm->objectName().isEmpty())
            AppBarSettingsForm->setObjectName(QString::fromUtf8("AppBarSettingsForm"));
        AppBarSettingsForm->resize(474, 387);
        formLayoutWidget = new QWidget(AppBarSettingsForm);
        formLayoutWidget->setObjectName(QString::fromUtf8("formLayoutWidget"));
        formLayoutWidget->setGeometry(QRect(0, 0, 221, 231));
        formLayout = new QFormLayout(formLayoutWidget);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        formLayout->setContentsMargins(0, 0, 0, 0);
        useThemeColorsLabel = new QLabel(formLayoutWidget);
        useThemeColorsLabel->setObjectName(QString::fromUtf8("useThemeColorsLabel"));

        formLayout->setWidget(0, QFormLayout::LabelRole, useThemeColorsLabel);

        useThemeColorsCheckBox = new QCheckBox(formLayoutWidget);
        useThemeColorsCheckBox->setObjectName(QString::fromUtf8("useThemeColorsCheckBox"));

        formLayout->setWidget(0, QFormLayout::FieldRole, useThemeColorsCheckBox);

        backgroundColorLabel = new QLabel(formLayoutWidget);
        backgroundColorLabel->setObjectName(QString::fromUtf8("backgroundColorLabel"));

        formLayout->setWidget(1, QFormLayout::LabelRole, backgroundColorLabel);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        backgroundColorLineEdit = new QLineEdit(formLayoutWidget);
        backgroundColorLineEdit->setObjectName(QString::fromUtf8("backgroundColorLineEdit"));
        backgroundColorLineEdit->setEnabled(false);

        horizontalLayout_5->addWidget(backgroundColorLineEdit);

        backgroundColorToolButton = new QToolButton(formLayoutWidget);
        backgroundColorToolButton->setObjectName(QString::fromUtf8("backgroundColorToolButton"));

        horizontalLayout_5->addWidget(backgroundColorToolButton);


        formLayout->setLayout(1, QFormLayout::FieldRole, horizontalLayout_5);


        retranslateUi(AppBarSettingsForm);

        QMetaObject::connectSlotsByName(AppBarSettingsForm);
    } // setupUi

    void retranslateUi(QWidget *AppBarSettingsForm)
    {
        AppBarSettingsForm->setWindowTitle(QApplication::translate("AppBarSettingsForm", "Form", nullptr));
        useThemeColorsLabel->setText(QApplication::translate("AppBarSettingsForm", "Use theme colors", nullptr));
        backgroundColorLabel->setText(QApplication::translate("AppBarSettingsForm", "Background color", nullptr));
        backgroundColorToolButton->setText(QApplication::translate("AppBarSettingsForm", "...", nullptr));
    } // retranslateUi

};

namespace Ui {
    class AppBarSettingsForm: public Ui_AppBarSettingsForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_APPBARSETTINGSFORM_H
