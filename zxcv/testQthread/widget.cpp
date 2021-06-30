#include "widget.h"
#include "ui_widget.h"
#include <QtDebug>
#include <QTimer>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    worker_ = new Worker;
    worker_->moveToThread(&worker_thread_);

    connect(this, &Widget::doWork, worker_, &Worker::doWork);
    connect(worker_, &Worker::progressChanged, this, &Widget::onWokerProgress);
    connect(worker_, &Worker::finished, this, &Widget::onWorkerFinished);
    connect(worker_, &Worker::errorOccurs, this, &Widget::onWorkerError);
    worker_thread_.start();

    connect(ui->pushButton, &QPushButton::clicked, this, &Widget::doWork);
}

Widget::~Widget()
{
    worker_thread_.quit();
    worker_thread_.wait();

    delete worker_;
    delete ui;
}

void Widget::closeEvent(QCloseEvent *event)
{
    worker_thread_.quit();
    worker_thread_.wait();
}

void Widget::onWokerProgress(int value)
{
    ui->progressBar->setValue(value);
}

void Widget::onWorkerFinished()
{
    QTimer::singleShot(2000, this, &Widget::close);
}

void Widget::onWorkerError()
{

}


Worker::Worker(QObject *parent)
    : QObject(parent)
{

}

Worker::~Worker()
{
    qInfo()<<__FUNCTION__;
}

void Worker::doWork()
{
    for(int i = 0; i < 101; i++){
        i++;
        emit progressChanged(i);
        QThread::msleep(500);
    }

    emit finished();
}
