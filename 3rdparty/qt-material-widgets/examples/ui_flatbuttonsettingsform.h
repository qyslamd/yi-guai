/********************************************************************************
** Form generated from reading UI file 'flatbuttonsettingsform.ui'
**
** Created by: Qt User Interface Compiler version 5.12.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FLATBUTTONSETTINGSFORM_H
#define UI_FLATBUTTONSETTINGSFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FlatButtonSettingsForm
{
public:
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout;
    QCheckBox *disabledCheckBox;
    QCheckBox *checkableCheckBox;
    QCheckBox *checkedCheckBox;
    QCheckBox *showHaloCheckBox;
    QCheckBox *transparentCheckBox;
    QCheckBox *iconCheckBox;
    QSpacerItem *verticalSpacer;
    QFrame *line;
    QFormLayout *formLayout;
    QLabel *buttonRoleLabel;
    QComboBox *buttonRoleComboBox;
    QLabel *rippleStyleLabel;
    QComboBox *rippleStyleComboBox;
    QLabel *hoverStyleLabel;
    QComboBox *hoverStyleComboBox;
    QLabel *iconPlacementLabel;
    QComboBox *iconPlacementComboBox;
    QLabel *cornerRadiusLabel;
    QSpinBox *cornerRadiusSpinBox;
    QLabel *overlayOpacityLabel;
    QDoubleSpinBox *overlayOpacityDoubleSpinBox;
    QLabel *iconSizeLabel;
    QSpinBox *iconSizeSpinBox;
    QLabel *fontSizeLabel;
    QDoubleSpinBox *fontSizeDoubleSpinBox;
    QLabel *buttonTextLabel;
    QLineEdit *buttonTextLineEdit;
    QLabel *textAlignmentLabel;
    QComboBox *textAlignmentComboBox;
    QFrame *line_2;
    QVBoxLayout *verticalLayout_4;
    QCheckBox *useThemeColorsCheckBox;
    QFormLayout *formLayout_2;
    QLabel *backgroundColorLabel;
    QHBoxLayout *horizontalLayout_3;
    QLineEdit *backgroundColorLineEdit;
    QToolButton *backgroundColorToolButton;
    QLabel *foregroundColorLabel;
    QHBoxLayout *horizontalLayout_4;
    QLineEdit *foregroundColorLineEdit;
    QToolButton *foregroundColorToolButton;
    QLabel *disabledBgColorLabel;
    QLabel *disabledFgColorLabel;
    QLabel *overlayColorLabel;
    QHBoxLayout *horizontalLayout_5;
    QLineEdit *overlayColorLineEdit;
    QToolButton *overlayColorToolButton;
    QHBoxLayout *horizontalLayout_6;
    QLineEdit *disabledBgColorLineEdit;
    QToolButton *disabledBgColorToolButton;
    QHBoxLayout *horizontalLayout_7;
    QLineEdit *disableFgColorLineEdit;
    QToolButton *disabledFgColorToolButton;
    QHBoxLayout *horizontalLayout_8;
    QPushButton *defaultPresetPushButton;
    QPushButton *checkablePresetPushButton;
    QSpacerItem *horizontalSpacer;

    void setupUi(QWidget *FlatButtonSettingsForm)
    {
        if (FlatButtonSettingsForm->objectName().isEmpty())
            FlatButtonSettingsForm->setObjectName(QString::fromUtf8("FlatButtonSettingsForm"));
        FlatButtonSettingsForm->resize(624, 537);
        verticalLayoutWidget = new QWidget(FlatButtonSettingsForm);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(0, 0, 549, 314));
        verticalLayout_2 = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        disabledCheckBox = new QCheckBox(verticalLayoutWidget);
        disabledCheckBox->setObjectName(QString::fromUtf8("disabledCheckBox"));

        verticalLayout->addWidget(disabledCheckBox);

        checkableCheckBox = new QCheckBox(verticalLayoutWidget);
        checkableCheckBox->setObjectName(QString::fromUtf8("checkableCheckBox"));

        verticalLayout->addWidget(checkableCheckBox);

        checkedCheckBox = new QCheckBox(verticalLayoutWidget);
        checkedCheckBox->setObjectName(QString::fromUtf8("checkedCheckBox"));

        verticalLayout->addWidget(checkedCheckBox);

        showHaloCheckBox = new QCheckBox(verticalLayoutWidget);
        showHaloCheckBox->setObjectName(QString::fromUtf8("showHaloCheckBox"));

        verticalLayout->addWidget(showHaloCheckBox);

        transparentCheckBox = new QCheckBox(verticalLayoutWidget);
        transparentCheckBox->setObjectName(QString::fromUtf8("transparentCheckBox"));

        verticalLayout->addWidget(transparentCheckBox);

        iconCheckBox = new QCheckBox(verticalLayoutWidget);
        iconCheckBox->setObjectName(QString::fromUtf8("iconCheckBox"));

        verticalLayout->addWidget(iconCheckBox);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        horizontalLayout->addLayout(verticalLayout);

        line = new QFrame(verticalLayoutWidget);
        line->setObjectName(QString::fromUtf8("line"));
        line->setFrameShape(QFrame::VLine);
        line->setFrameShadow(QFrame::Sunken);

        horizontalLayout->addWidget(line);

        formLayout = new QFormLayout();
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        buttonRoleLabel = new QLabel(verticalLayoutWidget);
        buttonRoleLabel->setObjectName(QString::fromUtf8("buttonRoleLabel"));

        formLayout->setWidget(0, QFormLayout::LabelRole, buttonRoleLabel);

        buttonRoleComboBox = new QComboBox(verticalLayoutWidget);
        buttonRoleComboBox->addItem(QString());
        buttonRoleComboBox->addItem(QString());
        buttonRoleComboBox->addItem(QString());
        buttonRoleComboBox->setObjectName(QString::fromUtf8("buttonRoleComboBox"));

        formLayout->setWidget(0, QFormLayout::FieldRole, buttonRoleComboBox);

        rippleStyleLabel = new QLabel(verticalLayoutWidget);
        rippleStyleLabel->setObjectName(QString::fromUtf8("rippleStyleLabel"));

        formLayout->setWidget(1, QFormLayout::LabelRole, rippleStyleLabel);

        rippleStyleComboBox = new QComboBox(verticalLayoutWidget);
        rippleStyleComboBox->addItem(QString());
        rippleStyleComboBox->addItem(QString());
        rippleStyleComboBox->addItem(QString());
        rippleStyleComboBox->setObjectName(QString::fromUtf8("rippleStyleComboBox"));

        formLayout->setWidget(1, QFormLayout::FieldRole, rippleStyleComboBox);

        hoverStyleLabel = new QLabel(verticalLayoutWidget);
        hoverStyleLabel->setObjectName(QString::fromUtf8("hoverStyleLabel"));

        formLayout->setWidget(2, QFormLayout::LabelRole, hoverStyleLabel);

        hoverStyleComboBox = new QComboBox(verticalLayoutWidget);
        hoverStyleComboBox->addItem(QString());
        hoverStyleComboBox->addItem(QString());
        hoverStyleComboBox->addItem(QString());
        hoverStyleComboBox->setObjectName(QString::fromUtf8("hoverStyleComboBox"));

        formLayout->setWidget(2, QFormLayout::FieldRole, hoverStyleComboBox);

        iconPlacementLabel = new QLabel(verticalLayoutWidget);
        iconPlacementLabel->setObjectName(QString::fromUtf8("iconPlacementLabel"));

        formLayout->setWidget(3, QFormLayout::LabelRole, iconPlacementLabel);

        iconPlacementComboBox = new QComboBox(verticalLayoutWidget);
        iconPlacementComboBox->addItem(QString());
        iconPlacementComboBox->addItem(QString());
        iconPlacementComboBox->setObjectName(QString::fromUtf8("iconPlacementComboBox"));

        formLayout->setWidget(3, QFormLayout::FieldRole, iconPlacementComboBox);

        cornerRadiusLabel = new QLabel(verticalLayoutWidget);
        cornerRadiusLabel->setObjectName(QString::fromUtf8("cornerRadiusLabel"));

        formLayout->setWidget(4, QFormLayout::LabelRole, cornerRadiusLabel);

        cornerRadiusSpinBox = new QSpinBox(verticalLayoutWidget);
        cornerRadiusSpinBox->setObjectName(QString::fromUtf8("cornerRadiusSpinBox"));

        formLayout->setWidget(4, QFormLayout::FieldRole, cornerRadiusSpinBox);

        overlayOpacityLabel = new QLabel(verticalLayoutWidget);
        overlayOpacityLabel->setObjectName(QString::fromUtf8("overlayOpacityLabel"));

        formLayout->setWidget(6, QFormLayout::LabelRole, overlayOpacityLabel);

        overlayOpacityDoubleSpinBox = new QDoubleSpinBox(verticalLayoutWidget);
        overlayOpacityDoubleSpinBox->setObjectName(QString::fromUtf8("overlayOpacityDoubleSpinBox"));
        overlayOpacityDoubleSpinBox->setMaximum(1.000000000000000);
        overlayOpacityDoubleSpinBox->setSingleStep(0.050000000000000);

        formLayout->setWidget(6, QFormLayout::FieldRole, overlayOpacityDoubleSpinBox);

        iconSizeLabel = new QLabel(verticalLayoutWidget);
        iconSizeLabel->setObjectName(QString::fromUtf8("iconSizeLabel"));

        formLayout->setWidget(7, QFormLayout::LabelRole, iconSizeLabel);

        iconSizeSpinBox = new QSpinBox(verticalLayoutWidget);
        iconSizeSpinBox->setObjectName(QString::fromUtf8("iconSizeSpinBox"));

        formLayout->setWidget(7, QFormLayout::FieldRole, iconSizeSpinBox);

        fontSizeLabel = new QLabel(verticalLayoutWidget);
        fontSizeLabel->setObjectName(QString::fromUtf8("fontSizeLabel"));

        formLayout->setWidget(8, QFormLayout::LabelRole, fontSizeLabel);

        fontSizeDoubleSpinBox = new QDoubleSpinBox(verticalLayoutWidget);
        fontSizeDoubleSpinBox->setObjectName(QString::fromUtf8("fontSizeDoubleSpinBox"));

        formLayout->setWidget(8, QFormLayout::FieldRole, fontSizeDoubleSpinBox);

        buttonTextLabel = new QLabel(verticalLayoutWidget);
        buttonTextLabel->setObjectName(QString::fromUtf8("buttonTextLabel"));

        formLayout->setWidget(9, QFormLayout::LabelRole, buttonTextLabel);

        buttonTextLineEdit = new QLineEdit(verticalLayoutWidget);
        buttonTextLineEdit->setObjectName(QString::fromUtf8("buttonTextLineEdit"));

        formLayout->setWidget(9, QFormLayout::FieldRole, buttonTextLineEdit);

        textAlignmentLabel = new QLabel(verticalLayoutWidget);
        textAlignmentLabel->setObjectName(QString::fromUtf8("textAlignmentLabel"));

        formLayout->setWidget(5, QFormLayout::LabelRole, textAlignmentLabel);

        textAlignmentComboBox = new QComboBox(verticalLayoutWidget);
        textAlignmentComboBox->addItem(QString());
        textAlignmentComboBox->addItem(QString());
        textAlignmentComboBox->setObjectName(QString::fromUtf8("textAlignmentComboBox"));

        formLayout->setWidget(5, QFormLayout::FieldRole, textAlignmentComboBox);


        horizontalLayout->addLayout(formLayout);

        line_2 = new QFrame(verticalLayoutWidget);
        line_2->setObjectName(QString::fromUtf8("line_2"));
        line_2->setFrameShape(QFrame::VLine);
        line_2->setFrameShadow(QFrame::Sunken);

        horizontalLayout->addWidget(line_2);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        useThemeColorsCheckBox = new QCheckBox(verticalLayoutWidget);
        useThemeColorsCheckBox->setObjectName(QString::fromUtf8("useThemeColorsCheckBox"));

        verticalLayout_4->addWidget(useThemeColorsCheckBox);

        formLayout_2 = new QFormLayout();
        formLayout_2->setObjectName(QString::fromUtf8("formLayout_2"));
        backgroundColorLabel = new QLabel(verticalLayoutWidget);
        backgroundColorLabel->setObjectName(QString::fromUtf8("backgroundColorLabel"));

        formLayout_2->setWidget(0, QFormLayout::LabelRole, backgroundColorLabel);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        backgroundColorLineEdit = new QLineEdit(verticalLayoutWidget);
        backgroundColorLineEdit->setObjectName(QString::fromUtf8("backgroundColorLineEdit"));
        backgroundColorLineEdit->setEnabled(false);

        horizontalLayout_3->addWidget(backgroundColorLineEdit);

        backgroundColorToolButton = new QToolButton(verticalLayoutWidget);
        backgroundColorToolButton->setObjectName(QString::fromUtf8("backgroundColorToolButton"));

        horizontalLayout_3->addWidget(backgroundColorToolButton);


        formLayout_2->setLayout(0, QFormLayout::FieldRole, horizontalLayout_3);

        foregroundColorLabel = new QLabel(verticalLayoutWidget);
        foregroundColorLabel->setObjectName(QString::fromUtf8("foregroundColorLabel"));

        formLayout_2->setWidget(1, QFormLayout::LabelRole, foregroundColorLabel);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        foregroundColorLineEdit = new QLineEdit(verticalLayoutWidget);
        foregroundColorLineEdit->setObjectName(QString::fromUtf8("foregroundColorLineEdit"));
        foregroundColorLineEdit->setEnabled(false);

        horizontalLayout_4->addWidget(foregroundColorLineEdit);

        foregroundColorToolButton = new QToolButton(verticalLayoutWidget);
        foregroundColorToolButton->setObjectName(QString::fromUtf8("foregroundColorToolButton"));

        horizontalLayout_4->addWidget(foregroundColorToolButton);


        formLayout_2->setLayout(1, QFormLayout::FieldRole, horizontalLayout_4);

        disabledBgColorLabel = new QLabel(verticalLayoutWidget);
        disabledBgColorLabel->setObjectName(QString::fromUtf8("disabledBgColorLabel"));

        formLayout_2->setWidget(3, QFormLayout::LabelRole, disabledBgColorLabel);

        disabledFgColorLabel = new QLabel(verticalLayoutWidget);
        disabledFgColorLabel->setObjectName(QString::fromUtf8("disabledFgColorLabel"));

        formLayout_2->setWidget(4, QFormLayout::LabelRole, disabledFgColorLabel);

        overlayColorLabel = new QLabel(verticalLayoutWidget);
        overlayColorLabel->setObjectName(QString::fromUtf8("overlayColorLabel"));

        formLayout_2->setWidget(2, QFormLayout::LabelRole, overlayColorLabel);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        overlayColorLineEdit = new QLineEdit(verticalLayoutWidget);
        overlayColorLineEdit->setObjectName(QString::fromUtf8("overlayColorLineEdit"));
        overlayColorLineEdit->setEnabled(false);

        horizontalLayout_5->addWidget(overlayColorLineEdit);

        overlayColorToolButton = new QToolButton(verticalLayoutWidget);
        overlayColorToolButton->setObjectName(QString::fromUtf8("overlayColorToolButton"));

        horizontalLayout_5->addWidget(overlayColorToolButton);


        formLayout_2->setLayout(2, QFormLayout::FieldRole, horizontalLayout_5);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        disabledBgColorLineEdit = new QLineEdit(verticalLayoutWidget);
        disabledBgColorLineEdit->setObjectName(QString::fromUtf8("disabledBgColorLineEdit"));
        disabledBgColorLineEdit->setEnabled(false);

        horizontalLayout_6->addWidget(disabledBgColorLineEdit);

        disabledBgColorToolButton = new QToolButton(verticalLayoutWidget);
        disabledBgColorToolButton->setObjectName(QString::fromUtf8("disabledBgColorToolButton"));

        horizontalLayout_6->addWidget(disabledBgColorToolButton);


        formLayout_2->setLayout(3, QFormLayout::FieldRole, horizontalLayout_6);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        disableFgColorLineEdit = new QLineEdit(verticalLayoutWidget);
        disableFgColorLineEdit->setObjectName(QString::fromUtf8("disableFgColorLineEdit"));
        disableFgColorLineEdit->setEnabled(false);

        horizontalLayout_7->addWidget(disableFgColorLineEdit);

        disabledFgColorToolButton = new QToolButton(verticalLayoutWidget);
        disabledFgColorToolButton->setObjectName(QString::fromUtf8("disabledFgColorToolButton"));

        horizontalLayout_7->addWidget(disabledFgColorToolButton);


        formLayout_2->setLayout(4, QFormLayout::FieldRole, horizontalLayout_7);


        verticalLayout_4->addLayout(formLayout_2);


        horizontalLayout->addLayout(verticalLayout_4);


        verticalLayout_2->addLayout(horizontalLayout);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        defaultPresetPushButton = new QPushButton(verticalLayoutWidget);
        defaultPresetPushButton->setObjectName(QString::fromUtf8("defaultPresetPushButton"));

        horizontalLayout_8->addWidget(defaultPresetPushButton);

        checkablePresetPushButton = new QPushButton(verticalLayoutWidget);
        checkablePresetPushButton->setObjectName(QString::fromUtf8("checkablePresetPushButton"));

        horizontalLayout_8->addWidget(checkablePresetPushButton);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_8->addItem(horizontalSpacer);


        verticalLayout_2->addLayout(horizontalLayout_8);


        retranslateUi(FlatButtonSettingsForm);

        QMetaObject::connectSlotsByName(FlatButtonSettingsForm);
    } // setupUi

    void retranslateUi(QWidget *FlatButtonSettingsForm)
    {
        FlatButtonSettingsForm->setWindowTitle(QApplication::translate("FlatButtonSettingsForm", "Form", nullptr));
        disabledCheckBox->setText(QApplication::translate("FlatButtonSettingsForm", "Disabled", nullptr));
        checkableCheckBox->setText(QApplication::translate("FlatButtonSettingsForm", "Checkable", nullptr));
        checkedCheckBox->setText(QApplication::translate("FlatButtonSettingsForm", "Checked", nullptr));
        showHaloCheckBox->setText(QApplication::translate("FlatButtonSettingsForm", "Show halo", nullptr));
        transparentCheckBox->setText(QApplication::translate("FlatButtonSettingsForm", "Transparent", nullptr));
        iconCheckBox->setText(QApplication::translate("FlatButtonSettingsForm", "Icon", nullptr));
        buttonRoleLabel->setText(QApplication::translate("FlatButtonSettingsForm", "Button role", nullptr));
        buttonRoleComboBox->setItemText(0, QApplication::translate("FlatButtonSettingsForm", "Default", nullptr));
        buttonRoleComboBox->setItemText(1, QApplication::translate("FlatButtonSettingsForm", "Primary", nullptr));
        buttonRoleComboBox->setItemText(2, QApplication::translate("FlatButtonSettingsForm", "Secondary", nullptr));

        rippleStyleLabel->setText(QApplication::translate("FlatButtonSettingsForm", "Ripple style", nullptr));
        rippleStyleComboBox->setItemText(0, QApplication::translate("FlatButtonSettingsForm", "Centered", nullptr));
        rippleStyleComboBox->setItemText(1, QApplication::translate("FlatButtonSettingsForm", "Positioned", nullptr));
        rippleStyleComboBox->setItemText(2, QApplication::translate("FlatButtonSettingsForm", "No Ripple", nullptr));

        hoverStyleLabel->setText(QApplication::translate("FlatButtonSettingsForm", "Hover style", nullptr));
        hoverStyleComboBox->setItemText(0, QApplication::translate("FlatButtonSettingsForm", "No Overlay", nullptr));
        hoverStyleComboBox->setItemText(1, QApplication::translate("FlatButtonSettingsForm", "Tinted Overlay", nullptr));
        hoverStyleComboBox->setItemText(2, QApplication::translate("FlatButtonSettingsForm", "Gray Overlay", nullptr));

        iconPlacementLabel->setText(QApplication::translate("FlatButtonSettingsForm", "Icon placement", nullptr));
        iconPlacementComboBox->setItemText(0, QApplication::translate("FlatButtonSettingsForm", "Left", nullptr));
        iconPlacementComboBox->setItemText(1, QApplication::translate("FlatButtonSettingsForm", "Right", nullptr));

        cornerRadiusLabel->setText(QApplication::translate("FlatButtonSettingsForm", "Corner radius", nullptr));
        overlayOpacityLabel->setText(QApplication::translate("FlatButtonSettingsForm", "Overlay opacity", nullptr));
        iconSizeLabel->setText(QApplication::translate("FlatButtonSettingsForm", "Icon size", nullptr));
        fontSizeLabel->setText(QApplication::translate("FlatButtonSettingsForm", "Font size", nullptr));
        buttonTextLabel->setText(QApplication::translate("FlatButtonSettingsForm", "Button text", nullptr));
        textAlignmentLabel->setText(QApplication::translate("FlatButtonSettingsForm", "Text alignment", nullptr));
        textAlignmentComboBox->setItemText(0, QApplication::translate("FlatButtonSettingsForm", "Left", nullptr));
        textAlignmentComboBox->setItemText(1, QApplication::translate("FlatButtonSettingsForm", "Center", nullptr));

        useThemeColorsCheckBox->setText(QApplication::translate("FlatButtonSettingsForm", "Use theme colors", nullptr));
        backgroundColorLabel->setText(QApplication::translate("FlatButtonSettingsForm", "Background color", nullptr));
        backgroundColorToolButton->setText(QApplication::translate("FlatButtonSettingsForm", "...", nullptr));
        foregroundColorLabel->setText(QApplication::translate("FlatButtonSettingsForm", "Foreground color", nullptr));
        foregroundColorToolButton->setText(QApplication::translate("FlatButtonSettingsForm", "...", nullptr));
        disabledBgColorLabel->setText(QApplication::translate("FlatButtonSettingsForm", "Disabled bg color", nullptr));
        disabledFgColorLabel->setText(QApplication::translate("FlatButtonSettingsForm", "Disabled fg color", nullptr));
        overlayColorLabel->setText(QApplication::translate("FlatButtonSettingsForm", "Overlay color", nullptr));
        overlayColorToolButton->setText(QApplication::translate("FlatButtonSettingsForm", "...", nullptr));
        disabledBgColorToolButton->setText(QApplication::translate("FlatButtonSettingsForm", "...", nullptr));
        disabledFgColorToolButton->setText(QApplication::translate("FlatButtonSettingsForm", "...", nullptr));
        defaultPresetPushButton->setText(QApplication::translate("FlatButtonSettingsForm", "Apply default preset", nullptr));
        checkablePresetPushButton->setText(QApplication::translate("FlatButtonSettingsForm", "Apply checkable preset", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FlatButtonSettingsForm: public Ui_FlatButtonSettingsForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FLATBUTTONSETTINGSFORM_H
