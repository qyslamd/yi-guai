#ifndef QT_WIN_FRAMELESS_H
#define QT_WIN_FRAMELESS_H

#include <qsystemdetection.h>
#include <QMainWindow>

#if defined(Q_OS_WIN)

class QScreen;
class QtWinFramelessWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit QtWinFramelessWindow(QWidget *parent = 0);
    virtual bool event(QEvent *ev) override;
signals:
    void DwmCompsitionChanged();
    void dpiChanged(const int dpi);
    void screenChanged(QScreen *screen);
protected:
    virtual bool nativeEvent(const QByteArray &eventType,
                             void *message, long *result) override;
    virtual void moveEvent(QMoveEvent *event) override;
    ///
    /// \brief Implemention in subclass to judge HTCAPTION
    /// \param gPos - global cursor postion
    /// \return return true means HTCAPTION, otherwise HTCLIENT
    ///
    virtual bool hitTestCaption(const QPoint &gPos);
private:
    bool maximized_ = false;
};
#elif defined (Q_OS_LINUX1)
class QtWinFramelessWindow : public QMainWindow
{
public:
    explicit QtWinFramelessWindow(QWidget *parent = nullptr);
    bool event(QEvent *ev) override;

protected:
    virtual bool nativeEvent(const QByteArray &eventType, void *message, long *result) override;
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void leaveEvent(QEvent *event) override;
    virtual void changeEvent(QEvent *event) override;
private:
    int FrameWidth = 4;
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
    void setFrameWidth(int width);

    // Get rectangle of caption buttons
    QRectF btnRect(CaptionButtons button);
    QPixmap btnPixmap(CaptionButtons button);
    CaptionButtons buttonAt(const QPoint &pos);

    void drawShadow(QPainter *p);
    void drawButtons(QPainter *p);

    void clearButtonHover();
};
#else
class QtWinFramelessWindow : public QMainWindow
{
public:
    explicit QtWinFramelessWindow(QWidget *parent = nullptr)
        : QMainWindow(parent){}
};
#endif

#endif // QT_WIN_FRAMELESS_H
