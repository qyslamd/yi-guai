#ifndef BUTTONLINEEDIT_H
#define BUTTONLINEEDIT_H

#include <QFrame>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QToolButton>
#include <QPushButton>

class ButtonLineEdit : public QFrame
{
    Q_OBJECT
public:
    ButtonLineEdit(QWidget *parent = nullptr);

    bool eventFilter(QObject *obj, QEvent *ev) override;

private:
    QHBoxLayout *layout_;
    QPushButton *btnSiteInfo_;
    QLineEdit *lineEdit_;
    QToolButton *btnZoom_;
    QToolButton *btnMark_;
};

#endif // BUTTONLINEEDIT_H
