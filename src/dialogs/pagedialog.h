#ifndef PAGEDIALOG_H
#define PAGEDIALOG_H

#include <QWidget>

namespace Ui {
class PageDialog;
}

class PageDialog : public QWidget
{
    Q_OBJECT

public:
    explicit PageDialog(QWidget *parent = nullptr);
    ~PageDialog();
signals:
    void btnOkClicked();
private:
    Ui::PageDialog *ui;
};

#endif // PAGEDIALOG_H
