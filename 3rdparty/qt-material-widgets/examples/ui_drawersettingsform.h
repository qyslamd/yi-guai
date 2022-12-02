/********************************************************************************
** Form generated from reading UI file 'drawersettingsform.ui'
**
** Created by: Qt User Interface Compiler version 5.12.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DRAWERSETTINGSFORM_H
#define UI_DRAWERSETTINGSFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DrawerSettingsForm
{
public:
    QWidget *formLayoutWidget;
    QFormLayout *formLayout;
    QLabel *overlayModeLabel;
    QCheckBox *overlayModeCheckBox;
    QLabel *clickToCloseLabel;
    QCheckBox *clickToCloseCheckBox;
    QPushButton *showDrawerButton;
    QPushButton *hideDrawerButton;

    void setupUi(QWidget *DrawerSettingsForm)
    {
        if (DrawerSettingsForm->objectName().isEmpty())
            DrawerSettingsForm->setObjectName(QString::fromUtf8("DrawerSettingsForm"));
        DrawerSettingsForm->resize(474, 387);
        formLayoutWidget = new QWidget(DrawerSettingsForm);
        formLayoutWidget->setObjectName(QString::fromUtf8("formLayoutWidget"));
        formLayoutWidget->setGeometry(QRect(0, 0, 180, 94));
        formLayout = new QFormLayout(formLayoutWidget);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        formLayout->setContentsMargins(0, 0, 0, 0);
        overlayModeLabel = new QLabel(formLayoutWidget);
        overlayModeLabel->setObjectName(QString::fromUtf8("overlayModeLabel"));

        formLayout->setWidget(0, QFormLayout::LabelRole, overlayModeLabel);

        overlayModeCheckBox = new QCheckBox(formLayoutWidget);
        overlayModeCheckBox->setObjectName(QString::fromUtf8("overlayModeCheckBox"));

        formLayout->setWidget(0, QFormLayout::FieldRole, overlayModeCheckBox);

        clickToCloseLabel = new QLabel(formLayoutWidget);
        clickToCloseLabel->setObjectName(QString::fromUtf8("clickToCloseLabel"));

        formLayout->setWidget(1, QFormLayout::LabelRole, clickToCloseLabel);

        clickToCloseCheckBox = new QCheckBox(formLayoutWidget);
        clickToCloseCheckBox->setObjectName(QString::fromUtf8("clickToCloseCheckBox"));

        formLayout->setWidget(1, QFormLayout::FieldRole, clickToCloseCheckBox);

        showDrawerButton = new QPushButton(formLayoutWidget);
        showDrawerButton->setObjectName(QString::fromUtf8("showDrawerButton"));

        formLayout->setWidget(2, QFormLayout::LabelRole, showDrawerButton);

        hideDrawerButton = new QPushButton(formLayoutWidget);
        hideDrawerButton->setObjectName(QString::fromUtf8("hideDrawerButton"));

        formLayout->setWidget(2, QFormLayout::FieldRole, hideDrawerButton);


        retranslateUi(DrawerSettingsForm);

        QMetaObject::connectSlotsByName(DrawerSettingsForm);
    } // setupUi

    void retranslateUi(QWidget *DrawerSettingsForm)
    {
        DrawerSettingsForm->setWindowTitle(QApplication::translate("DrawerSettingsForm", "Form", nullptr));
        overlayModeLabel->setText(QApplication::translate("DrawerSettingsForm", "Overlay mode", nullptr));
        clickToCloseLabel->setText(QApplication::translate("DrawerSettingsForm", "Click to close", nullptr));
        showDrawerButton->setText(QApplication::translate("DrawerSettingsForm", "Show drawer", nullptr));
        hideDrawerButton->setText(QApplication::translate("DrawerSettingsForm", "Hide drawer", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DrawerSettingsForm: public Ui_DrawerSettingsForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DRAWERSETTINGSFORM_H
