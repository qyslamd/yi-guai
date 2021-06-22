#ifndef UTIL_QT_H
#define UTIL_QT_H

#include <QVariant>

class QPaintDevice;
namespace UtilQt {
    extern bool isValidIpV4(const QString &str);
    extern QString check_url(const QString &str);
    /// \brief windows7以上版本，获取是否将主题颜色应用到标题栏和边框
    /// \return
    ///
    extern bool dwmColorPrevalence();

    // 根据背景颜色的明亮度获取前景色
    // 背景色亮，前景色就暗
    extern QColor getForgroundColor(const QColor &backgroundColor);

#ifdef Q_OS_WIN
    extern const float WIN_DEFAULT_DPI;
    extern int sacleUI(int spec);
    extern float windowsDpiScaleX();
    extern float windowsDpiScaleY();
    extern bool isWindows10();
#endif

    extern const QString appDataPath();
    extern const QString userDataPath();
    extern std::string GetFileExtension(const std::string& path);
    extern QString readFileString(const QString &path);
    extern QByteArray readFileUtf8(const QString &path);
    extern long long writeStringToFile(const QString &path, const QString &data);
    extern long long writeDataToFile(const QString &path, const QByteArray &data);
    extern QString GetFileNameFromURL(const QString &url);

    extern void drawShadow(QPaintDevice *device);

    extern QString getElideText(const QString &origin,
                                const QFont &font,
                                const int width,
                                Qt::TextElideMode mode = Qt::ElideRight);
    QString getRandomColor();
}

#endif // UTIL_QT_H
