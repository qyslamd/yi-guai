#ifndef FULLSCNHINT_H
#define FULLSCNHINT_H

#include <QFrame>

namespace Ui {
class FullscnHint;
}

class FullscnHint : public QFrame
{
    Q_OBJECT

public:
    explicit FullscnHint(QWidget *parent = nullptr);
    ~FullscnHint() override;

    void setKeyStr(const QString &str);
protected:
    void paintEvent(QPaintEvent *e) override;
private:
    Ui::FullscnHint *ui;
};

#endif // FULLSCNHINT_H
