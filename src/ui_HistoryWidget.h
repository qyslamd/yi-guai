/********************************************************************************
** Form generated from reading UI file 'HistoryWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.12.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HISTORYWIDGET_H
#define UI_HISTORYWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListView>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_HistoryWidget
{
public:
    QVBoxLayout *verticalLayout_3;
    QStackedWidget *stackedWidget;
    QWidget *pageSearch;
    QVBoxLayout *verticalLayout_2;
    QFrame *frameSearchTop;
    QHBoxLayout *horizontalLayout_3;
    QToolButton *buttonSearchBack;
    QLineEdit *lineEditSearch;
    QListWidget *listWidgetSearch;
    QWidget *pageGeneral;
    QVBoxLayout *verticalLayout;
    QFrame *frameGeneralTop;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QSpacerItem *horizontalSpacer;
    QToolButton *buttonSearch;
    QToolButton *buttonMore;
    QToolButton *buttonPinOrClose;
    QTabWidget *tabWidget;
    QWidget *tabAll;
    QHBoxLayout *horizontalLayout_4;
    QTreeView *treeViewAll;
    QWidget *tabRecentlyClosed;
    QHBoxLayout *horizontalLayout_5;
    QListView *listViewRecently;

    void setupUi(QWidget *HistoryWidget)
    {
        if (HistoryWidget->objectName().isEmpty())
            HistoryWidget->setObjectName(QString::fromUtf8("HistoryWidget"));
        HistoryWidget->resize(414, 529);
        verticalLayout_3 = new QVBoxLayout(HistoryWidget);
        verticalLayout_3->setSpacing(0);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(1, 1, 1, 1);
        stackedWidget = new QStackedWidget(HistoryWidget);
        stackedWidget->setObjectName(QString::fromUtf8("stackedWidget"));
        stackedWidget->setStyleSheet(QString::fromUtf8(""));
        pageSearch = new QWidget();
        pageSearch->setObjectName(QString::fromUtf8("pageSearch"));
        verticalLayout_2 = new QVBoxLayout(pageSearch);
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        frameSearchTop = new QFrame(pageSearch);
        frameSearchTop->setObjectName(QString::fromUtf8("frameSearchTop"));
        frameSearchTop->setStyleSheet(QString::fromUtf8(".QFrame{\n"
"border:none;\n"
"border-bottom:1px solid #D2D2D2;\n"
"}"));
        frameSearchTop->setFrameShape(QFrame::StyledPanel);
        frameSearchTop->setFrameShadow(QFrame::Raised);
        horizontalLayout_3 = new QHBoxLayout(frameSearchTop);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        buttonSearchBack = new QToolButton(frameSearchTop);
        buttonSearchBack->setObjectName(QString::fromUtf8("buttonSearchBack"));
        buttonSearchBack->setMinimumSize(QSize(30, 30));
        buttonSearchBack->setStyleSheet(QString::fromUtf8(".QFrame{\n"
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
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/resources/imgs/arrow_back_left_64px.png"), QSize(), QIcon::Normal, QIcon::Off);
        buttonSearchBack->setIcon(icon);

        horizontalLayout_3->addWidget(buttonSearchBack);

        lineEditSearch = new QLineEdit(frameSearchTop);
        lineEditSearch->setObjectName(QString::fromUtf8("lineEditSearch"));
        lineEditSearch->setClearButtonEnabled(true);

        horizontalLayout_3->addWidget(lineEditSearch);


        verticalLayout_2->addWidget(frameSearchTop);

        listWidgetSearch = new QListWidget(pageSearch);
        listWidgetSearch->setObjectName(QString::fromUtf8("listWidgetSearch"));
        listWidgetSearch->setFrameShape(QFrame::NoFrame);
        listWidgetSearch->setFrameShadow(QFrame::Plain);

        verticalLayout_2->addWidget(listWidgetSearch);

        stackedWidget->addWidget(pageSearch);
        pageGeneral = new QWidget();
        pageGeneral->setObjectName(QString::fromUtf8("pageGeneral"));
        verticalLayout = new QVBoxLayout(pageGeneral);
        verticalLayout->setSpacing(2);
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
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(-1, 4, -1, 4);
        label = new QLabel(frameGeneralTop);
        label->setObjectName(QString::fromUtf8("label"));
        label->setStyleSheet(QString::fromUtf8("font: 75 12pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));

        horizontalLayout->addWidget(label);

        horizontalSpacer = new QSpacerItem(186, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        buttonSearch = new QToolButton(frameGeneralTop);
        buttonSearch->setObjectName(QString::fromUtf8("buttonSearch"));
        buttonSearch->setMinimumSize(QSize(30, 30));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/icons/resources/imgs/search2_64px.png"), QSize(), QIcon::Normal, QIcon::Off);
        buttonSearch->setIcon(icon1);
        buttonSearch->setIconSize(QSize(24, 24));

        horizontalLayout->addWidget(buttonSearch);

        buttonMore = new QToolButton(frameGeneralTop);
        buttonMore->setObjectName(QString::fromUtf8("buttonMore"));
        buttonMore->setMinimumSize(QSize(30, 30));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/icons/resources/imgs/ios7/more_50px.png"), QSize(), QIcon::Normal, QIcon::Off);
        buttonMore->setIcon(icon2);
        buttonMore->setIconSize(QSize(24, 24));

        horizontalLayout->addWidget(buttonMore);

        buttonPinOrClose = new QToolButton(frameGeneralTop);
        buttonPinOrClose->setObjectName(QString::fromUtf8("buttonPinOrClose"));
        buttonPinOrClose->setMinimumSize(QSize(30, 30));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/icons/resources/imgs/pin_64px.png"), QSize(), QIcon::Normal, QIcon::Off);
        buttonPinOrClose->setIcon(icon3);
        buttonPinOrClose->setIconSize(QSize(24, 24));

        horizontalLayout->addWidget(buttonPinOrClose);


        verticalLayout->addWidget(frameGeneralTop);

        tabWidget = new QTabWidget(pageGeneral);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setStyleSheet(QString::fromUtf8("QTabWidget {\n"
"border: none;\n"
"}\n"
"QTabWidget::pane {\n"
"border: none;\n"
"top:4px;\n"
" }\n"
"QTabWidget::tab-bar {\n"
"alignment: center;\n"
"}\n"
"QTabBar::tab {\n"
"    border:none;\n"
"    background: #FFFFFF;\n"
"    width: 80px;\n"
"    height: 30px;\n"
"    color: #999999;\n"
"}\n"
"QTabBar::tab:hover {\n"
"    color:#84b2fc;\n"
"}\n"
"QTabBar::tab:selected {\n"
"color:black;\n"
"color:#4285f4;\n"
"font:bold;\n"
"border-bottom:2px solid #5183D5;\n"
"}\n"
""));
        tabAll = new QWidget();
        tabAll->setObjectName(QString::fromUtf8("tabAll"));
        horizontalLayout_4 = new QHBoxLayout(tabAll);
        horizontalLayout_4->setSpacing(0);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalLayout_4->setContentsMargins(0, 0, 0, 0);
        treeViewAll = new QTreeView(tabAll);
        treeViewAll->setObjectName(QString::fromUtf8("treeViewAll"));
        treeViewAll->setFrameShape(QFrame::NoFrame);
        treeViewAll->setFrameShadow(QFrame::Plain);
        treeViewAll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        treeViewAll->setEditTriggers(QAbstractItemView::NoEditTriggers);
        treeViewAll->setHeaderHidden(true);

        horizontalLayout_4->addWidget(treeViewAll);

        tabWidget->addTab(tabAll, QString());
        tabRecentlyClosed = new QWidget();
        tabRecentlyClosed->setObjectName(QString::fromUtf8("tabRecentlyClosed"));
        horizontalLayout_5 = new QHBoxLayout(tabRecentlyClosed);
        horizontalLayout_5->setSpacing(0);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        horizontalLayout_5->setContentsMargins(9, 9, 9, 9);
        listViewRecently = new QListView(tabRecentlyClosed);
        listViewRecently->setObjectName(QString::fromUtf8("listViewRecently"));
        listViewRecently->setFrameShape(QFrame::NoFrame);
        listViewRecently->setFrameShadow(QFrame::Plain);

        horizontalLayout_5->addWidget(listViewRecently);

        tabWidget->addTab(tabRecentlyClosed, QString());

        verticalLayout->addWidget(tabWidget);

        stackedWidget->addWidget(pageGeneral);

        verticalLayout_3->addWidget(stackedWidget);


        retranslateUi(HistoryWidget);

        stackedWidget->setCurrentIndex(1);
        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(HistoryWidget);
    } // setupUi

    void retranslateUi(QWidget *HistoryWidget)
    {
        HistoryWidget->setWindowTitle(QApplication::translate("HistoryWidget", "Form", nullptr));
        buttonSearchBack->setText(QString());
        label->setText(QApplication::translate("HistoryWidget", "Histories", nullptr));
        buttonSearch->setText(QString());
        buttonMore->setText(QString());
        buttonPinOrClose->setText(QString());
        tabWidget->setTabText(tabWidget->indexOf(tabAll), QApplication::translate("HistoryWidget", "All", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabRecentlyClosed), QApplication::translate("HistoryWidget", "Recently Closed", nullptr));
    } // retranslateUi

};

namespace Ui {
    class HistoryWidget: public Ui_HistoryWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HISTORYWIDGET_H
