#ifndef APPCONFIGWIDGET_H
#define APPCONFIGWIDGET_H

#include <QWidget>

namespace Ui {
class AppCfgWidget;
}

class AppCfgWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AppCfgWidget(QWidget *parent = nullptr);
    ~AppCfgWidget();

private:
    Ui::AppCfgWidget *ui;
};

#endif // APPCONFIGWIDGET_H
