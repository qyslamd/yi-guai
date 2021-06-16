#ifndef BOOKMARKBAR_H
#define BOOKMARKBAR_H

#include <QFrame>
#include "globaldef.h"

class BookmarkBar : public QFrame
{
    Q_OBJECT
public:
    explicit BookmarkBar(QWidget *parent = nullptr);

signals:

protected:
    void paintEvent(QPaintEvent *event) override;

};

#endif // BOOKMARKBAR_H
