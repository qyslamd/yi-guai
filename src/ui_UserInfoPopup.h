/********************************************************************************
** Form generated from reading UI file 'UserInfoPopup.ui'
**
** Created by: Qt User Interface Compiler version 5.12.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_USERINFOPOPUP_H
#define UI_USERINFOPOPUP_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_UserInfoPopup
{
public:
    QVBoxLayout *verticalLayout_2;
    QFrame *frame;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *lineEditName;
    QToolButton *btnRename;
    QSpacerItem *horizontalSpacer_2;
    QToolButton *btnClose;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_4;
    QLabel *labelPic;
    QSpacerItem *horizontalSpacer_5;
    QFrame *frame_2;
    QFormLayout *formLayout;
    QLabel *label_2;
    QPlainTextEdit *editOS;
    QLabel *label_3;
    QPlainTextEdit *editDevice;
    QLabel *label_5;
    QPlainTextEdit *editCPU;
    QLabel *label_4;
    QPlainTextEdit *editRAM;
    QHBoxLayout *horizontalLayout_4;
    QPushButton *btnCopyInfo;
    QSpacerItem *horizontalSpacer;

    void setupUi(QWidget *UserInfoPopup)
    {
        if (UserInfoPopup->objectName().isEmpty())
            UserInfoPopup->setObjectName(QString::fromUtf8("UserInfoPopup"));
        UserInfoPopup->resize(245, 375);
        verticalLayout_2 = new QVBoxLayout(UserInfoPopup);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(-1, 9, -1, 9);
        frame = new QFrame(UserInfoPopup);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        verticalLayout = new QVBoxLayout(frame);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(frame);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout->addWidget(label);

        lineEditName = new QLineEdit(frame);
        lineEditName->setObjectName(QString::fromUtf8("lineEditName"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(lineEditName->sizePolicy().hasHeightForWidth());
        lineEditName->setSizePolicy(sizePolicy);
        lineEditName->setMinimumSize(QSize(0, 26));
        lineEditName->setStyleSheet(QString::fromUtf8("QLineEdit{\n"
"border:none;\n"
"font-size:11pt;\n"
"}"));
        lineEditName->setFrame(true);
        lineEditName->setReadOnly(true);

        horizontalLayout->addWidget(lineEditName);

        btnRename = new QToolButton(frame);
        btnRename->setObjectName(QString::fromUtf8("btnRename"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/resources/newIcons/quill_pen_48px.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnRename->setIcon(icon);
        btnRename->setIconSize(QSize(20, 20));

        horizontalLayout->addWidget(btnRename);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        btnClose = new QToolButton(frame);
        btnClose->setObjectName(QString::fromUtf8("btnClose"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/icons/resources/imgs/general_close.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnClose->setIcon(icon1);

        horizontalLayout->addWidget(btnClose);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalSpacer_4 = new QSpacerItem(46, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_4);

        labelPic = new QLabel(frame);
        labelPic->setObjectName(QString::fromUtf8("labelPic"));
        labelPic->setMinimumSize(QSize(48, 48));
        labelPic->setMaximumSize(QSize(48, 48));
        labelPic->setStyleSheet(QString::fromUtf8("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 rgba(234,229,201, 0.8), stop:1 rgba(108,198,203, 0.3));"));
        labelPic->setPixmap(QPixmap(QString::fromUtf8(":/icons/resources/newIcons/dragon_face_96px.png")));
        labelPic->setScaledContents(true);

        horizontalLayout_2->addWidget(labelPic);

        horizontalSpacer_5 = new QSpacerItem(58, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_5);


        verticalLayout->addLayout(horizontalLayout_2);


        verticalLayout_2->addWidget(frame);

        frame_2 = new QFrame(UserInfoPopup);
        frame_2->setObjectName(QString::fromUtf8("frame_2"));
        frame_2->setStyleSheet(QString::fromUtf8("QFrame#frame_2{\n"
"border:1px solid #DCDCDC;\n"
"border-radius:8px;\n"
"background-color:#F5F5F5;\n"
"}\n"
"QLabel{\n"
"font-size:10pt;\n"
"font-weight:bold;\n"
"color:#696969;\n"
"}\n"
"\n"
"QPlainTextEdit{\n"
"border:none;\n"
"}"));
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        formLayout = new QFormLayout(frame_2);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        label_2 = new QLabel(frame_2);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setStyleSheet(QString::fromUtf8(""));

        formLayout->setWidget(0, QFormLayout::LabelRole, label_2);

        editOS = new QPlainTextEdit(frame_2);
        editOS->setObjectName(QString::fromUtf8("editOS"));
        editOS->setMaximumSize(QSize(16777215, 46));
        editOS->setReadOnly(true);

        formLayout->setWidget(0, QFormLayout::FieldRole, editOS);

        label_3 = new QLabel(frame_2);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        formLayout->setWidget(1, QFormLayout::LabelRole, label_3);

        editDevice = new QPlainTextEdit(frame_2);
        editDevice->setObjectName(QString::fromUtf8("editDevice"));
        editDevice->setMaximumSize(QSize(16777215, 46));
        editDevice->setReadOnly(true);

        formLayout->setWidget(1, QFormLayout::FieldRole, editDevice);

        label_5 = new QLabel(frame_2);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        formLayout->setWidget(2, QFormLayout::LabelRole, label_5);

        editCPU = new QPlainTextEdit(frame_2);
        editCPU->setObjectName(QString::fromUtf8("editCPU"));
        editCPU->setMaximumSize(QSize(16777215, 46));
        editCPU->setReadOnly(true);

        formLayout->setWidget(2, QFormLayout::FieldRole, editCPU);

        label_4 = new QLabel(frame_2);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        formLayout->setWidget(3, QFormLayout::LabelRole, label_4);

        editRAM = new QPlainTextEdit(frame_2);
        editRAM->setObjectName(QString::fromUtf8("editRAM"));
        editRAM->setMaximumSize(QSize(16777215, 46));
        editRAM->setReadOnly(true);

        formLayout->setWidget(3, QFormLayout::FieldRole, editRAM);


        verticalLayout_2->addWidget(frame_2);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        btnCopyInfo = new QPushButton(UserInfoPopup);
        btnCopyInfo->setObjectName(QString::fromUtf8("btnCopyInfo"));
        btnCopyInfo->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"border-radius:4px;\n"
"border:none;\n"
"background-color: #DCDCDC;\n"
"padding:8px;\n"
"}\n"
"QPushButton:hover{\n"
"background-color: #DCDCDC;\n"
"}\n"
""));

        horizontalLayout_4->addWidget(btnCopyInfo);

        horizontalSpacer = new QSpacerItem(228, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer);


        verticalLayout_2->addLayout(horizontalLayout_4);


        retranslateUi(UserInfoPopup);

        QMetaObject::connectSlotsByName(UserInfoPopup);
    } // setupUi

    void retranslateUi(QWidget *UserInfoPopup)
    {
        UserInfoPopup->setWindowTitle(QApplication::translate("UserInfoPopup", "User", nullptr));
        label->setText(QApplication::translate("UserInfoPopup", "welcome", nullptr));
#ifndef QT_NO_TOOLTIP
        lineEditName->setToolTip(QApplication::translate("UserInfoPopup", "your name", nullptr));
#endif // QT_NO_TOOLTIP
        lineEditName->setText(QApplication::translate("UserInfoPopup", "slamdd", nullptr));
        btnRename->setText(QString());
#ifndef QT_NO_TOOLTIP
        btnClose->setToolTip(QApplication::translate("UserInfoPopup", "close", nullptr));
#endif // QT_NO_TOOLTIP
        btnClose->setText(QString());
        labelPic->setText(QString());
        label_2->setText(QApplication::translate("UserInfoPopup", "OS", nullptr));
        label_3->setText(QApplication::translate("UserInfoPopup", "Dev", nullptr));
        label_5->setText(QApplication::translate("UserInfoPopup", "CPU", nullptr));
        label_4->setText(QApplication::translate("UserInfoPopup", "RAM", nullptr));
        btnCopyInfo->setText(QApplication::translate("UserInfoPopup", "Copy info", nullptr));
    } // retranslateUi

};

namespace Ui {
    class UserInfoPopup: public Ui_UserInfoPopup {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_USERINFOPOPUP_H
