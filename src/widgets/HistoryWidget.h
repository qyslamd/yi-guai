#ifndef HISTORYWIDGET_H
#define HISTORYWIDGET_H

#include <QWidget>
#include "globaldef.h"

namespace Ui {
class HistoryWidget;
}

class QStandardItemModel;
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
    QStandardItemModel* all_model_;

    void initUi();
    void initSignalSlots();

    void loadAllHistories();
};

#endif // HISTORYWIDGET_H
