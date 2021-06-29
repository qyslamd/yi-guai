#ifndef QTFRAMELESS_WND_H
#define QTFRAMELESS_WND_H

#include <QWidget>
#include <QFrame>
#include <QObject>
#include <QHBoxLayout>

class QtFrameLessWnd : public QWidget
{
    Q_OBJECT
public:
    QtFrameLessWnd(QWidget *parent = nullptr);

    bool event(QEvent *ev) override;

    ///
    /// \brief set main widget,the widget will become the child of this,
    /// Any previously set widget will be hidden.
    /// \param widget
    ///
    void setWidget(QWidget *widget);

    QWidget *widget();

protected:
    virtual bool nativeEvent(const QByteArray &eventType, void *message, long *result) override;
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void leaveEvent(QEvent *event) override;
    virtual void changeEvent(QEvent *event) override;
private:
    int FrameWidth = 8;
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

    ///
    /// \brief The CaptionButtons enum
    ///
    enum class CaptionButtons{
        Button_None = -1,
        Button_Mini,
        Button_NormalMax,
        Button_Close,
    };

    bool mouse_left_btn_pressed_ = false;
    QPoint mouse_left_btn_press_pos_;
    Region region_;

    // Determine the area and change the shape during the mouse movement
    void judgeRegion(const QPoint &windowPos);

    bool min_button_hover_ = false,
    max_button_hover_ = false,
    close_button_hover_ = false;

    bool min_button_press_ = false,
    max_buttton_press_ = false,
    close_button_press_ = false;

    QWidget *central_widget = nullptr;
    QHBoxLayout *layout_ = nullptr;

    void setFrameWidth(int width);

    // Get rectangle of caption buttons
    QRectF btnRect(CaptionButtons button);
    QPixmap btnPixmap(CaptionButtons button);
    CaptionButtons buttonAt(const QPoint &pos);

    void drawShadow(QPainter *p);
    void drawButtons(QPainter *p);

    void clearButtonHover();
};

#endif // QTFRAMELESS_WND_H
