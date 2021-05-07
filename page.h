#ifndef PAGE_H
#define PAGE_H

#include <QMainWindow>
#include "globaldef.h"

class QHBoxLayout;
class QVBoxLayout;
class CefQWidget;
class QDockWidget;

///
/// \brief The Page class
/// 每一个独立的标签页
class Page : public QMainWindow
{
    Q_OBJECT
public:
    explicit Page(const QString &startup_url, QWidget *parent = nullptr);

    CefQWidget* getBrowserWidget();
public slots:
    void onTopLevelWindowStateChanged(Qt::WindowStates state, const QVariant &data);
signals:
    void pageCmd(PageCmd cmd, const QVariant &data);
protected:
    void closeEvent(QCloseEvent *event) override;
private:
    QVBoxLayout *main_layout_ = nullptr;   /*整体布局*/
    CefQWidget *browser_widget_ = nullptr;    /*浏览器QWidget*/
    QDockWidget *dock_dev_tool_ = nullptr;    /*开发者工具停靠窗口*/
private:
    void initUi(const QString &url);

private slots:
    void onBrowserClosing();
};

#endif // PAGE_H
