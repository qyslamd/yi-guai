/********************************************************************************
** Form generated from reading UI file 'BookmarkWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.12.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BOOKMARKWIDGET_H
#define UI_BOOKMARKWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_BookmarkWidget
{
public:
    QVBoxLayout *verticalLayout_2;
    QStackedWidget *stackedWidget;
    QWidget *pageGeneral;
    QVBoxLayout *verticalLayout;
    QFrame *frameGeneralTop;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QSpacerItem *horizontalSpacer;
    QToolButton *buttonAddBookmark;
    QToolButton *buttonAddBkmkDir;
    QToolButton *buttonSearch;
    QToolButton *buttonMore;
    QToolButton *buttonPinOrClose;
    QTreeView *treeView;
    QWidget *pageSearch;
    QVBoxLayout *verticalLayout_3;
    QFrame *frameSearchTop;
    QHBoxLayout *horizontalLayout_2;
    QToolButton *buttonSearhBack;
    QLineEdit *lineEditSearch;
    QListWidget *listWidget;

    void setupUi(QWidget *BookmarkWidget)
    {
        if (BookmarkWidget->objectName().isEmpty())
            BookmarkWidget->setObjectName(QString::fromUtf8("BookmarkWidget"));
        BookmarkWidget->resize(350, 500);
        verticalLayout_2 = new QVBoxLayout(BookmarkWidget);
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        stackedWidget = new QStackedWidget(BookmarkWidget);
        stackedWidget->setObjectName(QString::fromUtf8("stackedWidget"));
        stackedWidget->setStyleSheet(QString::fromUtf8(""));
        pageGeneral = new QWidget();
        pageGeneral->setObjectName(QString::fromUtf8("pageGeneral"));
        pageGeneral->setStyleSheet(QString::fromUtf8(""));
        verticalLayout = new QVBoxLayout(pageGeneral);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        frameGeneralTop = new QFrame(pageGeneral);
        frameGeneralTop->setObjectName(QString::fromUtf8("frameGeneralTop"));
        frameGeneralTop->setStyleSheet(QString::fromUtf8(".QFrame{\n"
"border:none;\n"
"border-bottom:1px solid #D2D2D2;\n"
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
        frameGeneralTop->setFrameShape(QFrame::StyledPanel);
        frameGeneralTop->setFrameShadow(QFrame::Raised);
        horizontalLayout = new QHBoxLayout(frameGeneralTop);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(-1, 4, -1, 4);
        label = new QLabel(frameGeneralTop);
        label->setObjectName(QString::fromUtf8("label"));
        label->setStyleSheet(QString::fromUtf8("font: 75 12pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));

        horizontalLayout->addWidget(label);

        horizontalSpacer = new QSpacerItem(103, 19, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        buttonAddBookmark = new QToolButton(frameGeneralTop);
        buttonAddBookmark->setObjectName(QString::fromUtf8("buttonAddBookmark"));
        buttonAddBookmark->setMinimumSize(QSize(30, 30));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/resources/imgs/add_to_favorites_52px.png"), QSize(), QIcon::Normal, QIcon::Off);
        buttonAddBookmark->setIcon(icon);
        buttonAddBookmark->setIconSize(QSize(24, 24));

        horizontalLayout->addWidget(buttonAddBookmark);

        buttonAddBkmkDir = new QToolButton(frameGeneralTop);
        buttonAddBkmkDir->setObjectName(QString::fromUtf8("buttonAddBkmkDir"));
        buttonAddBkmkDir->setMinimumSize(QSize(30, 30));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/icons/resources/imgs/add_folder_60px.png"), QSize(), QIcon::Normal, QIcon::Off);
        buttonAddBkmkDir->setIcon(icon1);
        buttonAddBkmkDir->setIconSize(QSize(24, 24));

        horizontalLayout->addWidget(buttonAddBkmkDir);

        buttonSearch = new QToolButton(frameGeneralTop);
        buttonSearch->setObjectName(QString::fromUtf8("buttonSearch"));
        buttonSearch->setMinimumSize(QSize(30, 30));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/icons/resources/imgs/search2_64px.png"), QSize(), QIcon::Normal, QIcon::Off);
        buttonSearch->setIcon(icon2);
        buttonSearch->setIconSize(QSize(24, 24));

        horizontalLayout->addWidget(buttonSearch);

        buttonMore = new QToolButton(frameGeneralTop);
        buttonMore->setObjectName(QString::fromUtf8("buttonMore"));
        buttonMore->setMinimumSize(QSize(30, 30));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/icons/resources/imgs/more_60px.png"), QSize(), QIcon::Normal, QIcon::Off);
        buttonMore->setIcon(icon3);
        buttonMore->setIconSize(QSize(24, 24));

        horizontalLayout->addWidget(buttonMore);

        buttonPinOrClose = new QToolButton(frameGeneralTop);
        buttonPinOrClose->setObjectName(QString::fromUtf8("buttonPinOrClose"));
        buttonPinOrClose->setMinimumSize(QSize(30, 30));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/icons/resources/imgs/pin_64px.png"), QSize(), QIcon::Normal, QIcon::Off);
        buttonPinOrClose->setIcon(icon4);
        buttonPinOrClose->setIconSize(QSize(24, 24));

        horizontalLayout->addWidget(buttonPinOrClose);


        verticalLayout->addWidget(frameGeneralTop);

        treeView = new QTreeView(pageGeneral);
        treeView->setObjectName(QString::fromUtf8("treeView"));
        treeView->setFrameShape(QFrame::NoFrame);
        treeView->setFrameShadow(QFrame::Plain);
        treeView->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
        treeView->setUniformRowHeights(true);
        treeView->setHeaderHidden(true);
        treeView->header()->setDefaultSectionSize(20);

        verticalLayout->addWidget(treeView);

        stackedWidget->addWidget(pageGeneral);
        pageSearch = new QWidget();
        pageSearch->setObjectName(QString::fromUtf8("pageSearch"));
        verticalLayout_3 = new QVBoxLayout(pageSearch);
        verticalLayout_3->setSpacing(0);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        frameSearchTop = new QFrame(pageSearch);
        frameSearchTop->setObjectName(QString::fromUtf8("frameSearchTop"));
        frameSearchTop->setStyleSheet(QString::fromUtf8(".QFrame{\n"
"border:none;\n"
"border-bottom:1px solid #D2D2D2;\n"
"background-color:white;\n"
"}"));
        frameSearchTop->setFrameShape(QFrame::StyledPanel);
        frameSearchTop->setFrameShadow(QFrame::Raised);
        horizontalLayout_2 = new QHBoxLayout(frameSearchTop);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        buttonSearhBack = new QToolButton(frameSearchTop);
        buttonSearhBack->setObjectName(QString::fromUtf8("buttonSearhBack"));
        buttonSearhBack->setMinimumSize(QSize(30, 30));
        buttonSearhBack->setStyleSheet(QString::fromUtf8("QToolButton{\n"
"border:none;\n"
"}\n"
"QToolButton:hover{\n"
"border-radius:15px;\n"
"background-color:rgba(192,192,192,80%);\n"
"}\n"
"QToolButton::menu-indicator{\n"
"    image:none;\n"
"}"));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/icons/resources/imgs/arrow_back_left2_64px.png"), QSize(), QIcon::Normal, QIcon::Off);
        buttonSearhBack->setIcon(icon5);

        horizontalLayout_2->addWidget(buttonSearhBack);

        lineEditSearch = new QLineEdit(frameSearchTop);
        lineEditSearch->setObjectName(QString::fromUtf8("lineEditSearch"));
        lineEditSearch->setClearButtonEnabled(true);

        horizontalLayout_2->addWidget(lineEditSearch);


        verticalLayout_3->addWidget(frameSearchTop);

        listWidget = new QListWidget(pageSearch);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));
        listWidget->setFrameShape(QFrame::NoFrame);
        listWidget->setFrameShadow(QFrame::Plain);

        verticalLayout_3->addWidget(listWidget);

        stackedWidget->addWidget(pageSearch);

        verticalLayout_2->addWidget(stackedWidget);


        retranslateUi(BookmarkWidget);

        stackedWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(BookmarkWidget);
    } // setupUi

    void retranslateUi(QWidget *BookmarkWidget)
    {
        BookmarkWidget->setWindowTitle(QApplication::translate("BookmarkWidget", "Form", nullptr));
        label->setText(QApplication::translate("BookmarkWidget", "Bookmarks", nullptr));
#ifndef QT_NO_TOOLTIP
        buttonAddBookmark->setToolTip(QApplication::translate("BookmarkWidget", "add current to favorites", nullptr));
#endif // QT_NO_TOOLTIP
        buttonAddBookmark->setText(QString());
#ifndef QT_NO_TOOLTIP
        buttonAddBkmkDir->setToolTip(QApplication::translate("BookmarkWidget", "add bookmark folder", nullptr));
#endif // QT_NO_TOOLTIP
        buttonAddBkmkDir->setText(QString());
#ifndef QT_NO_TOOLTIP
        buttonSearch->setToolTip(QApplication::translate("BookmarkWidget", "search in bookmarks", nullptr));
#endif // QT_NO_TOOLTIP
        buttonSearch->setText(QString());
#ifndef QT_NO_TOOLTIP
        buttonMore->setToolTip(QApplication::translate("BookmarkWidget", "more options", nullptr));
#endif // QT_NO_TOOLTIP
        buttonMore->setText(QString());
#ifndef QT_NO_TOOLTIP
        buttonPinOrClose->setToolTip(QApplication::translate("BookmarkWidget", "pin bookmark", nullptr));
#endif // QT_NO_TOOLTIP
        buttonPinOrClose->setText(QString());
        buttonSearhBack->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class BookmarkWidget: public Ui_BookmarkWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BOOKMARKWIDGET_H
