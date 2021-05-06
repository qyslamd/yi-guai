#ifndef NAVIBAR_H
#define NAVIBAR_H

#include <QFrame>

class QHBoxLayout;
class QToolButton;
class AddressBar;

class NaviBar final: public QFrame
{
public:
    NaviBar(QWidget *parent = nullptr);
    void setSpacing(int spacing);

private:
    QHBoxLayout *layout_;   /*整体布局*/

    QToolButton *btn_back_; /*后退*/
    decltype (btn_back_) btn_forward_,  /*前进*/
    btn_refresh_stop_,  /*刷新/停止*/
    btn_home_,  /*主页*/
    btn_user_,   /*用户*/
    btn_other_options_; /*其它选项*/

    AddressBar *address_bar_;   /*地址栏及其孩子*/
    QFrame *frame_extensions_,  /*扩展容器*/
    *frame_tools_;  /*工具容器*/

};

#endif // NAVIBAR_H
