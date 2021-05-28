#ifndef TABPAGESBAR_H
#define TABPAGESBAR_H

#include <QFrame>
#include "globaldef.h"

class QToolButton;
class TabBar;
class QHBoxLayout;
class TabPagesBar final: public QFrame
{
    Q_OBJECT
public:
    explicit TabPagesBar(bool inprivate, QWidget *parent = nullptr);
    ~TabPagesBar();
    int insertTab(int index, const QString &text);
    int count()const;
    void moveTab(int from, int to);
    void removeTab(int index);
    void setTabIcon(int index, const QIcon &icon);
    void setTabText(int index, const QString &text);
    QString tabText(int index) const;
signals:
    void currentChanged(int index);
    void tabCloseRequested(int index);
    void tabMoved(int from, int to);
    void showPreview(const QPoint &g_pos, const int index);
    void addPage();
    void showDockPage();
    void tabbarMenuTriggered(TabBarCmd cmd, const QVariant &data);
    void testBtnClicked();
public slots:
    void setCurrentIndex(int index);
    void onDwmColorChanged();
private:
    bool inprivate_;
    QHBoxLayout *layout_;
    QToolButton *btn_dock_tabs_ = nullptr;
    TabBar *tab_bar_;
    QToolButton *btn_add_page_;

private:
    void initUi();
};

#endif // TABPAGESBAR_H
