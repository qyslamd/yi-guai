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
    void onBkmkBarVisibleChanged();
signals:
    void pinOrCloseClicked();
    void menuCmd(BookmarkCmd cmd, const QVariant &para);
public:
    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    Ui::BookmarkWidget *ui;
    void initUi();
    void initSignalSlots();
    QStandardItem * menu_trigger_item_;

    // actions for more button
    StyledMenu *menu_more_;
    QAction *action_manage_bookmarks_;         // 管理书签
                                               // ------------------
    QAction *action_add_current_;              // 将当前页添加到收藏夹
    QAction *action_add_all_;                  // 将所有页添加到收藏夹
                                               // ------------------
    QAction *action_import_bookmarks_;         // 导入收藏夹
    QAction *action_export_bookmarks_;         // 导出收藏夹
    QAction *action_delete_duplicate_;         // 删除重复的收藏夹
                                               // ------------------
    QAction *action_show_bookmark_bar_;        // 显示收藏夹栏
    QAction *action_show_bookmark_btn_;        // 在工具栏中显示/隐藏收藏夹按钮

    QAction *ac_details_;
    // actions for bookmarks
    StyledMenu *menu_tree_bkmk_;
    QAction *ac_open_bkmk_in_new_page_;        // 在新标签页中打开
    QAction *ac_open_bkmk_in_new_window_;      // 在新窗口中打开
    QAction *ac_open_in_inprivate_;            // 在新无痕窗口中打开
                                               // -----------------
    QAction *ac_rename_bkmk_;                  // 重命名
    QAction *ac_copy_link_;                    // 复制链接
    QAction *ac_modify_bkmk_;                  // 编辑
    QAction *ac_delte_bkmk_;                   // 删除

    // actions for bookmark folder
    StyledMenu *menu_tree_bkmk_dir_;
    QAction *ac_open_all_bkmk_page_;           // 在新标签页中打开所有
    QAction *ac_open_all_bkmk_window_;         // 在新窗口中打开所有
    QAction *ac_open_all_in_inprivate_;        // 在新无痕窗口中打开
                                               // -----------------
    QAction *ac_sort_as_name_;                 // 按名称排序
    QAction *ac_rename_folder_;                // 重命名
    QAction *ac_delte_folder_;                 // 删除
                                               // -----------------
    QAction *ac_add_cur_to_folder_;            // 当前页添加到文件夹
    QAction *ac_add_all_to_folder_;            // 将所有页添加到文件夹

protected:
    void showEvent(QShowEvent *event) override;

private slots:
    void onBookmarksChanged();
    void onAllBkmkTreeCustomContextMenu(const QPoint &pos);
};

#endif // BOOKMARKWIDGET_H
