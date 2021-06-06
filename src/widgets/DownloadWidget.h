#ifndef DOWNLOADWIDGET_H
#define DOWNLOADWIDGET_H

#include <QWidget>
#include "globaldef.h"

namespace Ui {
class DownloadWidget;
}

class DownloadWidget : public QWidget
{
    Q_OBJECT

public:
     explicit DownloadWidget(QWidget *parent = nullptr);
    ~DownloadWidget();
    void onShowModeChanged(ToolWndShowMode mode);
signals:
    void pinOrCloseClicked(bool pin);

private:
    Ui::DownloadWidget *ui;
};

#endif // DOWNLOADWIDGET_H
