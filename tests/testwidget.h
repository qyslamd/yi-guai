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
    void changeEvent(QEvent *event) override;
private:
    Ui::TestWidget *ui;
    int FrameWidth = 10;
    const int CaptionHeight = 30;

    enum class Region{
        HT_Error = -2,
        HT_Left,
        HT_TopLeft,
        HT_Top,
        HT_Caption,
        HT_MinButton,
        HT_MaxButton,
        HT_CloseButton,
        HT_TopRight,
        HT_Right,
        HT_BottomRight,
        HT_Bottom,
        HT_BottomLeft,
        HT_Client
    };

    bool mouse_left_btn_pressed_ = false;
    QPoint mouse_left_btn_press_pos_;
    Region region_;

    // Determine the area and change the shape during the mouse movement
    void judgeRegion(const QPoint &windowPos);

    ///
    /// \brief The CaptionButtons enum
    ///
    enum class CaptionButtons{
        Button_None = -1,
        Button_Mini,
        Button_NormalMax,
        Button_Close,
    };

    bool min_button_hover_ = false,
    max_button_hover_ = false,
    close_button_hover_ = false;

    bool min_button_press_ = false,
    max_buttton_press_ = false,
    close_button_press_ = false;

    // Get rectangle of caption buttons
    QRectF btnRect(CaptionButtons button);
    QPixmap btnPixmap(CaptionButtons button);

    void drawShadow(QPainter *p);
    void drawButtons(QPainter *p);

    void clearButtonHover();

};

#endif // TESTWIDGET_H
