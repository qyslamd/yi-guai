#ifndef ADDRESSBAR_H
#define ADDRESSBAR_H

#include <QLineEdit>

class QHBoxLayout;
class QToolButton;
class AddressBar final: public QLineEdit
{
public:
    AddressBar(QWidget *parent = nullptr);

    void setSpacing(int spacing);

private:
    QHBoxLayout *layout_;   /*整体布局*/

    QToolButton *btn_site_info_;    /*网站信息*/
    decltype (btn_site_info_)
    btn_find_hint_,     /*搜索提示*/
    btn_zoom_hint_,     /*缩放提示*/
    btn_mark_site_;     /*收藏站点*/

};

#endif // ADDRESSBAR_H
