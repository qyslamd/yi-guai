#ifndef POPUPBASE_H
#define POPUPBASE_H

#include <QFrame>

namespace Ui {
class PopupBase;
}

class PopupBase : public QFrame
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
