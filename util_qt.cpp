#include "util_qt.h"
#include <QUrl>
#include <QtDebug>

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

}
