#ifndef HISTORYWIDGET_H
#define HISTORYWIDGET_H

#include <QWidget>

namespace Ui {
class HistoryWidget;
}

class HistoryWidget : public QWidget
{
    Q_OBJECT

public:
     explicit HistoryWidget(QWidget *parent = nullptr);
    ~HistoryWidget();
signals:
    void pinOrCloseClicked();

private:
    Ui::HistoryWidget *ui;
};

#endif // HISTORYWIDGET_H
