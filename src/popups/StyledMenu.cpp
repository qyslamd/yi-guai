#include "StyledMenu.h"

StyledMenu::StyledMenu(const QString &title, QWidget *parent)
    : QMenu(title, parent)
{
    setWindowFlag(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setAutoFillBackground(true);
}

StyledMenu::StyledMenu(QWidget *parent)
    : QMenu(parent)
{
    setWindowFlag(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setAutoFillBackground(true);
}
