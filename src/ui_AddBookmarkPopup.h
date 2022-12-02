/********************************************************************************
** Form generated from reading UI file 'AddBookmarkPopup.ui'
**
** Created by: Qt User Interface Compiler version 5.12.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ADDBOOKMARKPOPUP_H
#define UI_ADDBOOKMARKPOPUP_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AddBkmkPopup
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QSpacerItem *horizontalSpacer;
    QToolButton *btnClose;
    QFormLayout *formLayout;
    QLabel *label_2;
    QLineEdit *lineEditName;
    QLabel *label_3;
    QComboBox *comboBoxFolder;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *btnMore;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *btnFinish;
    QPushButton *btnRemove;

    void setupUi(QWidget *AddBkmkPopup)
    {
        if (AddBkmkPopup->objectName().isEmpty())
            AddBkmkPopup->setObjectName(QString::fromUtf8("AddBkmkPopup"));
        AddBkmkPopup->resize(320, 180);
        AddBkmkPopup->setWindowTitle(QString::fromUtf8("Form"));
        verticalLayout = new QVBoxLayout(AddBkmkPopup);
        verticalLayout->setSpacing(8);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(-1, 6, 2, -1);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(AddBkmkPopup);
        label->setObjectName(QString::fromUtf8("label"));
        label->setStyleSheet(QString::fromUtf8("font: 75 10pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));

        horizontalLayout->addWidget(label);

        horizontalSpacer = new QSpacerItem(238, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        btnClose = new QToolButton(AddBkmkPopup);
        btnClose->setObjectName(QString::fromUtf8("btnClose"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/resources/imgs/general_close.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnClose->setIcon(icon);

        horizontalLayout->addWidget(btnClose);


        verticalLayout->addLayout(horizontalLayout);

        formLayout = new QFormLayout();
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        formLayout->setHorizontalSpacing(16);
        formLayout->setVerticalSpacing(12);
        formLayout->setContentsMargins(-1, -1, 12, -1);
        label_2 = new QLabel(AddBkmkPopup);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        formLayout->setWidget(0, QFormLayout::LabelRole, label_2);

        lineEditName = new QLineEdit(AddBkmkPopup);
        lineEditName->setObjectName(QString::fromUtf8("lineEditName"));
        lineEditName->setMinimumSize(QSize(0, 30));

        formLayout->setWidget(0, QFormLayout::FieldRole, lineEditName);

        label_3 = new QLabel(AddBkmkPopup);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        formLayout->setWidget(1, QFormLayout::LabelRole, label_3);

        comboBoxFolder = new QComboBox(AddBkmkPopup);
        comboBoxFolder->setObjectName(QString::fromUtf8("comboBoxFolder"));
        comboBoxFolder->setMinimumSize(QSize(0, 30));

        formLayout->setWidget(1, QFormLayout::FieldRole, comboBoxFolder);


        verticalLayout->addLayout(formLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(-1, -1, 12, -1);
        btnMore = new QPushButton(AddBkmkPopup);
        btnMore->setObjectName(QString::fromUtf8("btnMore"));
        btnMore->setMinimumSize(QSize(0, 32));

        horizontalLayout_2->addWidget(btnMore);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        btnFinish = new QPushButton(AddBkmkPopup);
        btnFinish->setObjectName(QString::fromUtf8("btnFinish"));
        btnFinish->setMinimumSize(QSize(0, 32));

        horizontalLayout_2->addWidget(btnFinish);

        btnRemove = new QPushButton(AddBkmkPopup);
        btnRemove->setObjectName(QString::fromUtf8("btnRemove"));
        btnRemove->setMinimumSize(QSize(0, 32));

        horizontalLayout_2->addWidget(btnRemove);


        verticalLayout->addLayout(horizontalLayout_2);


        retranslateUi(AddBkmkPopup);

        QMetaObject::connectSlotsByName(AddBkmkPopup);
    } // setupUi

    void retranslateUi(QWidget *AddBkmkPopup)
    {
        label->setText(QApplication::translate("AddBkmkPopup", "\345\267\262\346\267\273\345\212\240\345\210\260\346\224\266\350\227\217\345\244\271", nullptr));
        btnClose->setText(QString());
        label_2->setText(QApplication::translate("AddBkmkPopup", "\345\220\215\347\247\260", nullptr));
        label_3->setText(QApplication::translate("AddBkmkPopup", "\346\226\207\344\273\266\345\244\271", nullptr));
        btnMore->setText(QApplication::translate("AddBkmkPopup", "\346\233\264\345\244\232", nullptr));
        btnFinish->setText(QApplication::translate("AddBkmkPopup", "\345\256\214\346\210\220", nullptr));
        btnRemove->setText(QApplication::translate("AddBkmkPopup", "\345\210\240\351\231\244", nullptr));
        Q_UNUSED(AddBkmkPopup);
    } // retranslateUi

};

namespace Ui {
    class AddBkmkPopup: public Ui_AddBkmkPopup {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ADDBOOKMARKPOPUP_H
