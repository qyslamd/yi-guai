#ifndef QT_WIN_FRAMELESS_OLD_H
#define QT_WIN_FRAMELESS_OLD_H

#include <qsystemdetection.h>
#include <QObject>
#include <QMainWindow>

#if defined(Q_OS_WIN)

#include <QWidget>
#include <QList>
#include <QMargins>
#include <QRect>
class QtWinFramelessWindowOld : public QMainWindow
{
    Q_OBJECT
public:
    explicit QtWinFramelessWindowOld(QWidget *parent = 0);
    static QColor TitleBarCor();

    bool event(QEvent *event) override;
public:
    void setContentsMargins(const QMargins &margins);
    void setContentsMargins(int left, int top, int right, int bottom);
    QMargins contentsMargins() const;
    QRect contentsRect() const;
    void getContentsMargins(int *left, int *top, int *right, int *bottom) const;
signals:
    void DwmCompsitionChanged();
    void dpiChanged(const int xdpi, const int ydpi);
protected:
    void paintEvent(QPaintEvent *event) override;
    bool nativeEvent(const QByteArray &eventType, void *message, long *result) override;
    virtual bool isInprivate() const {return false;}

    const QMargins NormalMarginsWin10{1,1,1,1};
    const QMargins NormalMarginsWin7{1,2,1,1};

public slots:
    void showFullScreen();
private:
    int m_borderWidth;
    QMargins m_margins;
    QMargins m_frames;
    bool m_bJustMaximized;
    static QColor gTitleBarColor;

    void setResizeableAreaWidth(int width = 5);

    bool nativeEventWin7(const QByteArray &eventType, void *message, long *result);
    bool nativeEventWin8_OrNewer(const QByteArray &eventType, void *message, long *result);
};
#elif defined Q_OS_MAC
#include <QMouseEvent>
#include <QResizeEvent>
#include <QPoint>

//此类用于支持重载系统按钮的行为
//this Objective-c class is used to override the action of sysytem close button and zoom button
//https://stackoverflow.com/questions/27643659/setting-c-function-as-selector-for-nsbutton-produces-no-results
class OBJECTIVE_HEADFILE;
class CFramelessWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit CFramelessWindow(QWidget *parent = nullptr);
    ~CFramelessWindow() override ;
private:
    void initUI();
public:
    // 设置titleBar高度
    void setTitleBarHeight(float height);

    //In draggable area, window can be moved by mouse, (height = 0) means that the whole window is draggable
    void setDraggableAreaHeight(int height = 0);
    void enableDraggable(bool draggable);
    //只有OS X10.10及以后系统，才支持OS X原生样式包括：三个系统按钮、窗口圆角、窗口阴影
    //类初始化完成后，可以通过此函数查看是否已经启用了原生样式。如果未启动，需要自定义关闭按钮、最小化按钮、最大化按钮
    //Native style（three system button/ round corner/ drop shadow） works only on OS X 10.10 or later
    //after init, we should check whether NativeStyle is OK with this function
    //if NOT ok, we should implement close button/ min button/ max button ourself
    bool isNativeStyleOK() {return nativeSystemBtn_;}

    //如果设置setCloseBtnQuit(false)，那么点击关闭按钮后，程序不会退出，而是会隐藏,只有在OS X 10.10 及以后系统中有效
    //if setCloseBtnQuit(false), then when close button is clicked, the application will hide itself instead of quit
    //be carefull, after you set this to false, you can NOT change it to true again
    //this function should be called inside of the constructor function of derived classes, and can NOT be called more than once
    //only works for OS X 10.10 or later
    void setCloseBtnQuit(bool bQuit = true);

    //启用或禁用关闭按钮，只有在isNativeStyleOK()返回true的情况下才有效
    //enable or disable Close button, only worked if isNativeStyleOK() returns true
    void setCloseBtnEnabled(bool bEnable = true);

    //启用或禁用最小化按钮，只有在isNativeStyleOK()返回true的情况下才有效
    //enable or disable Miniaturize button, only worked if isNativeStyleOK() returns true
    void setMinBtnEnabled(bool bEnable = true);

    //启用或禁用zoom（最大化）按钮，只有在isNativeStyleOK()返回true的情况下才有效
    //enable or disable Zoom button(fullscreen button), only worked if isNativeStyleOK() returns true
    void setZoomBtnEnabled(bool bEnable = true);

    bool isCloseBtnEnabled() {return isCloseBtnEnabled_;}
    bool isMinBtnEnabled() {return isMinBtnEnabled_;}
    bool isZoomBtnEnabled() {return isZoomBtnEnabled_;}

private:
    int         draggableHeight_;
    bool        closeBtnQuit_;
    bool        nativeSystemBtn_;
    bool        isCloseBtnEnabled_;
    bool        isMinBtnEnabled_;
    bool        isZoomBtnEnabled_;

    //===============================================
    //TODO
    //下面的代码是试验性质的
    //tentative code

    //窗口从全屏状态恢复正常大小时，标题栏又会出现，原因未知。
    //默认情况下，系统的最大化按钮(zoom button)是进入全屏，为了避免标题栏重新出现的问题，
    //以上代码已经重新定义了系统zoom button的行为，是其功能变为最大化而不是全屏
    //以下代码尝试，每次窗口从全屏状态恢复正常大小时，都再次进行设置，以消除标题栏
    //问题在此版本中已经被修复 by scribbler on 2020.07.18
    //after the window restore from fullscreen mode, the titlebar will show again, it looks like a BUG
    //on OS X 10.10 and later, click the system green button (zoom button) will make the app become fullscreen
    //so we have override it's action to "maximized" in the CFramelessWindow Constructor function
    //but we may try something else such as delete the titlebar again and again...

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;

public:

private:
    OBJECTIVE_HEADFILE * OCHF;
    bool bPressFlag_;
    bool bCustomFlag_;
    QPoint beginDrag_;
};

#else
class CFramelessWindow : public QMainWindow
{
public:
    explicit CFramelessWindow(QWidget * parent = nullptr) : QMainWindow(parent){}
};
#endif

#endif // QT_WIN_FRAMELESS_OLD_H
