#include "FaviconManager.h"

#include <QDir>
#include <QFileInfo>
#include <QUrl>
#include <QStandardPaths>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonParseError>

#include "utils/util_qt.h"

FaviconMgr::FaviconMgr(QObject *parent)
    : QObject(parent)
{
    auto loc = UtilQt::appDataPath();
    record_file_path_ = QDir(loc).filePath("favicon");
    loadIcons();
}

FaviconMgr::~FaviconMgr() {
    qInfo()<<__FUNCTION__;
}

FaviconMgr& FaviconMgr::Instance(){
    static FaviconMgr inst;
    return inst;
}

void FaviconMgr::addIconRecord(const QString &urlIndex,
                                   const QString &filePath)
{
    if( urlIndex.isEmpty()||filePath.isEmpty())
        return;
    QUrl qUrl(urlIndex);
    if(qUrl.host().isEmpty())
        return;

    icons_cache_.insert(qUrl.host(), filePath);

    saveToFile();
}

void FaviconMgr::addIconsMap(const QMap<QString, QString> &)
{
    qInfo()<<__FUNCTION__<<"TODO:";
}

QString FaviconMgr::iconFilePath(const QString &urlIndex)
{
    if(urlIndex.isEmpty())
        return "";
    QUrl qUrl(urlIndex);
    if(qUrl.host().isEmpty())
        return "";
    if( icons_cache_.contains(qUrl.host()))
        return icons_cache_.value(qUrl.host());
    return "";
}

void FaviconMgr::loadIcons()
{
    icons_cache_.clear();

    auto data = UtilQt::readFileUtf8(record_file_path_);

    QJsonObject rootJsonObj;
    QJsonParseError jsonError;
    QJsonDocument doucment = QJsonDocument::fromJson(data, &jsonError);
    if (!doucment.isNull() && (jsonError.error == QJsonParseError::NoError))
    {
        if (doucment.isObject())
        {
            rootJsonObj = doucment.object();
        }
    }

    QJsonObject::Iterator it;
    for( it=rootJsonObj.begin(); it!=rootJsonObj.end(); it++)
    {
        QString key = it.key().toUtf8();
        if( !key.isEmpty() )
        {
            icons_cache_.insert(it.key().toUtf8(),it.value().toString());
        }
    }
}

void FaviconMgr::saveToFile()
{
    QJsonObject jsonObject;

    auto citer = icons_cache_.constBegin();
    auto cend = icons_cache_.constEnd();
    for( ; citer != cend; citer++)
    {
        jsonObject.insert(citer.key(), citer.value());
    }

    QJsonDocument document(jsonObject);
    QByteArray byteArray = document.toJson(QJsonDocument::Compact);
    QString strJson(byteArray);

    UtilQt::writeStringToFile(record_file_path_, strJson);
}
