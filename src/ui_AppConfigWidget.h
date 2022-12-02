/********************************************************************************
** Form generated from reading UI file 'AppConfigWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.12.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_APPCONFIGWIDGET_H
#define UI_APPCONFIGWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AppCfgWidget
{
public:
    QHBoxLayout *horizontalLayout;
    QSplitter *splitter;
    QScrollArea *scrollAreaNavi;
    QWidget *scrollAreaWidgetNavi;
    QVBoxLayout *verticalLayout_2;
    QFrame *frameNaviBtns;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QSpacerItem *verticalSpacer_3;
    QPushButton *btnAppearence;
    QPushButton *btnSearch;
    QPushButton *btnStartup;
    QPushButton *btnTabPage;
    QPushButton *btnDownload;
    QSpacerItem *verticalSpacer;
    QStackedWidget *stackedWidget;
    QWidget *pageAppearance;
    QVBoxLayout *verticalLayout_3;
    QScrollArea *scrollAreaPageApprearence;
    QWidget *scrollAreaWidgetContents;
    QVBoxLayout *verticalLayout_4;
    QLabel *label_2;
    QFrame *frameAppea_Browser;
    QGridLayout *gridLayout_2;
    QLabel *label_3;
    QSpacerItem *horizontalSpacer;
    QComboBox *comboBrowserTheme;
    QLabel *label_4;
    QSpacerItem *horizontalSpacer_2;
    QComboBox *comboZoomLevel;
    QLabel *label_5;
    QFrame *frame_2;
    QGridLayout *gridLayout_3;
    QLabel *label_8;
    QLabel *label_12;
    QToolButton *toolButtonShowHistoryBtn;
    QToolButton *toolButtonVerticalTab;
    QLineEdit *lineEdit_2;
    QSpacerItem *horizontalSpacer_13;
    QSpacerItem *horizontalSpacer_5;
    QToolButton *toolButtonShowDownloadBtn;
    QSpacerItem *horizontalSpacer_11;
    QLabel *label_9;
    QLabel *label_13;
    QSpacerItem *horizontalSpacer_9;
    QLabel *label_14;
    QSpacerItem *horizontalSpacer_10;
    QSpacerItem *horizontalSpacer_7;
    QComboBox *comboBoxShowBkmkBar;
    QLabel *label_6;
    QSpacerItem *horizontalSpacer_8;
    QSpacerItem *horizontalSpacer_6;
    QSpacerItem *horizontalSpacer_4;
    QLabel *label_10;
    QSpacerItem *horizontalSpacer_12;
    QLabel *label_7;
    QPushButton *btnSaveHomePage;
    QToolButton *toolButtonShowCaptureBtn;
    QToolButton *toolButtonShowFeedbackBtn;
    QLabel *label_11;
    QToolButton *toolButtonHomePageBtn;
    QToolButton *toolButtonShowBkmkBtn;
    QSpacerItem *verticalSpacer_2;
    QWidget *pageStartup;
    QWidget *pageNewTabPage;
    QWidget *pageSearch;
    QWidget *pageDownload;

    void setupUi(QWidget *AppCfgWidget)
    {
        if (AppCfgWidget->objectName().isEmpty())
            AppCfgWidget->setObjectName(QString::fromUtf8("AppCfgWidget"));
        AppCfgWidget->resize(838, 536);
        AppCfgWidget->setStyleSheet(QString::fromUtf8("QSplitter::handle:horizontal {\n"
"width: 1px;\n"
"background:#DCDCDC;\n"
"}\n"
"\n"
"QSplitter::handle:vertical {\n"
"width: 1px;\n"
"background:#DCDCDC;\n"
"}"));
        horizontalLayout = new QHBoxLayout(AppCfgWidget);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        splitter = new QSplitter(AppCfgWidget);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        splitter->setOrientation(Qt::Horizontal);
        splitter->setOpaqueResize(false);
        splitter->setChildrenCollapsible(false);
        scrollAreaNavi = new QScrollArea(splitter);
        scrollAreaNavi->setObjectName(QString::fromUtf8("scrollAreaNavi"));
        scrollAreaNavi->setMinimumSize(QSize(250, 0));
        scrollAreaNavi->setMaximumSize(QSize(250, 16777215));
        scrollAreaNavi->setFrameShape(QFrame::NoFrame);
        scrollAreaNavi->setFrameShadow(QFrame::Plain);
        scrollAreaNavi->setWidgetResizable(true);
        scrollAreaWidgetNavi = new QWidget();
        scrollAreaWidgetNavi->setObjectName(QString::fromUtf8("scrollAreaWidgetNavi"));
        scrollAreaWidgetNavi->setGeometry(QRect(0, 0, 250, 536));
        verticalLayout_2 = new QVBoxLayout(scrollAreaWidgetNavi);
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        frameNaviBtns = new QFrame(scrollAreaWidgetNavi);
        frameNaviBtns->setObjectName(QString::fromUtf8("frameNaviBtns"));
        frameNaviBtns->setStyleSheet(QString::fromUtf8("QFrame#frameNaviBtns{\n"
"border:none;\n"
"border-right:1px solid #cdcdcd;\n"
"border-top:1px solid #cdcdcd;\n"
"background-color:#F7F7F7;\n"
"}\n"
"\n"
"QPushButton{\n"
"border:none;\n"
"/*border-radius:12px;*/\n"
"text-align:left;\n"
"padding:8px;\n"
"background:transparent;\n"
"}\n"
"QPushButton:hover{\n"
"background-color:#EAEAEA;\n"
"}\n"
"\n"
"QPushButton:checked{\n"
"background-color:#E5E5E5;\n"
"border-left:3px solid #0072C9;\n"
"}"));
        frameNaviBtns->setFrameShape(QFrame::StyledPanel);
        frameNaviBtns->setFrameShadow(QFrame::Raised);
        verticalLayout = new QVBoxLayout(frameNaviBtns);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(12, -1, -1, -1);
        label = new QLabel(frameNaviBtns);
        label->setObjectName(QString::fromUtf8("label"));
        label->setStyleSheet(QString::fromUtf8("font-weight:bold;\n"
"font-size:14pt;\n"
""));

        verticalLayout->addWidget(label);

        verticalSpacer_3 = new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout->addItem(verticalSpacer_3);

        btnAppearence = new QPushButton(frameNaviBtns);
        btnAppearence->setObjectName(QString::fromUtf8("btnAppearence"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/resources/imgs/paint_palette_48px.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnAppearence->setIcon(icon);
        btnAppearence->setCheckable(true);

        verticalLayout->addWidget(btnAppearence);

        btnSearch = new QPushButton(frameNaviBtns);
        btnSearch->setObjectName(QString::fromUtf8("btnSearch"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/icons/resources/imgs/search2_64px.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnSearch->setIcon(icon1);
        btnSearch->setCheckable(true);

        verticalLayout->addWidget(btnSearch);

        btnStartup = new QPushButton(frameNaviBtns);
        btnStartup->setObjectName(QString::fromUtf8("btnStartup"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/icons/resources/imgs/shutdown_64px.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnStartup->setIcon(icon2);
        btnStartup->setCheckable(true);

        verticalLayout->addWidget(btnStartup);

        btnTabPage = new QPushButton(frameNaviBtns);
        btnTabPage->setObjectName(QString::fromUtf8("btnTabPage"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/icons/resources/imgs/tab_48px.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnTabPage->setIcon(icon3);
        btnTabPage->setCheckable(true);

        verticalLayout->addWidget(btnTabPage);

        btnDownload = new QPushButton(frameNaviBtns);
        btnDownload->setObjectName(QString::fromUtf8("btnDownload"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/icons/resources/imgs/download_64px.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnDownload->setIcon(icon4);
        btnDownload->setCheckable(true);

        verticalLayout->addWidget(btnDownload);

        verticalSpacer = new QSpacerItem(20, 385, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        verticalLayout_2->addWidget(frameNaviBtns);

        scrollAreaNavi->setWidget(scrollAreaWidgetNavi);
        splitter->addWidget(scrollAreaNavi);
        stackedWidget = new QStackedWidget(splitter);
        stackedWidget->setObjectName(QString::fromUtf8("stackedWidget"));
        pageAppearance = new QWidget();
        pageAppearance->setObjectName(QString::fromUtf8("pageAppearance"));
        pageAppearance->setStyleSheet(QString::fromUtf8(""));
        verticalLayout_3 = new QVBoxLayout(pageAppearance);
        verticalLayout_3->setSpacing(0);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        scrollAreaPageApprearence = new QScrollArea(pageAppearance);
        scrollAreaPageApprearence->setObjectName(QString::fromUtf8("scrollAreaPageApprearence"));
        scrollAreaPageApprearence->setFrameShape(QFrame::NoFrame);
        scrollAreaPageApprearence->setFrameShadow(QFrame::Plain);
        scrollAreaPageApprearence->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 588, 536));
        scrollAreaWidgetContents->setStyleSheet(QString::fromUtf8(""));
        verticalLayout_4 = new QVBoxLayout(scrollAreaWidgetContents);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        verticalLayout_4->setContentsMargins(-1, 0, -1, -1);
        label_2 = new QLabel(scrollAreaWidgetContents);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setStyleSheet(QString::fromUtf8("font-weight:bold;\n"
"font-size:13pt;\n"
""));

        verticalLayout_4->addWidget(label_2);

        frameAppea_Browser = new QFrame(scrollAreaWidgetContents);
        frameAppea_Browser->setObjectName(QString::fromUtf8("frameAppea_Browser"));
        frameAppea_Browser->setStyleSheet(QString::fromUtf8(".QFrame{\n"
"border:1px solid gray;\n"
"background-color:white;\n"
"border-radius:8px;\n"
"}"));
        frameAppea_Browser->setFrameShape(QFrame::StyledPanel);
        frameAppea_Browser->setFrameShadow(QFrame::Raised);
        gridLayout_2 = new QGridLayout(frameAppea_Browser);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        label_3 = new QLabel(frameAppea_Browser);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout_2->addWidget(label_3, 0, 0, 1, 1);

        horizontalSpacer = new QSpacerItem(328, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer, 0, 1, 1, 1);

        comboBrowserTheme = new QComboBox(frameAppea_Browser);
        comboBrowserTheme->setObjectName(QString::fromUtf8("comboBrowserTheme"));

        gridLayout_2->addWidget(comboBrowserTheme, 0, 2, 1, 1);

        label_4 = new QLabel(frameAppea_Browser);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout_2->addWidget(label_4, 1, 0, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(328, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_2, 1, 1, 1, 1);

        comboZoomLevel = new QComboBox(frameAppea_Browser);
        comboZoomLevel->setObjectName(QString::fromUtf8("comboZoomLevel"));

        gridLayout_2->addWidget(comboZoomLevel, 1, 2, 1, 1);


        verticalLayout_4->addWidget(frameAppea_Browser);

        label_5 = new QLabel(scrollAreaWidgetContents);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setStyleSheet(QString::fromUtf8("font-weight:bold;\n"
"font-size:13pt;\n"
""));

        verticalLayout_4->addWidget(label_5);

        frame_2 = new QFrame(scrollAreaWidgetContents);
        frame_2->setObjectName(QString::fromUtf8("frame_2"));
        frame_2->setLayoutDirection(Qt::LeftToRight);
        frame_2->setStyleSheet(QString::fromUtf8(".QFrame{\n"
"border:1px solid gray;\n"
"background-color:white;\n"
"border-radius:8px;\n"
"}\n"
"QToolButton{\n"
"border:none;\n"
"	border-image: url(:/icons/resources/imgs/switch_off_64px.png);\n"
"}\n"
"QToolButton:checked{\n"
"	border-image: url(:/icons/resources/imgs/switch_on_64px.png);\n"
"}"));
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        gridLayout_3 = new QGridLayout(frame_2);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        label_8 = new QLabel(frame_2);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setStyleSheet(QString::fromUtf8("font-weight:bold;\n"
"font-size:10pt;\n"
""));

        gridLayout_3->addWidget(label_8, 2, 0, 1, 1);

        label_12 = new QLabel(frame_2);
        label_12->setObjectName(QString::fromUtf8("label_12"));

        gridLayout_3->addWidget(label_12, 7, 0, 1, 3);

        toolButtonShowHistoryBtn = new QToolButton(frame_2);
        toolButtonShowHistoryBtn->setObjectName(QString::fromUtf8("toolButtonShowHistoryBtn"));
        toolButtonShowHistoryBtn->setMinimumSize(QSize(32, 32));
        toolButtonShowHistoryBtn->setMaximumSize(QSize(32, 32));
        toolButtonShowHistoryBtn->setCheckable(true);
        toolButtonShowHistoryBtn->setChecked(false);

        gridLayout_3->addWidget(toolButtonShowHistoryBtn, 7, 5, 1, 1);

        toolButtonVerticalTab = new QToolButton(frame_2);
        toolButtonVerticalTab->setObjectName(QString::fromUtf8("toolButtonVerticalTab"));
        toolButtonVerticalTab->setMinimumSize(QSize(32, 32));
        toolButtonVerticalTab->setMaximumSize(QSize(32, 32));
        toolButtonVerticalTab->setStyleSheet(QString::fromUtf8(""));
        toolButtonVerticalTab->setCheckable(true);
        toolButtonVerticalTab->setChecked(false);

        gridLayout_3->addWidget(toolButtonVerticalTab, 0, 5, 1, 1);

        lineEdit_2 = new QLineEdit(frame_2);
        lineEdit_2->setObjectName(QString::fromUtf8("lineEdit_2"));

        gridLayout_3->addWidget(lineEdit_2, 3, 4, 1, 1);

        horizontalSpacer_13 = new QSpacerItem(278, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer_13, 9, 4, 1, 1);

        horizontalSpacer_5 = new QSpacerItem(278, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer_5, 1, 4, 1, 1);

        toolButtonShowDownloadBtn = new QToolButton(frame_2);
        toolButtonShowDownloadBtn->setObjectName(QString::fromUtf8("toolButtonShowDownloadBtn"));
        toolButtonShowDownloadBtn->setMinimumSize(QSize(32, 32));
        toolButtonShowDownloadBtn->setMaximumSize(QSize(32, 32));
        toolButtonShowDownloadBtn->setCheckable(true);
        toolButtonShowDownloadBtn->setChecked(false);

        gridLayout_3->addWidget(toolButtonShowDownloadBtn, 6, 5, 1, 1);

        horizontalSpacer_11 = new QSpacerItem(278, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer_11, 7, 4, 1, 1);

        label_9 = new QLabel(frame_2);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        gridLayout_3->addWidget(label_9, 4, 0, 1, 2);

        label_13 = new QLabel(frame_2);
        label_13->setObjectName(QString::fromUtf8("label_13"));

        gridLayout_3->addWidget(label_13, 8, 0, 1, 3);

        horizontalSpacer_9 = new QSpacerItem(278, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer_9, 5, 4, 1, 1);

        label_14 = new QLabel(frame_2);
        label_14->setObjectName(QString::fromUtf8("label_14"));

        gridLayout_3->addWidget(label_14, 9, 0, 1, 4);

        horizontalSpacer_10 = new QSpacerItem(278, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer_10, 6, 4, 1, 1);

        horizontalSpacer_7 = new QSpacerItem(377, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer_7, 2, 1, 1, 5);

        comboBoxShowBkmkBar = new QComboBox(frame_2);
        comboBoxShowBkmkBar->setObjectName(QString::fromUtf8("comboBoxShowBkmkBar"));

        gridLayout_3->addWidget(comboBoxShowBkmkBar, 4, 5, 1, 1);

        label_6 = new QLabel(frame_2);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        gridLayout_3->addWidget(label_6, 0, 0, 1, 2);

        horizontalSpacer_8 = new QSpacerItem(290, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer_8, 4, 2, 1, 3);

        horizontalSpacer_6 = new QSpacerItem(107, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer_6, 3, 0, 1, 2);

        horizontalSpacer_4 = new QSpacerItem(284, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer_4, 0, 3, 1, 2);

        label_10 = new QLabel(frame_2);
        label_10->setObjectName(QString::fromUtf8("label_10"));

        gridLayout_3->addWidget(label_10, 5, 0, 1, 4);

        horizontalSpacer_12 = new QSpacerItem(278, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer_12, 8, 4, 1, 1);

        label_7 = new QLabel(frame_2);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        gridLayout_3->addWidget(label_7, 1, 0, 1, 4);

        btnSaveHomePage = new QPushButton(frame_2);
        btnSaveHomePage->setObjectName(QString::fromUtf8("btnSaveHomePage"));
        btnSaveHomePage->setMinimumSize(QSize(70, 28));

        gridLayout_3->addWidget(btnSaveHomePage, 3, 5, 1, 1);

        toolButtonShowCaptureBtn = new QToolButton(frame_2);
        toolButtonShowCaptureBtn->setObjectName(QString::fromUtf8("toolButtonShowCaptureBtn"));
        toolButtonShowCaptureBtn->setMinimumSize(QSize(32, 32));
        toolButtonShowCaptureBtn->setMaximumSize(QSize(32, 32));
        toolButtonShowCaptureBtn->setCheckable(true);
        toolButtonShowCaptureBtn->setChecked(false);

        gridLayout_3->addWidget(toolButtonShowCaptureBtn, 8, 5, 1, 1);

        toolButtonShowFeedbackBtn = new QToolButton(frame_2);
        toolButtonShowFeedbackBtn->setObjectName(QString::fromUtf8("toolButtonShowFeedbackBtn"));
        toolButtonShowFeedbackBtn->setMinimumSize(QSize(32, 32));
        toolButtonShowFeedbackBtn->setMaximumSize(QSize(32, 32));
        toolButtonShowFeedbackBtn->setCheckable(true);
        toolButtonShowFeedbackBtn->setChecked(false);

        gridLayout_3->addWidget(toolButtonShowFeedbackBtn, 9, 5, 1, 1);

        label_11 = new QLabel(frame_2);
        label_11->setObjectName(QString::fromUtf8("label_11"));

        gridLayout_3->addWidget(label_11, 6, 0, 1, 4);

        toolButtonHomePageBtn = new QToolButton(frame_2);
        toolButtonHomePageBtn->setObjectName(QString::fromUtf8("toolButtonHomePageBtn"));
        toolButtonHomePageBtn->setMinimumSize(QSize(32, 32));
        toolButtonHomePageBtn->setMaximumSize(QSize(32, 32));
        toolButtonHomePageBtn->setCheckable(true);
        toolButtonHomePageBtn->setChecked(false);

        gridLayout_3->addWidget(toolButtonHomePageBtn, 1, 5, 1, 1);

        toolButtonShowBkmkBtn = new QToolButton(frame_2);
        toolButtonShowBkmkBtn->setObjectName(QString::fromUtf8("toolButtonShowBkmkBtn"));
        toolButtonShowBkmkBtn->setMinimumSize(QSize(32, 32));
        toolButtonShowBkmkBtn->setMaximumSize(QSize(32, 32));
        toolButtonShowBkmkBtn->setCheckable(true);
        toolButtonShowBkmkBtn->setChecked(false);

        gridLayout_3->addWidget(toolButtonShowBkmkBtn, 5, 5, 1, 1);


        verticalLayout_4->addWidget(frame_2);

        verticalSpacer_2 = new QSpacerItem(20, 6, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_4->addItem(verticalSpacer_2);

        scrollAreaPageApprearence->setWidget(scrollAreaWidgetContents);

        verticalLayout_3->addWidget(scrollAreaPageApprearence);

        stackedWidget->addWidget(pageAppearance);
        pageStartup = new QWidget();
        pageStartup->setObjectName(QString::fromUtf8("pageStartup"));
        stackedWidget->addWidget(pageStartup);
        pageNewTabPage = new QWidget();
        pageNewTabPage->setObjectName(QString::fromUtf8("pageNewTabPage"));
        stackedWidget->addWidget(pageNewTabPage);
        pageSearch = new QWidget();
        pageSearch->setObjectName(QString::fromUtf8("pageSearch"));
        stackedWidget->addWidget(pageSearch);
        pageDownload = new QWidget();
        pageDownload->setObjectName(QString::fromUtf8("pageDownload"));
        stackedWidget->addWidget(pageDownload);
        splitter->addWidget(stackedWidget);

        horizontalLayout->addWidget(splitter);


        retranslateUi(AppCfgWidget);

        stackedWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(AppCfgWidget);
    } // setupUi

    void retranslateUi(QWidget *AppCfgWidget)
    {
        AppCfgWidget->setWindowTitle(QApplication::translate("AppCfgWidget", "Form", nullptr));
        label->setText(QApplication::translate("AppCfgWidget", "Settings", nullptr));
        btnAppearence->setText(QApplication::translate("AppCfgWidget", "Appearence", nullptr));
        btnSearch->setText(QApplication::translate("AppCfgWidget", "Search", nullptr));
        btnStartup->setText(QApplication::translate("AppCfgWidget", "Startup", nullptr));
        btnTabPage->setText(QApplication::translate("AppCfgWidget", "New tab page", nullptr));
        btnDownload->setText(QApplication::translate("AppCfgWidget", "Download", nullptr));
        label_2->setText(QApplication::translate("AppCfgWidget", "Customize browser", nullptr));
        label_3->setText(QApplication::translate("AppCfgWidget", "defualt theme", nullptr));
        label_4->setText(QApplication::translate("AppCfgWidget", "Zoom level", nullptr));
        label_5->setText(QApplication::translate("AppCfgWidget", "Customize tool bars", nullptr));
        label_8->setText(QApplication::translate("AppCfgWidget", "set home page", nullptr));
        label_12->setText(QApplication::translate("AppCfgWidget", "show history button", nullptr));
        toolButtonShowHistoryBtn->setText(QString());
        toolButtonVerticalTab->setText(QString());
        toolButtonShowDownloadBtn->setText(QString());
        label_9->setText(QApplication::translate("AppCfgWidget", "show bookmark bar", nullptr));
        label_13->setText(QApplication::translate("AppCfgWidget", "show capture button", nullptr));
        label_14->setText(QApplication::translate("AppCfgWidget", "show feedback button", nullptr));
        label_6->setText(QApplication::translate("AppCfgWidget", "show vertical tabs", nullptr));
        label_10->setText(QApplication::translate("AppCfgWidget", "show bookmark button", nullptr));
        label_7->setText(QApplication::translate("AppCfgWidget", "show homepage button", nullptr));
        btnSaveHomePage->setText(QApplication::translate("AppCfgWidget", "save", nullptr));
        toolButtonShowCaptureBtn->setText(QString());
        toolButtonShowFeedbackBtn->setText(QString());
        label_11->setText(QApplication::translate("AppCfgWidget", "show downlaod button", nullptr));
        toolButtonHomePageBtn->setText(QString());
        toolButtonShowBkmkBtn->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class AppCfgWidget: public Ui_AppCfgWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_APPCONFIGWIDGET_H
