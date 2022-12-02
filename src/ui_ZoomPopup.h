/********************************************************************************
** Form generated from reading UI file 'ZoomPopup.ui'
**
** Created by: Qt User Interface Compiler version 5.12.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ZOOMPOPUP_H
#define UI_ZOOMPOPUP_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ZoomPopup
{
public:
    QHBoxLayout *horizontalLayout;
    QLineEdit *lineEdit;
    QToolButton *btnZoomOut;
    QToolButton *btnZoomIn;
    QPushButton *btnReset;

    void setupUi(QWidget *ZoomPopup)
    {
        if (ZoomPopup->objectName().isEmpty())
            ZoomPopup->setObjectName(QString::fromUtf8("ZoomPopup"));
        ZoomPopup->resize(250, 40);
        horizontalLayout = new QHBoxLayout(ZoomPopup);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(-1, 5, -1, 5);
        lineEdit = new QLineEdit(ZoomPopup);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setMinimumSize(QSize(0, 30));
        lineEdit->setStyleSheet(QString::fromUtf8("border:none;"));
        lineEdit->setReadOnly(true);

        horizontalLayout->addWidget(lineEdit);

        btnZoomOut = new QToolButton(ZoomPopup);
        btnZoomOut->setObjectName(QString::fromUtf8("btnZoomOut"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/resources/imgs/zoom_out_64px.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnZoomOut->setIcon(icon);

        horizontalLayout->addWidget(btnZoomOut);

        btnZoomIn = new QToolButton(ZoomPopup);
        btnZoomIn->setObjectName(QString::fromUtf8("btnZoomIn"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/icons/resources/imgs/zoom_in_64px.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnZoomIn->setIcon(icon1);

        horizontalLayout->addWidget(btnZoomIn);

        btnReset = new QPushButton(ZoomPopup);
        btnReset->setObjectName(QString::fromUtf8("btnReset"));
        btnReset->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"border:none;\n"
"padding:6px;\n"
"border-radius:4px;\n"
"background-color:#D3D3D3;\n"
"}\n"
"\n"
"QPushButton:hover{\n"
"background-color:#C0C0C0;\n"
"}"));

        horizontalLayout->addWidget(btnReset);


        retranslateUi(ZoomPopup);

        QMetaObject::connectSlotsByName(ZoomPopup);
    } // setupUi

    void retranslateUi(QWidget *ZoomPopup)
    {
        ZoomPopup->setWindowTitle(QApplication::translate("ZoomPopup", "Form", nullptr));
#ifndef QT_NO_TOOLTIP
        btnZoomOut->setToolTip(QApplication::translate("ZoomPopup", "zoom in", nullptr));
#endif // QT_NO_TOOLTIP
        btnZoomOut->setText(QString());
#ifndef QT_NO_TOOLTIP
        btnZoomIn->setToolTip(QApplication::translate("ZoomPopup", "zoom out", nullptr));
#endif // QT_NO_TOOLTIP
        btnZoomIn->setText(QString());
#ifndef QT_NO_TOOLTIP
        btnReset->setToolTip(QApplication::translate("ZoomPopup", "reset zoom level", nullptr));
#endif // QT_NO_TOOLTIP
        btnReset->setText(QApplication::translate("ZoomPopup", "Reset", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ZoomPopup: public Ui_ZoomPopup {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ZOOMPOPUP_H
