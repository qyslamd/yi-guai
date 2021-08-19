#ifndef FRAME2_H
#define FRAME2_H

#include <QFrame>

namespace Ui {
class Frame2;
}

class Frame2 : public QFrame
{
    Q_OBJECT

public:
    explicit Frame2(QWidget *parent = nullptr);
    ~Frame2();

signals:
    void btnOkClicked();
    void btnCancelClicked();

private:
    Ui::Frame2 *ui;
};

#endif // FRAME2_H
