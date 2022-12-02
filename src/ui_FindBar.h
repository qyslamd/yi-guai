/********************************************************************************
** Form generated from reading UI file 'FindBar.ui'
**
** Created by: Qt User Interface Compiler version 5.12.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FINDBAR_H
#define UI_FINDBAR_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QToolButton>

QT_BEGIN_NAMESPACE

class Ui_FindBar
{
public:
    QHBoxLayout *horizontalLayout_2;
    QFrame *frameShadow;
    QHBoxLayout *horizontalLayout;
    QLineEdit *lineEdit;
    QLabel *label;
    QFrame *line;
    QToolButton *buttonUp;
    QToolButton *buttonDown;
    QToolButton *buttonClose;

    void setupUi(QDialog *FindBar)
    {
        if (FindBar->objectName().isEmpty())
            FindBar->setObjectName(QString::fromUtf8("FindBar"));
        FindBar->resize(391, 59);
        horizontalLayout_2 = new QHBoxLayout(FindBar);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(-1, 2, -1, 12);
        frameShadow = new QFrame(FindBar);
        frameShadow->setObjectName(QString::fromUtf8("frameShadow"));
        frameShadow->setStyleSheet(QString::fromUtf8("QFrame#frameShadow{\n"
"border-radius:3px;\n"
"background-color:white;\n"
"}\n"
"QToolButton{\n"
"border-radius:4px;\n"
"border:none;\n"
"}\n"
"QToolButton:hover{\n"
"background-color:#EBEBEB;\n"
"}"));
        frameShadow->setFrameShape(QFrame::StyledPanel);
        frameShadow->setFrameShadow(QFrame::Raised);
        horizontalLayout = new QHBoxLayout(frameShadow);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        lineEdit = new QLineEdit(frameShadow);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setMinimumSize(QSize(230, 24));
        lineEdit->setStyleSheet(QString::fromUtf8("QLineEdit{\n"
"border:none;\n"
"}"));

        horizontalLayout->addWidget(lineEdit);

        label = new QLabel(frameShadow);
        label->setObjectName(QString::fromUtf8("label"));
        label->setText(QString::fromUtf8("0/0"));

        horizontalLayout->addWidget(label);

        line = new QFrame(frameShadow);
        line->setObjectName(QString::fromUtf8("line"));
        line->setFrameShape(QFrame::VLine);
        line->setFrameShadow(QFrame::Sunken);

        horizontalLayout->addWidget(line);

        buttonUp = new QToolButton(frameShadow);
        buttonUp->setObjectName(QString::fromUtf8("buttonUp"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/Resources/imgs/search_up.png"), QSize(), QIcon::Normal, QIcon::Off);
        buttonUp->setIcon(icon);
        buttonUp->setIconSize(QSize(24, 24));

        horizontalLayout->addWidget(buttonUp);

        buttonDown = new QToolButton(frameShadow);
        buttonDown->setObjectName(QString::fromUtf8("buttonDown"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/Resources/imgs/search_down.png"), QSize(), QIcon::Normal, QIcon::Off);
        buttonDown->setIcon(icon1);
        buttonDown->setIconSize(QSize(24, 24));

        horizontalLayout->addWidget(buttonDown);

        buttonClose = new QToolButton(frameShadow);
        buttonClose->setObjectName(QString::fromUtf8("buttonClose"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/Resources/imgs/window_close_light_2x.png"), QSize(), QIcon::Normal, QIcon::Off);
        buttonClose->setIcon(icon2);
        buttonClose->setIconSize(QSize(24, 24));

        horizontalLayout->addWidget(buttonClose);


        horizontalLayout_2->addWidget(frameShadow);


        retranslateUi(FindBar);

        QMetaObject::connectSlotsByName(FindBar);
    } // setupUi

    void retranslateUi(QDialog *FindBar)
    {
        FindBar->setWindowTitle(QApplication::translate("FindBar", "Form", nullptr));
#ifndef QT_NO_TOOLTIP
        buttonUp->setToolTip(QApplication::translate("FindBar", "previous", nullptr));
#endif // QT_NO_TOOLTIP
        buttonUp->setText(QString());
#ifndef QT_NO_TOOLTIP
        buttonDown->setToolTip(QApplication::translate("FindBar", "next", nullptr));
#endif // QT_NO_TOOLTIP
        buttonDown->setText(QString());
#ifndef QT_NO_TOOLTIP
        buttonClose->setToolTip(QApplication::translate("FindBar", "close", nullptr));
#endif // QT_NO_TOOLTIP
        buttonClose->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class FindBar: public Ui_FindBar {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FINDBAR_H
