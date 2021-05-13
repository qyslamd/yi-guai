#include "util_qt.h"
#include <QUrl>
#include <QtDebug>
#include <QSettings>
#include <QColor>
#include <QScreen>
#include <QApplication>
#include <QStandardPaths>
#include <QDir>
#include <QFile>
#include <QFileInfo>

#ifdef Q_OS_WIN
#include <Windows.h>
#pragma comment(lib, "Gdi32.lib")
#pragma comment(lib, "User32.lib")
#endif

namespace UtilQt {

    bool isValidIpV4(const QString &str)
    {
        if(str.compare("localhost", Qt::CaseInsensitive) == 0){
            return true;
        }
        auto parts = str.split('.');
        if(parts.count() != 3) return false;

        auto ok = false;
        auto part1 = parts.at(0).toUInt(&ok, 10);
        if(!ok) return false;
        if(part1 < 0 || part1 > 255) return false;

        ok = false;
        auto part2 = parts.at(1).toUInt(&ok, 10);
        if(!ok) return false;
        if(part2 < 0 || part2 > 255) return false;

        ok = false;
        auto part3 = parts.at(2).toUInt(&ok, 10);
        if(!ok) return false;
        if(part3 < 0 || part3 > 255) return false;

        return true;
    }

    QString check_url(const QString &str)
    {
        const QString serachStr = QString("https://www.baidu.com/s?ie=utf-8&wd=")
                + str;
        auto qurl = QUrl::fromUserInput(str);
        // 构造不合法
        if(qurl.isEmpty() || !qurl.isValid() ){
            return serachStr;
        }
        //判断是否是localfile
        if(qurl.isLocalFile()){
            return qurl.toString();
        }
        auto host = qurl.host();
        // 判断是否是ipV4地址 包含 localhost
        if(isValidIpV4(host)){
            return qurl.toString();
        }
        // 判断域名
        auto parts = host.split(".");
        if(parts.count() <= 1 ) return serachStr;
        auto tld = parts.last();
        if(QUrl::idnWhitelist().contains(tld, Qt::CaseInsensitive)){
            return qurl.toString();
        }

        // TODO:判断是否是国际化域名

        qDebug()<<__FUNCTION__<<qurl;
        return qurl.toString();
    }

    bool dwmColorPrevalence()
    {
#ifdef Q_OS_WIN
    const QString path = "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\DWM";
    const QString key = "ColorPrevalence";
    QSettings settings(path, QSettings::NativeFormat);

    auto value = settings.value(key, false).toBool();
    return value;
#endif
    return false;
    }

    QColor getForgroundColor(const QColor &backgroundColor)
    {
        auto color = backgroundColor;
        double gray = (0.299 * color.red() +
                       0.587 * color.green() + 0.114 * color.blue()) / 255;
        return gray > 0.5 ? Qt::black : Qt::white;
    }
#ifdef Q_OS_WIN
    int sacleUI(int spec)
    {
        double rate = 0;
        QList<QScreen*> screens = QApplication::screens();
        if (screens.size() > 0) {
            QScreen* screen = screens[0];
            double dpi = screen->logicalDotsPerInch();
            rate = dpi / 96.0;

            if (rate < 1.1) {
                rate = 1.0;
            } else if (rate < 1.4) {
                rate = 1.25;
            } else if (rate < 1.6) {
                rate = 1.5;
            } else if (rate < 1.8) {
                rate = 1.75;
            } else {
                rate = 2.0;
            }
        }
        return int(spec * rate);
    }

    const float WIN_DEFAULT_DPI = 96.0;
    float windowsDpiScaleX()
    {
        HDC screen = CreateDC(L"DISPLAY", NULL, NULL, NULL);/*GetDC(0)*/;
        FLOAT dpiX = static_cast<FLOAT>(GetDeviceCaps(screen, LOGPIXELSX));
        ReleaseDC(0, screen);
        return dpiX / WIN_DEFAULT_DPI;

    //    float rate = 0.0f;
    //    QList<QScreen*> screens = QApplication::screens();
    //    if (screens.size() > 0) {
    //        QScreen* screen = screens[0];
    //        double dpi = screen->logicalDotsPerInchX();
    //        rate = dpi / 96.0;
    //    }
    //    return rate;
    }

    float windowsDpiScaleY() {
        HDC screen = CreateDC(L"DISPLAY", NULL, NULL, NULL);/*GetDC(0)*/;
        FLOAT dpiY = static_cast<FLOAT>(GetDeviceCaps(screen, LOGPIXELSY));
        ReleaseDC(0, screen);
        return dpiY / WIN_DEFAULT_DPI;

    //    float rate = 0.0f;
    //    QList<QScreen*> screens = QApplication::screens();
    //    if (screens.size() > 0) {
    //        QScreen* screen = screens[0];
    //        double dpi = screen->logicalDotsPerInchY();
    //        rate = dpi / 96.0;
    //    }
        //    return rate;
    }

    const QString appDataPath()
    {
        auto loc = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
        return loc;
    }

    const QString userDataPath(const QString &uerName)
    {
        auto dataPath = UtilQt::appDataPath();
        if(uerName.isEmpty()){
           return dataPath;
        }
        QDir dir(dataPath);
        if(!dir.cd(uerName)){
            dir.mkdir(uerName);
        }
        dir.cd(uerName);
        return dir.absolutePath();
    }

    QByteArray readFileUtf8(const QString &path)
    {
        QFile file(path);
        if(!file.exists()){
            return QByteArray();
        }
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
            return  QByteArray();
        }
        auto result = QString(file.readAll());
        file.close();

        return result.toUtf8();
    }

    long long writeStringToFile(const QString &path, const QString &data)
    {
        QFile file(path);

        if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
            return 0;

        file.write(data.toUtf8().data());
        file.close();

        return data.size();
    }

    std::string GetFileExtension(const std::string &path)
    {
        size_t sep = path.find_last_of(".");
        if (sep != std::string::npos)
            return path.substr(sep + 1);
        return std::string();
    }

    QString readFileString(const QString &path)
    {
        QString result;
        QFile file(path);
        if(!file.exists()){
            return result;
        }
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
            return result;
        }
        result = QString(file.readAll());
        file.close();

        return result;
    }

    QString GetFileNameFromURL(const QString &url)
    {
        QUrl qUrl(url);
        if(qUrl.isEmpty() || !qUrl.isValid()){
            return "default.ico";
        }

        QString subFix = QFileInfo(url).suffix();
        QString name = qUrl.host();
        return name + "." + subFix;
    }

    long long writeDataToFile(const QString &path, const QByteArray &data)
    {
        QFile file(path);

        if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
            return 0;

        file.write(data.data());
        file.close();

        return data.size();
    }

#endif

}
