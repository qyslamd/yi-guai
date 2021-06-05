#ifndef MYFRAME_H
#define MYFRAME_H

#include <QFrame>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QToolButton>
#include <QPushButton>

class MyFrame : public QFrame
{
    Q_OBJECT
public:
    MyFrame(QWidget *parent = nullptr);

    bool eventFilter(QObject *obj, QEvent *ev) override;

private:
    QHBoxLayout *layout_;
    QPushButton *btnSiteInfo_;
    QLineEdit *lineEdit_;
    QToolButton *btnZoom_;
    QToolButton *btnMark_;
};

#endif // MYFRAME_H
