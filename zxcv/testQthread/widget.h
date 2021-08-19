#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QThread>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class SearchBar;
class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    bool event(QEvent *ev) override;
protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void on_pushButton_clicked();

private:
    Ui::Widget *ui;
    SearchBar *search_bar_ = nullptr;

};

#endif // WIDGET_H
