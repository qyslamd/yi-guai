#ifndef ADDRESSBAR_H
#define ADDRESSBAR_H

#include <QLineEdit>

class QHBoxLayout;
class QToolButton;
class QAction;
class AddressBar final: public QLineEdit
{
    Q_OBJECT
public:
    AddressBar(QWidget *parent = nullptr);
private:
    QAction *btn_site_info_;    /*网站信息*/
    decltype (btn_site_info_)
    btn_find_hint_,     /*搜索提示*/
    btn_zoom_hint_,     /*缩放提示*/
    btn_mark_site_;     /*收藏站点*/

    void initUi();
    void setAppearance();

};

#endif // ADDRESSBAR_H
