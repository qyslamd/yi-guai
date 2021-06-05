#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QToolButton>
#include <QLineEdit>
#include "ButtonLineEdit.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

#include <QStackedWidget>
#include <QFrame>
#include <QHBoxLayout>

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    bool eventFilter(QObject *obj, QEvent *ev) override;
protected:
    void timerEvent(QTimerEvent *event) override;
private:
    Ui::Widget *ui;

    QPixmap switchPix(bool on);

    void testMenuBar();
    void testShortcut();
    void testLineEdit();
    void testTabShape();
};
#endif // WIDGET_H
