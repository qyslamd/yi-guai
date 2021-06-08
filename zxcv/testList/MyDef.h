#ifndef MYDEF_H
#define MYDEF_H

#include <QVariant>
#include <QString>
#include <QPixmap>

enum ItemStatus {
    S_RED,
    S_BLUE,
    S_YELLOW
};

struct ItemData{
    QString name;
    QString tel;
    QPixmap icon;
};

Q_DECLARE_METATYPE(ItemData);

#endif // MYDEF_H
