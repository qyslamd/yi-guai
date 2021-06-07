#ifndef MYDEF_H
#define MYDEF_H

#include <QVariant>
#include <QString>

enum ItemStatus {
    S_RED,
    S_BLUE,
    S_YELLOW
};

struct ItemData{
    QString name;
    QString tel;
};

Q_DECLARE_METATYPE(ItemData);

#endif // MYDEF_H
