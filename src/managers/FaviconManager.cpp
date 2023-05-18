#include "FaviconManager.h"

#include <QtDebug>
#include <QDir>
#include <QFileInfo>
#include <QUrl>
#include <QIcon>
#include <QJsonObject>
#include <QJsonDocument>
#include <QApplication>
#include <QStyle>
#include <QJsonParseError>

#include "utils/util_qt.h"

QIcon FaviconMgr::systemFileIcon;
QIcon FaviconMgr::systemDirIcon;

FaviconMgr::FaviconMgr(QObject *parent)
    : QObject(parent)
{
    systemDirIcon = qApp->style()->standardIcon(QStyle::SP_DirIcon);
    systemFileIcon = qApp->style()->standardIcon(QStyle::SP_FileIcon);

//    auto loc = UtilQt::appDataPath();
//    record_file_path_ = QDir(loc).filePath("favicon");
//    loadIcons();
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

    icons_path_cache_.insert(qUrl.host(), filePath);
    emit iconUpdated(qUrl.host());

    saveToFile();
}

void FaviconMgr::addIconsMap(const QMap<QString, QString> &)
{
    qInfo()<<__FUNCTION__<<"TODO:";
}

QIcon FaviconMgr::getFavicon(const QString &urlIndex)
{
    if(urlIndex.isEmpty())
        return systemFileIcon;
    auto host = QUrl(urlIndex).host();

    if(icons_.contains(host)){
        return icons_[host];
    }

    if(host.isEmpty())
    {
        host = urlIndex;
    }
    if( icons_path_cache_.contains(host)){
        icons_.insert(host, QIcon(icons_path_cache_.value(host)));
        return icons_[host];
    }

    return systemFileIcon;
}

void FaviconMgr::loadIcons()
{
    icons_path_cache_.clear();

    auto data = UtilQt::readFileUtf8(record_file_path_);

    QJsonObject rootJsonObj;
    QJsonParseError jsonError;
    QJsonDocument document = QJsonDocument::fromJson(data, &jsonError);
    if (!document.isNull() && (jsonError.error == QJsonParseError::NoError))
    {
        if (document.isObject())
        {
            rootJsonObj = document.object();
        }
    }

    QJsonObject::Iterator it;
    for( it=rootJsonObj.begin(); it!=rootJsonObj.end(); it++)
    {
        QString key = it.key().toUtf8();
        if( !key.isEmpty() )
        {
            icons_path_cache_.insert(it.key().toUtf8(),it.value().toString());
        }
    }
}

void FaviconMgr::saveToFile()
{
    QJsonObject jsonObject;

    auto citer = icons_path_cache_.constBegin();
    auto cend = icons_path_cache_.constEnd();
    for( ; citer != cend; citer++)
    {
        jsonObject.insert(citer.key(), citer.value());
    }

    QJsonDocument document(jsonObject);
    QByteArray byteArray = document.toJson(QJsonDocument::Compact);
    QString strJson(byteArray);

    UtilQt::writeStringToFile(record_file_path_, strJson);
}
