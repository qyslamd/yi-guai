#ifndef INFO_H
#define INFO_H

#include <QWidget>
#include <QLabel>
#include <QPainter>

class SizeInfoWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SizeInfoWidget(QWidget *parent = nullptr);

    void size(const QSize& size);

protected:
    void paintEvent(QPaintEvent *);

private:
    QLabel * label_ = nullptr;
    QPainter painter_;
};

#endif // INFO_H
