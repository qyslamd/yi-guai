#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QThread>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Worker;
class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

signals:
    void doWork();

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    Ui::Widget *ui;

    QThread worker_thread_;
    Worker *worker_ = nullptr;

private slots:
    void onWokerProgress(int value);
    void onWorkerFinished();
    void onWorkerError();
};


class Worker : public QObject
{
    Q_OBJECT
public:
    explicit Worker(QObject *parent = nullptr);
    ~Worker();
signals:
    void progressChanged(int value);
    void finished();
    void errorOccurs();
public slots:
    void doWork();
};

#endif // WIDGET_H
