#include "AddrInputManager.h"

#include <QtDebug>
#include <QDir>
#include <QFileInfo>
#include <QUrl>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonParseError>

#include "utils/util_qt.h"

AddrInputMgr::AddrInputMgr(QObject *parent)
    : QObject(parent)
{
    auto loc = UtilQt::appDataPath();
    file_path_ = QDir(loc).filePath("addr-input");
    loadToCache();
}

AddrInputMgr::~AddrInputMgr() {
    qInfo()<<__FUNCTION__;
}

QStringList AddrInputMgr::inputList() const
{
    return cache_;
}

void AddrInputMgr::addRecord(const QString &data)
{
    if(!data.isEmpty())
    {
        cache_.append(data);
    }
    saveToFile();
}

void AddrInputMgr::addRecords(const QStringList &data)
{
    if(!data.isEmpty())
    {
        cache_.append(data);
    }
    saveToFile();
}

AddrInputMgr& AddrInputMgr::Instance(){
    static AddrInputMgr inst;
    return inst;
}

void AddrInputMgr::loadToCache()
{
    cache_.clear();
    auto data = UtilQt::readFileUtf8(file_path_);
    QJsonArray rootArray;
    QJsonParseError jsonError;
    QJsonDocument document = QJsonDocument::fromJson(data, &jsonError);
    if (!document.isNull() && (jsonError.error == QJsonParseError::NoError))
    {
        if(!document.isArray()){
            return;
        }
        rootArray = document.array();
    }

    auto cit  = rootArray.cbegin();
    for(; cit != rootArray.cend(); cit++)
    {
        auto value = *cit;
        if(value.isString()){
            cache_.append(value.toString());
        }
    }
}

void AddrInputMgr::saveToFile()
{
    QJsonArray jArray;

    auto cit = cache_.constBegin();
    auto cend = cache_.constEnd();
    for(; cit != cend; cit++){
        jArray.append(QJsonValue(*cit));
    }

    QJsonDocument jDoc(jArray);

    auto data = jDoc.toJson(QJsonDocument::Compact);
    UtilQt::writeDataToFile(file_path_, data);
}
