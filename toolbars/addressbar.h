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
    bool eventFilter(QObject *obj, QEvent *ev) override;
    bool event(QEvent *ev) override;

    QRect gGeometryBtnSiteInfo() const;
signals:
    void viewSiteInfo();

protected:
    void mousePressEvent(QMouseEvent *event) override;
private:
    QAction *btn_site_info_;    /*网站信息*/
    decltype (btn_site_info_)
    btn_find_hint_,     /*搜索提示*/
    btn_zoom_hint_,     /*缩放提示*/
    btn_mark_site_;     /*收藏站点*/

    QToolButton *internal_btn_siteInfo_ = nullptr;

private:
    void initUi();
    void setAppearance();
    void fuckButton();

};

#endif // ADDRESSBAR_H
