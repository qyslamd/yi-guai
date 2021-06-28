#ifndef BOOKMARKBAR_H
#define BOOKMARKBAR_H

#include <QMenu>
#include <QFrame>
#include <QPushButton>
#include <QToolBar>

#include "globaldef.h"
#include "popups/StyledMenu.h"

class BookmarkMenu;
class BarItem;
class QMenu;
class QLabel;
class QStandardItem;
class QHBoxLayout;
class BookmarkToolBar;
class BookmarkBar : public QFrame
{
    Q_OBJECT
public:
    explicit BookmarkBar(QWidget *parent = nullptr);

signals:
    void cmdTriggered(BookmarkCmd cmd, const QVariant &data);
    void appBtnClicked(const QVariant &data);
protected:
    void paintEvent(QPaintEvent *event) override;
    void showEvent(QShowEvent *event) override;
private:
    void initUi();
    QHBoxLayout *layout_;
    QPushButton *btn_application_;
    QLabel *label_empty_;
    BookmarkToolBar *toolbar_;
    QPushButton *btn_others_;

    QColor test_color_;
    int test_int_ = 1;
    bool loaded_ = false;

    BookmarkMenu *makeMenu(const QStandardItem *item);

private slots:
    void onCustomContextMenuRequested(const QPoint &pos);
    void loadBookmarks();

};

class BookmarkToolBar : public QToolBar
{
    Q_OBJECT
public:
    explicit BookmarkToolBar(QWidget *parent = nullptr);
    explicit BookmarkToolBar(const QString &title, QWidget *parent = nullptr);

    bool event(QEvent *ev) override;

private:
    void initUi();
};

#endif // BOOKMARKBAR_H
