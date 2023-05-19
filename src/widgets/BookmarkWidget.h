#ifndef BOOKMARKWIDGET_H
#define BOOKMARKWIDGET_H

#include <QWidget>

//#include "persistences/bookmarkmanager.h"
#include "globaldef.h"

namespace Ui {
class BookmarkWidget;
}

class QStandardItem;
class StyledMenu;
class QAction;
class BookmarkWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BookmarkWidget(QWidget *parent = nullptr);
    ~BookmarkWidget();

    void onShowModeChanged(ToolWndShowMode mode);
signals:
    void pinOrCloseClicked();
public:
    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    Ui::BookmarkWidget *ui;
    void initUi();
    void initSignalSlots();

    // actions for more button
    StyledMenu *menu_more_;

protected:
    void showEvent(QShowEvent *event) override;

private slots:
    void onBookmarksChanged();
    void onAllBkmkTreeCustomContextMenu(const QPoint &pos);
};

#endif // BOOKMARKWIDGET_H
