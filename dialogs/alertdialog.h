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
    explicit AlertDialog(QWidget *parent = nullptr);
    ~AlertDialog();

    void setTitle(const QString &title);
    void setMsg(const QString &msg);

private:
    Ui::AlertDialog *ui;
};

#endif // ALERTDIALOG_H
