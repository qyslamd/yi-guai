#ifndef BOOKMARKBAR_H
#define BOOKMARKBAR_H

#include <QMenu>
#include <QFrame>
#include <QPushButton>

#include "globaldef.h"
#include "popups/StyledMenu.h"

class BookmarkMenu;
class BarItem;
class QMenu;
class QLabel;
class QStandardItem;
class QHBoxLayout;
class BookmarkBar : public QFrame
{
    Q_OBJECT
public:
    explicit BookmarkBar(QWidget *parent = nullptr);

signals:
    void cmdTriggered(BookmarkCmd cmd, const QVariant &data);
protected:
    void paintEvent(QPaintEvent *event) override;
    void showEvent(QShowEvent *event) override;
private:
    void initUi();
    QHBoxLayout *layout_;
    QToolButton *btn_niubi_;
    QLabel *label_empty_;
    QHBoxLayout *item_layout_;
    QPushButton *btn_others_;
    bool loaded_ = false;

    BookmarkMenu *makeMenu(const QStandardItem *item);

private slots:
    void onCustomContextMenuRequested(const QPoint &pos);
    void onBookmarksChanged();

};

class BarItem : public QPushButton
{
    Q_OBJECT
public:
    BarItem(const QIcon &icon, const QString &title, QWidget *parent = nullptr);
    BarItem(const QString &title, QWidget *parent = nullptr);
    BarItem(QWidget *parent = nullptr);

    QVariant data() const;
    void setData(const QVariant &var);
private:
    void init();

    QVariant data_;
};

class BookmarkMenu : public QMenu
{
    Q_OBJECT
public:
    BookmarkMenu(const QString &title, QWidget *parent = nullptr);
    BookmarkMenu(QWidget *parent = nullptr);

private:
    void initUi();

private slots:
    void onCustomContextMenuRequested(const QPoint &pos);
};

#endif // BOOKMARKBAR_H
