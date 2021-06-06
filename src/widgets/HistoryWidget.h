#ifndef HISTORYWIDGET_H
#define HISTORYWIDGET_H

#include <QWidget>
#include "globaldef.h"

namespace Ui {
class HistoryWidget;
}

class HistoryWidget : public QWidget
{
    Q_OBJECT

public:
     explicit HistoryWidget(QWidget *parent = nullptr);
    ~HistoryWidget();
    void onShowModeChanged(ToolWndShowMode mode);
signals:
    void pinOrCloseClicked(bool pin);

private:
    Ui::HistoryWidget *ui;
};

#endif // HISTORYWIDGET_H
