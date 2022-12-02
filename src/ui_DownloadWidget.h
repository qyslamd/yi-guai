/********************************************************************************
** Form generated from reading UI file 'DownloadWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.12.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DOWNLOADWIDGET_H
#define UI_DOWNLOADWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListView>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DownloadWidget
{
public:
    QVBoxLayout *verticalLayout_3;
    QStackedWidget *stackedWidget;
    QWidget *pageSearch;
    QHBoxLayout *horizontalLayout_2;
    QToolButton *buttonSearchBack;
    QLineEdit *lineEditSearch;
    QWidget *pageGeneral;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QSpacerItem *horizontalSpacer;
    QToolButton *buttonOpenFolder;
    QToolButton *buttonSearch;
    QToolButton *buttonMore;
    QToolButton *buttonPinOrClose;
    QListView *listView;

    void setupUi(QWidget *DownloadWidget)
    {
        if (DownloadWidget->objectName().isEmpty())
            DownloadWidget->setObjectName(QString::fromUtf8("DownloadWidget"));
        DownloadWidget->resize(414, 529);
        verticalLayout_3 = new QVBoxLayout(DownloadWidget);
        verticalLayout_3->setSpacing(0);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        stackedWidget = new QStackedWidget(DownloadWidget);
        stackedWidget->setObjectName(QString::fromUtf8("stackedWidget"));
        stackedWidget->setStyleSheet(QString::fromUtf8("QWidget#stackedWidget{\n"
"border:none;\n"
"border-bottom:1px solid #D2D2D2;\n"
"}"));
        pageSearch = new QWidget();
        pageSearch->setObjectName(QString::fromUtf8("pageSearch"));
        horizontalLayout_2 = new QHBoxLayout(pageSearch);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        buttonSearchBack = new QToolButton(pageSearch);
        buttonSearchBack->setObjectName(QString::fromUtf8("buttonSearchBack"));
        buttonSearchBack->setMinimumSize(QSize(30, 30));
        buttonSearchBack->setStyleSheet(QString::fromUtf8("QToolButton{\n"
"border:none;\n"
"}\n"
"QToolButton:hover{\n"
"border-radius:15px;\n"
"background-color:rgba(192,192,192,80%);\n"
"}\n"
"QToolButton::menu-indicator{\n"
"    image:none;\n"
"}"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/resources/imgs/arrow_back_left_64px.png"), QSize(), QIcon::Normal, QIcon::Off);
        buttonSearchBack->setIcon(icon);

        horizontalLayout_2->addWidget(buttonSearchBack);

        lineEditSearch = new QLineEdit(pageSearch);
        lineEditSearch->setObjectName(QString::fromUtf8("lineEditSearch"));
        lineEditSearch->setClearButtonEnabled(true);

        horizontalLayout_2->addWidget(lineEditSearch);

        stackedWidget->addWidget(pageSearch);
        pageGeneral = new QWidget();
        pageGeneral->setObjectName(QString::fromUtf8("pageGeneral"));
        pageGeneral->setStyleSheet(QString::fromUtf8(".QWidget{\n"
"background-color:white;\n"
"}\n"
"\n"
"QToolButton{\n"
"border:none;\n"
"}\n"
"QToolButton:hover{\n"
"border-radius:15px;\n"
"background-color:rgba(192,192,192,80%);\n"
"}\n"
"QToolButton::menu-indicator{\n"
"    image:none;\n"
"}"));
        horizontalLayout = new QHBoxLayout(pageGeneral);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(pageGeneral);
        label->setObjectName(QString::fromUtf8("label"));
        label->setStyleSheet(QString::fromUtf8("font: 75 12pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));

        horizontalLayout->addWidget(label);

        horizontalSpacer = new QSpacerItem(211, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        buttonOpenFolder = new QToolButton(pageGeneral);
        buttonOpenFolder->setObjectName(QString::fromUtf8("buttonOpenFolder"));
        buttonOpenFolder->setMinimumSize(QSize(30, 30));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/icons/resources/imgs/folder_48px.png"), QSize(), QIcon::Normal, QIcon::Off);
        buttonOpenFolder->setIcon(icon1);
        buttonOpenFolder->setIconSize(QSize(24, 24));

        horizontalLayout->addWidget(buttonOpenFolder);

        buttonSearch = new QToolButton(pageGeneral);
        buttonSearch->setObjectName(QString::fromUtf8("buttonSearch"));
        buttonSearch->setMinimumSize(QSize(30, 30));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/icons/resources/imgs/search2_64px.png"), QSize(), QIcon::Normal, QIcon::Off);
        buttonSearch->setIcon(icon2);
        buttonSearch->setIconSize(QSize(24, 24));

        horizontalLayout->addWidget(buttonSearch);

        buttonMore = new QToolButton(pageGeneral);
        buttonMore->setObjectName(QString::fromUtf8("buttonMore"));
        buttonMore->setMinimumSize(QSize(30, 30));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/icons/resources/imgs/more_60px.png"), QSize(), QIcon::Normal, QIcon::Off);
        buttonMore->setIcon(icon3);
        buttonMore->setIconSize(QSize(24, 24));

        horizontalLayout->addWidget(buttonMore);

        buttonPinOrClose = new QToolButton(pageGeneral);
        buttonPinOrClose->setObjectName(QString::fromUtf8("buttonPinOrClose"));
        buttonPinOrClose->setMinimumSize(QSize(30, 30));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/icons/resources/imgs/pin_64px.png"), QSize(), QIcon::Normal, QIcon::Off);
        buttonPinOrClose->setIcon(icon4);
        buttonPinOrClose->setIconSize(QSize(24, 24));

        horizontalLayout->addWidget(buttonPinOrClose);

        stackedWidget->addWidget(pageGeneral);

        verticalLayout_3->addWidget(stackedWidget);

        listView = new QListView(DownloadWidget);
        listView->setObjectName(QString::fromUtf8("listView"));
        listView->setFrameShape(QFrame::NoFrame);

        verticalLayout_3->addWidget(listView);

        verticalLayout_3->setStretch(1, 9);

        retranslateUi(DownloadWidget);

        stackedWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(DownloadWidget);
    } // setupUi

    void retranslateUi(QWidget *DownloadWidget)
    {
        DownloadWidget->setWindowTitle(QApplication::translate("DownloadWidget", "Form", nullptr));
        buttonSearchBack->setText(QString());
        label->setText(QApplication::translate("DownloadWidget", "Downloads", nullptr));
        buttonOpenFolder->setText(QString());
        buttonSearch->setText(QString());
        buttonMore->setText(QString());
        buttonPinOrClose->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class DownloadWidget: public Ui_DownloadWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DOWNLOADWIDGET_H
