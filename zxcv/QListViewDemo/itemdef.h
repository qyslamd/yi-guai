#ifndef ITEMDEF_H
#define ITEMDEF_H

#include <QMetaType>
#include <QPixmap>

typedef enum{
    S_RED,
    S_BLUE,
    S_YELLOW,
} ItemStatus;

struct ItemData{
    QPixmap icon;
    QString name;
    QString tel;
};

Q_DECLARE_METATYPE(ItemData)

#endif // ITEMDEF_H
