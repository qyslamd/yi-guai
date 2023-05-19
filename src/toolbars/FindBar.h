#ifndef FINDBAR_H
#define FINDBAR_H

#include <QDialog>

namespace Ui {
class FindBar;
}

class FindBar : public QDialog
{
    Q_OBJECT

public:
    explicit FindBar(QWidget *parent = nullptr);
    ~FindBar();
    void setFindResult(int count, int index);
    void closeFind();
signals:
    void searchStop();
    void search(const QString &arg, bool matchCase);
    void searchUp(const QString &arg, bool matchCase);
    void searchDown(const QString &arg, bool matchCase);
protected:
    void showEvent(QShowEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
private:
    Ui::FindBar *ui;
    void initUi();
private slots:
    void onBtnUpClicked();
    void onBtnDownClicked();
    void onLineEditTextEdited(const QString &arg1);
    void onLineEditTextChanged(const QString &arg1);
    void onLineEditReturnPressed();
};

#endif // FINDBAR_H
