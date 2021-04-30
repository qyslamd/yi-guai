#ifndef DP_WIDGETCAPTION_H
#define DP_WIDGETCAPTION_H

#include <QWidget>

#ifdef Q_OS_WIN
class Dp_Widget_Caption : public QWidget
{
    Q_OBJECT
public:
    explicit Dp_Widget_Caption(bool isInprivate, QWidget *parent = 0);

    bool event(QEvent *e) override;
    bool isInprivate() const {return inprivate_;}

    void setSkinBtnEnable(bool enable){skin_btn_enable_ = enable;}
signals:
    void btnSkinClicked(const QVariant &para);
    void btnFullScrnClicked();
    void btnMinClicked();
    void btnMaxNormalClicked();
    void btnCloseClicked();
public slots:

protected:
    void paintEvent(QPaintEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void leaveEvent(QEvent *event) override;

private:
    enum RectType{
        Rect_Other = -1,
        Rect_Skin,
        Rect_FullScrn,
        Rect_Mini,
        Rect_NormalMax,
        Rect_Close,
    };

    enum IconType{
        Icon_Skin,
        Icon_FullScrn,
        Icon_Mini,
        Icon_Max,
        Icon_Restore,
        Icon_Close,
    };

    bool inprivate_  = false;   // 是否是无痕模式的窗口

    bool skin_hover_ = false;
    bool full_hover_ = false;
    bool mini_hover_ = false;
    bool max_hover_  = false;
    bool close_hover_ = false;

    bool skin_btn_enable_ = false;  // 换肤按钮是否出现
    bool full_btn_enable_ = false;  // 全屏按钮是否需要

    bool skin_pressed_ = false;
    bool full_pressed_ = false;
    bool mini_pressed_ = false;
    bool max_pressed_  = false;
    bool close_pressed_ = false;

    QRectF getRect(RectType type);
    QPixmap getPixmap(IconType type,const QColor &color);
    QColor getIconColor(bool isClose = false);
    int itemAt(const QPoint &pos);
};
#else
class Dp_Widget_Caption : public QWidget
{
    Q_OBJECT
public:
    Dp_Widget_Caption(bool isInprivate, QWidget *parent = nullptr)
        : QWidget(parent)
        , inprivate_(isInprivate)
    {
    }
    bool isInprivate() const {return inprivate_;}
private:
    bool inprivate_  = false;
};
#endif // Q_OS_WIN

#endif // DP_WIDGETCAPTION_H
