#ifndef NAVIBAR_H
#define NAVIBAR_H

#include <QFrame>
#include "globaldef.h"

class QHBoxLayout;
class QToolButton;
class AddressBar;

class NaviBar final: public QFrame
{
    Q_OBJECT
public:
    NaviBar(QWidget *parent = nullptr);
    void setSpacing(int spacing);
    void setAddress(const QString &url);
    void setLoadingState(bool isLoading, bool canGoBack, bool canGoForward);

signals:
    void naviBarCmd(NaviBarCmd cmd, const QVariant &para);
protected:
    void paintEvent(QPaintEvent *event) override;
private:
    QHBoxLayout *layout_;   /*整体布局*/

    QToolButton *btn_back_; /*后退*/
    decltype (btn_back_) btn_refresh_,  /*刷新*/
    btn_stop_,  /*停止加载*/
    btn_forward_,  /*前进*/
    btn_home_,  /*主页*/
    btn_user_,   /*用户*/
    btn_more_options_; /*其它选项*/

    AddressBar *address_bar_;   /*地址栏及其孩子*/
    QFrame *frame_extensions_,  /*扩展容器*/
    *frame_tools_;  /*工具容器*/

private:
    void initSignals();
    void setAppearance();

};

#endif // NAVIBAR_H
