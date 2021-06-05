﻿#ifndef EDITMENU_H
#define EDITMENU_H

#include <QWidget>
#include <QPen>
#include "../widgets/iconbutton.h"

class EditMenu : public QWidget
{
    Q_OBJECT
public:
    explicit EditMenu(QWidget *parent = nullptr);

    virtual QPen pen() const { return pen_; }
    virtual void pen(QPen pen) { pen_ = pen; emit styleChanged(); }

    virtual bool fill() const { return fill_; }
    virtual void fill(bool fill) { fill_ = fill; emit styleChanged(); }

    virtual void style(QPen pen, bool fill) { fill_ = fill; pen_ = pen; emit styleChanged(); }

signals:
    void changed();
    void styleChanged();

protected:
    void addButton(CustomButton *);
    void addSeparator();
    void addWidget(QWidget *);

    const int HEIGHT = 35;
    const int ICON_W = 25;

    QPen pen_;
    bool fill_ = false;
};

#endif // EDITMENU_H
