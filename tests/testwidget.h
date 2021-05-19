#ifndef TESTWIDGET_H
#define TESTWIDGET_H

#include <QWidget>

namespace Ui {
class TestWidget;
}

class TestWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TestWidget(QWidget *parent = nullptr);
    ~TestWidget();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void leaveEvent(QEvent *event) override;
private:
    Ui::TestWidget *ui;
    const int FrameWidth = 10;
    const int CaptionHeight = 30;

    enum class TenRegion{
        Left = 1,
        TopLeft,
        Top,
        TitleBar,
        TopRight,
        Right,
        BottomRight,
        Bottom,
        BottomLeft,
        Middle
    };

    bool mouse_left_btn_pressed_ = false;
    QPoint mouse_left_btn_press_pos_;
    TenRegion region_;

    // Determine the area and change the shape during the mouse movement
    void judgeRegionChangeCursor(const QPoint &posInThis);

};

#endif // TESTWIDGET_H
