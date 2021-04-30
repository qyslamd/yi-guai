#ifndef UTIL_QT_H
#define UTIL_QT_H

#include <QVariant>

namespace UtilQt {
    bool isValidIpV4(const QString &str);
    QString check_url(const QString &str);
    /// \brief windows7以上版本，获取是否将主题颜色应用到标题栏和边框
    /// \return
    ///
    bool dwmColorPrevalence();

    // 根据背景颜色的明亮度获取前景色
    // 背景色亮，前景色就暗
    QColor getForgroundColor(const QColor &backgroundColor);
}

#endif // UTIL_QT_H
