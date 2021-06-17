#ifndef BOOKMARKBAR_H
#define BOOKMARKBAR_H

#include <QMenu>
#include <QFrame>
#include <QPushButton>

#include "globaldef.h"

class BookmarkMenu;
class BarItem;
class QMenu;
class QStandardItem;
class QHBoxLayout;
class BookmarkBar : public QFrame
{
    Q_OBJECT
public:
    explicit BookmarkBar(QWidget *parent = nullptr);

signals:
protected:
    void paintEvent(QPaintEvent *event) override;
    void showEvent(QShowEvent *event) override;
private:
    void initUi();
    QHBoxLayout *layout_;
    QHBoxLayout *item_layout_;
    QPushButton *btn_others_;
    bool loaded_ = false;

    BookmarkMenu *makeMenu(const QStandardItem *item);

private slots:
    void onCustomContextMenuRequested(const QPoint &);
    void onBookmarkBarLoadFinished(const QStandardItem* item);

};

class BarItem : public QPushButton
{
    Q_OBJECT
public:
    BarItem(const QIcon &icon, const QString &title, QWidget *parent = nullptr);
    BarItem(const QString &title, QWidget *parent = nullptr);
    BarItem(QWidget *parent = nullptr);

private:
    void init();
};

class BookmarkMenu : public QMenu
{
    Q_OBJECT
public:
    BookmarkMenu(const QString &title, QWidget *parent = nullptr);
    BookmarkMenu(QWidget *parent = nullptr);

private:
    void initUi();
};

#endif // BOOKMARKBAR_H
