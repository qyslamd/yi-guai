#ifndef FAVICONMANAGER_H
#define FAVICONMANAGER_H

#include <QObject>
#include <QtDebug>

class FaviconManager : public QObject
{
    Q_OBJECT

    explicit FaviconManager(QObject *parent = nullptr);
    FaviconManager(const FaviconManager& other);
    FaviconManager& operator=(const FaviconManager & other);
public:
    static FaviconManager& Instance(){
        static FaviconManager inst;
        return inst;
    }
    ~FaviconManager() {
        qInfo()<<__FUNCTION__;
    }

public:
    // 添加一个favicon记录
    void addIconRecord(const QString &urlIndex, const QString &filePath);
    // 添加一个Map表，在导入 netscape-bookmark-file时，会有批量添加的需求
    void addIconsMap(const QMap<QString, QString> &icons);
    // 根据 url 获取 favicon 文件的本机存储路径
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
