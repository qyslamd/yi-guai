/********************************************************************************
** Form generated from reading UI file 'NotificationBar.ui'
**
** Created by: Qt User Interface Compiler version 5.12.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NOTIFICATIONBAR_H
#define UI_NOTIFICATIONBAR_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QToolButton>

QT_BEGIN_NAMESPACE

class Ui_NotificationBar
{
public:
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer;
    QLabel *labelIcon;
    QLabel *labelText;
    QSpacerItem *horizontalSpacer_2;
    QHBoxLayout *horizontalLayout;
    QPushButton *btnComfirm;
    QPushButton *btnCancel;
    QSpacerItem *horizontalSpacer_3;
    QToolButton *btnClose;

    void setupUi(QFrame *NotificationBar)
    {
        if (NotificationBar->objectName().isEmpty())
            NotificationBar->setObjectName(QString::fromUtf8("NotificationBar"));
        NotificationBar->resize(802, 43);
        NotificationBar->setStyleSheet(QString::fromUtf8(""));
        horizontalLayout_2 = new QHBoxLayout(NotificationBar);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalSpacer = new QSpacerItem(120, 20, QSizePolicy::Maximum, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        labelIcon = new QLabel(NotificationBar);
        labelIcon->setObjectName(QString::fromUtf8("labelIcon"));
        labelIcon->setMinimumSize(QSize(24, 24));
        labelIcon->setMaximumSize(QSize(24, 24));
        labelIcon->setText(QString::fromUtf8(""));
        labelIcon->setPixmap(QPixmap(QString::fromUtf8(":/icons/resources/imgs/normal_notify.png")));
        labelIcon->setScaledContents(true);
        labelIcon->setAlignment(Qt::AlignCenter);

        horizontalLayout_2->addWidget(labelIcon);

        labelText = new QLabel(NotificationBar);
        labelText->setObjectName(QString::fromUtf8("labelText"));
        labelText->setText(QString::fromUtf8("TextLabel"));

        horizontalLayout_2->addWidget(labelText);

        horizontalSpacer_2 = new QSpacerItem(240, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        btnComfirm = new QPushButton(NotificationBar);
        btnComfirm->setObjectName(QString::fromUtf8("btnComfirm"));

        horizontalLayout->addWidget(btnComfirm);

        btnCancel = new QPushButton(NotificationBar);
        btnCancel->setObjectName(QString::fromUtf8("btnCancel"));

        horizontalLayout->addWidget(btnCancel);


        horizontalLayout_2->addLayout(horizontalLayout);

        horizontalSpacer_3 = new QSpacerItem(120, 20, QSizePolicy::Maximum, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_3);

        btnClose = new QToolButton(NotificationBar);
        btnClose->setObjectName(QString::fromUtf8("btnClose"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/resources/imgs/close_24px.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnClose->setIcon(icon);

        horizontalLayout_2->addWidget(btnClose);


        retranslateUi(NotificationBar);

        QMetaObject::connectSlotsByName(NotificationBar);
    } // setupUi

    void retranslateUi(QFrame *NotificationBar)
    {
        NotificationBar->setWindowTitle(QApplication::translate("NotificationBar", "Frame", nullptr));
        btnComfirm->setText(QApplication::translate("NotificationBar", "Comfirm", nullptr));
        btnCancel->setText(QApplication::translate("NotificationBar", "Cancel", nullptr));
        btnClose->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class NotificationBar: public Ui_NotificationBar {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NOTIFICATIONBAR_H
