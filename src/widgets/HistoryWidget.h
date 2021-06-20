#ifndef HISTORYWIDGET_H
#define HISTORYWIDGET_H

#include <QWidget>
#include "globaldef.h"

namespace Ui {
class HistoryWidget;
}

class StyledMenu;
class QStandardItemModel;
class HistoryWidget : public QWidget
{
    Q_OBJECT

public:
     explicit HistoryWidget(QWidget *parent = nullptr);
    ~HistoryWidget();
    void onShowModeChanged(ToolWndShowMode mode);

    bool eventFilter(QObject *obj, QEvent *ev) override;

protected:
    void showEvent(QShowEvent *event) override;
signals:
    void pinOrCloseClicked(bool pin);
    void menuCmd(HistoryCmd cmd, const QVariant &para);


private:
    Ui::HistoryWidget *ui;
    QStandardItemModel* recently_model_;

    StyledMenu *menu_in_all_;
    QAction *ac_open_;
    QAction *ac_open_in_new_tab_;
    QAction *ac_open_in_new_window_;
    QAction *ac_open_in_inpriavte_;
    //---------------------------
    QAction *ac_delete_;
    QAction *ac_copy_link_;
    //---------------------------
    QAction *ac_add2favorite_;

    History menu_trigger_data_;

    void initUi();
    void initSignalSlots();
    void setIcons();

    void loadRecentlyHistories();
private slots:
    void onTreeAllHisItemClicked(const QModelIndex &index);
    void onTreeAllHisContextMenu(const QPoint &pos);
};

#endif // HISTORYWIDGET_H
