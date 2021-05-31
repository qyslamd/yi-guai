#ifndef ADDRESSBAR_H
#define ADDRESSBAR_H

#include <QLineEdit>

class QHBoxLayout;
class QToolButton;
class QAction;
class QCompleter;
class QStringListModel;

class AddressBar final: public QLineEdit
{
    Q_OBJECT
public:
    AddressBar(QWidget *parent = nullptr);
    bool eventFilter(QObject *obj, QEvent *ev) override;
    bool event(QEvent *ev) override;

    QRect gGeometryBtnSiteInfo() const;
    void setInprivate(bool inprivate);
signals:
    void viewSiteInfo();

protected:
    void mousePressEvent(QMouseEvent *event) override;
private:
    QCompleter *completer_;
    QStringListModel *model_;
    QAction *btn_site_info_;
    decltype (btn_site_info_)
    btn_find_hint_,
    btn_zoom_hint_,
    btn_mark_site_;

    QToolButton *internal_btn_siteInfo_ = nullptr;

private:
    void initUi();
    void setAppearance();
    void fuckButton();

private slots:
    void onEditingFinishsed();

};

#endif // ADDRESSBAR_H
