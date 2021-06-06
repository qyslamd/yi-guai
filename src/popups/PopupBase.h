#ifndef POPUPBASE_H
#define POPUPBASE_H

#include <QWidget>

namespace Ui {
class PopupBase;
}

class PopupBase : public QWidget
{
    Q_OBJECT

public:
    explicit PopupBase(QWidget *parent = nullptr);
    ~PopupBase();

    virtual QWidget *contentFrame();

    int shadowWidth() const;
    int shadowLeftWidth() const;
    int shadowRightWidth() const;
    int shadowBottomWidth() const;

protected:
    void mousePressEvent(QMouseEvent *event) override;

private:
    Ui::PopupBase *ui;
};

#endif // POPUPBASE_H
