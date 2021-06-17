#ifndef FAVICONMANAGER_H
#define FAVICONMANAGER_H

#include <QObject>
#include <QMap>

class FaviconMgr : public QObject
{
    Q_OBJECT
    explicit FaviconMgr(QObject *parent = nullptr);
    FaviconMgr(const FaviconMgr& other);
    FaviconMgr& operator=(const FaviconMgr & other);
public:
    static FaviconMgr& Instance();
    ~FaviconMgr();

    static QIcon systemFileIcon;
    static QIcon systemDirIcon;

public:
    void addIconRecord(const QString &urlIndex, const QString &filePath);
    void addIconsMap(const QMap<QString, QString> &icons);
    QIcon getFavicon(const QString &urlIndex);
signals:
    void iconUpdated(const QString &urlDomain);
private:
    QString record_file_path_;
    QMap<QString, QString> icons_path_cache_;

    QMap<QString, QIcon> icons_;

    void loadIcons();
    void saveToFile();
};

#endif // FAVICONMANAGER_H
