#ifndef ALERTDIALOG_H
#define ALERTDIALOG_H

#include <QDialog>

namespace Ui {
class AlertDialog;
}

class AlertDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AlertDialog(const QString &title, const QString &msg, QWidget *parent = nullptr);
    ~AlertDialog();

private:
    Ui::AlertDialog *ui;
};

#endif // ALERTDIALOG_H
