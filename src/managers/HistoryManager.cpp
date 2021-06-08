#include "HistoryManager.h"

#include <QtDebug>
#include <QDir>
#include <QFileInfo>
#include <QUrl>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonParseError>

#include "utils/util_qt.h"

HistoryMgr::HistoryMgr(QObject *parent)
    : QObject(parent)
{
    auto loc = UtilQt::appDataPath();
    record_file_path_ = QDir(loc).filePath("hisotiry");
    loadHistories();
}

HistoryMgr &HistoryMgr::Instance()
{
    static HistoryMgr inst;
    return inst;
}

HistoryMgr::~HistoryMgr() {
    qInfo()<<__FUNCTION__;
}

void HistoryMgr::addHistoryRecord(const History &data)
{
    if(data.url.isEmpty()){
        return;
    }

    histories_cache_.insert(0, data);
    saveToFile();
}

QList<History> HistoryMgr::allHistories()
{
    // TODO:这将是一个巨大拷贝量
    return histories_cache_;
}

void HistoryMgr::loadHistories()
{
    histories_cache_.clear();

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

    auto it = jsonArray.begin();
    auto end = jsonArray.end();
    for(; it != end; it++)
    {
        auto value = *it;
        if(value.isObject()){
            auto obj = value.toObject();
            History data
            {
                (long)obj.value("time").toDouble(),
                        obj.value("url").toString(),
                        obj.value("title").toString()
            };
            histories_cache_.append(data);
        }
    }
}

void HistoryMgr::saveToFile()
{
    QJsonArray jsonArray;

    auto citer = histories_cache_.constBegin();
    auto cend = histories_cache_.constEnd();
    for( ; citer != cend; citer++)
    {
        QJsonObject jsonObj;
        jsonObj.insert("datetime",QString::number(citer->time));
        jsonObj.insert("url", citer->url);
        jsonObj.insert("title",citer->title);
        jsonArray.append(jsonObj);
    }

    QJsonDocument document(jsonArray);
    QByteArray byteArray = document.toJson(QJsonDocument::Compact);
    QString strJson(byteArray);

    UtilQt::writeStringToFile(record_file_path_, strJson);
}
