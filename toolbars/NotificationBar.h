#ifndef NOTIFICATIONBAR_H
#define NOTIFICATIONBAR_H

#include <QFrame>

namespace Ui {
class NotificationBar;
}

class NotificationBar : public QFrame
{
    Q_OBJECT

public:
    explicit NotificationBar(QWidget *parent = nullptr);
    ~NotificationBar();
protected:
    void paintEvent(QPaintEvent *event) override;
private:
    Ui::NotificationBar *ui;
};

#endif // NOTIFICATIONBAR_H
