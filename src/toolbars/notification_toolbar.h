#ifndef NOTIFICATION_TOOLBAR_H
#define NOTIFICATION_TOOLBAR_H

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

#endif // NOTIFICATION_TOOLBAR_H
