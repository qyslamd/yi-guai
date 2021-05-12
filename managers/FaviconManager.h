#ifndef FAVICONMANAGER_H
#define FAVICONMANAGER_H

#include <QObject>
#include <QtDebug>

class FaviconMgr : public QObject
{
    Q_OBJECT
    explicit FaviconMgr(QObject *parent = nullptr);
    FaviconMgr(const FaviconMgr& other);
    FaviconMgr& operator=(const FaviconMgr & other);
public:
    static FaviconMgr& Instance();
    ~FaviconMgr();

public:
    void addIconRecord(const QString &urlIndex, const QString &filePath);
    void addIconsMap(const QMap<QString, QString> &icons);
    QString iconFilePath(const QString &urlIndex);
signals:
    void iconChanged(const QString &url);
private:
    QString record_file_path_;  // 记录文件本机路径
    QMap<QString, QString> icons_cache_;    // 缓存

    void loadIcons();
    void saveToFile();
};

#endif // FAVICONMANAGER_H
