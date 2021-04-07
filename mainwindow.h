#ifndef MAINWINDOW_H
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
    void topLevelWindowStateChanged(Qt::WindowStates state, const QVariant &data);
protected:
    void closeEvent(QCloseEvent *evnet) override;
    void changeEvent(QEvent *event) override;
private slots:
    void onTabPageCloseRequested(int index);
private:
    Ui::MainWindow *ui;
    // Identifies whether the entire window needs to be closed
    bool closing_ = false;

    void initUi();
    void initSignalSlot();
    void initPage(CefQWidget *page);

    CefQWidget *GetActivePage();
};
#endif // MAINWINDOW_H
