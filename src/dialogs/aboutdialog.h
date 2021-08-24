#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>

namespace Ui {
class AboutDialog;
}
class QPropertyAnimation;
class AboutDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AboutDialog(QWidget *parent = nullptr);
    ~AboutDialog();

protected:
    void showEvent(QShowEvent *event) override;

private:
    Ui::AboutDialog *ui;
    QPropertyAnimation *anime_;
    bool need_close_ = false;

    void moveFrame(const QPoint &start,
                   const QPoint &end,
                   int duration = 300);
};

#endif // ABOUTDIALOG_H
