#ifndef MAINWINDOWMGR_H
#define MAINWINDOWMGR_H

#include <QObject>
#include <QtDebug>
#include <QSet>

class MainWindow;
class MainWindowMgr : public QObject
{
    Q_OBJECT
    explicit MainWindowMgr(QObject *parent = nullptr);
    MainWindowMgr(const MainWindowMgr& other);
    MainWindowMgr& operator=(const MainWindowMgr & other);
public:
    static MainWindowMgr& instance(){
        static MainWindowMgr inst;
        return inst;
    }
    ~MainWindowMgr() {
        qInfo()<<__FUNCTION__;
    }
    void createWindow();
signals:
private:
    QSet<MainWindow *> windows_;

    void closeAllWindows();

};

#endif // MAINWINDOWMGR_H
