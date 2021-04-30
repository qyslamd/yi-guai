#ifndef FRAMEWIDGET_H
#define FRAMEWIDGET_H

#include <QWidget>

namespace Ui {
class CaptionWidget;
}


///
/// \brief The FrameWidget class
///
class FrameWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FrameWidget(QWidget *parent = nullptr);
    ~FrameWidget();
protected:
    void paintEvent(QPaintEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void leaveEvent(QEvent *event) override;

private:
    const int PADDING = 9;
    enum Direction { UP=0, DOWN=1, LEFT, RIGHT,
                     LEFTTOP, LEFTBOTTOM, RIGHTBOTTOM, RIGHTTOP, NONE };

    Ui::CaptionWidget *ui;
    bool isLeftPressDown;
    QPoint dragPosition;
    Direction dir;

    void region(const QPoint &pos);
};

#endif // FRAMEWIDGET_H
