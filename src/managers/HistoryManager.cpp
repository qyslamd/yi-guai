#include "HistoryManager.h"

#include <QtDebug>
#include <QDir>
#include <QFileInfo>
#include <QUrl>
#include <QTimer>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QMutexLocker>
#include <QStandardItemModel>

#include "utils/util_qt.h"
#include "FaviconManager.h"

// 就只想在这个cpp文件中使用，又不想把这个函数作为类的成员函数，
// static 的全局函数就是一个好办法
static void addToModel(const History &data)
{
    bool find = false;
    for (int i = 0; i< HistoryMgr::gHistoryModel->rowCount(); i++)
    {
        if(data.url == HistoryMgr::gHistoryModel->item(i)->data(HistoryMgr::Url).toString()){
            auto count = HistoryMgr::gHistoryModel->item(i)->data(HistoryMgr::Count).toInt();
            HistoryMgr::gHistoryModel->item(i)->setData(data.lastVisitedTime, HistoryMgr::LastTime);
            HistoryMgr::gHistoryModel->item(i)->setData(count + 1, HistoryMgr::Count);
            find = true;
            break;
        }
    }
    if(!find){
        auto item = new QStandardItem(data.title);
        item->setData(data.lastVisitedTime, HistoryMgr::LastTime);
        item->setData(data.url, HistoryMgr::Url);
        item->setData(data.title, HistoryMgr::Title);
        item->setData(1, HistoryMgr::Count);
        HistoryMgr::gHistoryModel->insertRow(0, item);
    }
}

HistoryMgr * HistoryMgr::gInst = nullptr;
QMutex HistoryMgr::gMutex;
QStandardItemModel *HistoryMgr::gHistoryModel = nullptr;
QStack<History> HistoryMgr::RecentlyHistory;

HistoryMgr::HistoryMgr(QObject *parent)
    : QObject(parent)
{
    if(!gHistoryModel){
        gHistoryModel = new QStandardItemModel(this);
    }

    worker_ = new HistoryWorker;
    worker_->moveToThread(&worker_thread_);

//    connect(&worker_thread_, &QThread::finished, this, [](){qInfo()<<"HistoryWorker thread finished!";});
    connect(this, &HistoryMgr::load, worker_, &HistoryWorker::loadFromFile);
    connect(this, &HistoryMgr::save, worker_, &HistoryWorker::saveToFile);
    connect(worker_, &HistoryWorker::loadFinished, this, &HistoryMgr::onWorkerLoadFinished);
    connect(worker_, &HistoryWorker::saveFinished, this, &HistoryMgr::onWorkerSaveFinished);

    QTimer::singleShot(0, this, &HistoryMgr::doLoadWork);
}

HistoryMgr *HistoryMgr::Instance()
{
    if(gInst == nullptr){
        QMutexLocker locker(&gMutex);
        if(gInst == nullptr){
            gInst = new HistoryMgr;
        }
    }
    static HistoryMgr::Gc gc;
    return gInst;
}

HistoryMgr::~HistoryMgr()
{
    qInfo()<<__FUNCTION__;
    worker_thread_.quit();
    worker_thread_.wait();

    delete worker_;
}

void HistoryMgr::addHistoryRecord(const History &data)
{
    if(!loaded_){
        pending_list_.append(data);
        return;
    }
    addToModel(data);
    doSaveWork();
}

void HistoryMgr::doLoadWork()
{
    worker_thread_.start();
    emit load();
}

void HistoryMgr::doSaveWork()
{
    worker_thread_.start();
    emit save();
}

void HistoryMgr::onWorkerLoadFinished()
{
    loaded_ = true;
    if(pending_list_.isEmpty()){
        worker_thread_.quit();
        worker_thread_.wait();
    }else{
        for(auto item : pending_list_){
            addToModel(item);
        }
        doSaveWork();
    }

    emit historyChanged();
}

void HistoryMgr::onWorkerSaveFinished()
{
    worker_thread_.quit();
    worker_thread_.wait();

//    qInfo()<<__FUNCTION__;
}

HistoryWorker::HistoryWorker()
{
    auto loc = UtilQt::appDataPath();
    record_file_path_ = QDir(loc).filePath("hisotiry");
}

HistoryWorker::~HistoryWorker()
{
    qInfo()<<__FUNCTION__;
}

void HistoryWorker::loadFromFile()
{
    qRegisterMetaType<QVector<int>>();
    qInfo()<<"\033[34m[Thread]"<<__FUNCTION__<<QThread::currentThreadId()<<"\033[0m";

    QElapsedTimer timer;
    timer.start();

    auto data = UtilQt::readFileUtf8(record_file_path_);

    QJsonArray jsonArray;
    QJsonParseError jsonError;
    QJsonDocument document = QJsonDocument::fromJson(data, &jsonError);
    if (!document.isNull() && (jsonError.error == QJsonParseError::NoError))
    {
        if (document.isArray())
        {
            jsonArray = document.array();
        }
    }

    HistoryMgr::gHistoryModel->clear();
    auto it = jsonArray.begin();
    auto end = jsonArray.end();
    for(; it != end; it++)
    {
        auto value = *it;
        if(value.isObject()){
            auto obj = value.toObject();
            auto title = obj.value("title").toString();
            QStandardItem *item = new QStandardItem(FaviconMgr::systemFileIcon, title);
            HistoryMgr::gHistoryModel->appendRow(item);
            item->setData(obj.value("lastVisitedTime").toString(), HistoryMgr::LastTime);
            item->setData(obj.value("url").toString(), HistoryMgr::Url);
            item->setData(title, HistoryMgr::Title);
            item->setData(obj.value("count").toString(), HistoryMgr::Count);
        }
    }
    qInfo()<<"\033[32m[Execute Time]"<<__FUNCTION__<<":" << timer.elapsed() << "ms"<<"\033[0m";
    emit loadFinished();
}

void HistoryWorker::saveToFile()
{
//    qInfo()<<__FUNCTION__;
    QJsonArray jsonArray;
    for( int i = 0; i < HistoryMgr::gHistoryModel->rowCount(); i++)
    {
        auto item = HistoryMgr::gHistoryModel->item(i);
        QJsonObject jsonObj;
        jsonObj.insert("lastVisitedTime",item->data(HistoryMgr::LastTime).toString());
        jsonObj.insert("url", item->data(HistoryMgr::Url).toString());
        jsonObj.insert("title",item->data(HistoryMgr::Title).toString());
        jsonObj.insert("count",item->data(HistoryMgr::Count).toString());
        jsonArray.append(jsonObj);
    }

    QJsonDocument document(jsonArray);
    QByteArray byteArray = document.toJson(QJsonDocument::Compact);
    QString strJson(byteArray);

    UtilQt::writeStringToFile(record_file_path_, strJson);

    emit saveFinished();
}
