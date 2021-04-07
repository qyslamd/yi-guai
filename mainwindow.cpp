#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "cef_qwidget.h"
#include "cef_client_handler.h"
#include "util_qt.h"

#include <QtDebug>
#include <QUrl>
#include <QCloseEvent>
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

void MainWindow::closeEvent(QCloseEvent *event)
{
    closing_ = true;
    if(ui->tabWidget->count() > 0){
        // 一个一个的关闭，等待关闭完
        onTabPageCloseRequested(0);
        event->ignore();
        return;
    }else{
        // 真正的关闭
        event->accept();
    }
}

void MainWindow::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::WindowStateChange)
    {
        emit topLevelWindowStateChanged(windowState(), ui->tabWidget->currentWidget()->size());
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
    connect(ui->lineEdit, &QLineEdit::returnPressed, [this]()
    {
        auto url = UtilQt::check_url(ui->lineEdit->text());
        auto page = GetActivePage();
        if(page){
            page->Navigate(url);
        }

    });
}

void MainWindow::initPage(CefQWidget *page)
{
    connect(page, &CefQWidget::browserClosing, [this](CefQWidget *page)
    {
        auto index = ui->tabWidget->indexOf(page);
        ui->tabWidget->removeTab(index);
        // 必须要删除才能真正的释放cef的browser
        page->deleteLater();
        // 关掉一个以后，紧接着判断是不是用户在关闭整个窗口
        if(closing_){
            close();
        }
    });

    // 顶层窗口的窗口状态改变应该通知到page去，
    connect(this, &MainWindow::topLevelWindowStateChanged, page, &CefQWidget::onTopLevelWindowStateChanged);

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
        auto index = ui->tabWidget->addTab(window, "");
        ui->tabWidget->setCurrentIndex(index);
        initPage(window);
    });

}

CefQWidget *MainWindow::GetActivePage()
{
    auto widget = ui->tabWidget->currentWidget();
    if(widget){
        return qobject_cast<CefQWidget *>(widget);
    }
    return nullptr;
}

void MainWindow::onTabPageCloseRequested(int index)
{
    auto page = ui->tabWidget->widget(index);
    if(page){
        page->close();
    }
}
