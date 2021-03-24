﻿#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>

#include "cef_client_handler.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class CefQWidget;
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    int addOneBrowserPage(const QString &url, bool switchTo = false);
signals:
    void windowWannaClose();
protected:
    void timerEvent(QTimerEvent *event) override;
    void closeEvent(QCloseEvent *evnet) override;
private:
    Ui::MainWindow *ui;

    void initUi();
    void initSignalSlot();
    void initPage(CefQWidget *page);

    bool allow_close_ = false;
    int timer_id_close_;
private slots:
    void onTabPageCloseRequested(int index);
};
#endif // MAINWINDOW_H
