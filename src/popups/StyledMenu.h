#ifndef STYLEDMENU_H
#define STYLEDMENU_H

#include <QMenu>

class StyledMenu : public QMenu
{
    Q_OBJECT
public:
    StyledMenu(const QString &title, QWidget *parent = nullptr);
    StyledMenu(QWidget *parent = nullptr);
};

#endif // STYLEDMENU_H
