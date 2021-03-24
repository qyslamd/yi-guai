#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "cef_qwidget.h"
#include "cef_client_handler.h"

#include <QtDebug>
#include <QUrl>
#include <QCloseEvent>
#include <QTimerEvent>
#include <QTimer>
#include <QThread>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initUi();
    initSignalSlot();

    addOneBrowserPage("https://www.baidu.com/");

//    timer_id_close_ = startTimer(500);
}

MainWindow::~MainWindow()
{
    delete ui;
}

int MainWindow::addOneBrowserPage(const QString &url, bool switchTo)
{
    CefQWidget *page = new CefQWidget(url, this);
    auto index = ui->tabWidget->addTab(page, url);
    initPage(page);

    if(switchTo){
        ui->tabWidget->setCurrentIndex(index);
    }
    return index;
}

void MainWindow::timerEvent(QTimerEvent *event)
{
    if(event->timerId() == timer_id_close_){

    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    return;
    qInfo()<<__FUNCTION__;

    if(allow_close_){
        event->accept();
        return;
    }else{
        event->ignore();
    }

}

void MainWindow::initUi()
{
    ui->tabWidget->setTabsClosable(true);
}

void MainWindow::initSignalSlot()
{
    connect(ui->btnAddPage, &QPushButton::clicked, [this]
    {
        addOneBrowserPage("https://cn.bing.com/", true);
    });

    connect(ui->tabWidget, &QTabWidget::tabCloseRequested,
            this, &MainWindow::onTabPageCloseRequested);
}

void MainWindow::initPage(CefQWidget *page)
{
    connect(page, &CefQWidget::browserAddressChange, [this](const QString &address)
    {
        ui->lineEdit->setText(QUrl(address).toDisplayString());
    });

    connect(page, &CefQWidget::browserTitleChange, [this, page](const QString &title)
    {
        auto index = ui->tabWidget->indexOf(page);
        ui->tabWidget->setTabText(index, title);

    });
    connect(page, &CefQWidget::browserNewForgroundPage, [this](CefQWidget *window)
    {
        ui->tabWidget->addTab(window, "");
        initPage(window);
    });

}

void MainWindow::onTabPageCloseRequested(int index)
{
    auto widget = ui->tabWidget->widget(index);
    ui->tabWidget->removeTab(index);
    widget->close(); //隐藏
}
