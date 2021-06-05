#include "QtWinFrameless.h"
#include <QApplication>
#include <QtDebug>

#ifdef Q_OS_WIN
#include <windows.h>
#include <WinUser.h>
#include <windowsx.h>
#include <dwmapi.h>
#include <objidl.h> // Fixes error C2504: 'IUnknown' : base class undefined
#include <gdiplus.h>
#include <GdiPlusColor.h>
#pragma comment (lib,"Dwmapi.lib") // Adds missing library, fixes error LNK2019: unresolved external symbol __imp__DwmExtendFrameIntoClientArea
#pragma comment (lib,"user32.lib")

QtWinFramelessWindow::QtWinFramelessWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_bJustMaximized(false)
{
    // 好像不用这么复杂吧？？？
#if 0
    // 首先设置无边框和携带系统菜单的属性
    setWindowFlags(windowFlags() | Qt::Window | Qt::FramelessWindowHint | Qt::WindowSystemMenuHint);

    bool visible = isVisible();
    setWindowFlags(windowFlags() | Qt::WindowMaximizeButtonHint);

    // this line will get titlebar/thick frame/Aero back, which is exactly what we want
    // we will get rid of titlebar and thick frame again in nativeEvent() later
    // 此行代码可以带回Aero效果，同时也带回了标题栏和边框,在nativeEvent()会再次去掉标题栏
    HWND hwnd = (HWND)this->winId();
    DWORD style = ::GetWindowLong(hwnd, GWL_STYLE);
    ::SetWindowLong(hwnd, GWL_STYLE, style | WS_MAXIMIZEBOX | WS_THICKFRAME | WS_CAPTION);
    setVisible(visible);
#endif
}

bool QtWinFramelessWindow::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
    //Workaround for known bug -> check Qt forum : https://forum.qt.io/topic/93141/qtablewidget-itemselectionchanged/13
    #if (QT_VERSION == QT_VERSION_CHECK(5, 11, 1))
    MSG* msg = *reinterpret_cast<MSG**>(message);
    #else
    MSG* msg = reinterpret_cast<MSG*>(message);
    #endif
    
    switch (msg->message)
    {
    case WM_DWMCOLORIZATIONCOLORCHANGED:
    {
        // 发出颜色改变通知
        emit DwmCompsitionChanged();
        // 不处理，返回false
        return false;
    }
    case WM_DPICHANGED:
    {
        // 发出DPI改变通知
        emit dpiChanged(HIWORD(msg->wParam));
        // 不处理，返回false
        return false;
    }
    case WM_NCCALCSIZE:
    {
        *result = 0;

        /*
         * WM_NCCALCSIZE 消息在当必须计算窗口客户区的大小和位置时发送。
         * 通过处理这个消息，当窗口的大小或位置发生变化的时，应用程序可以控制窗口
         * 客户区的内容
         * wParam
         *      如果为TRUE，则它指定应用程序应该指示客户区哪一部分包含有效信息。
         *      系统将有效信息复制到新客户区域内的指定区域。此时的lParam指向NCCALCSIZE_PARAMS
         *      结构。
         *
         *      如果为FALSE，应用程序不需要指定客户区的有效区域
         *
         * typedef struct tagNCCALCSIZE_PARAMS {
         *     RECT       rgrc[3];
         *     PWINDOWPOS lppos;
         *   } NCCALCSIZE_PARAMS, *LPNCCALCSIZE_PARAMS;
         *
         *   此结构包含了三个矩形，当 WM_NCCALCSIZE 消息触发的时候：
         *   rgrc[0]包含了被移动或改变大小后的窗口坐标。即建议的窗口新坐标
         *   rgrc[1]包含了移动或改变大小之前的窗口坐标
         *   rgrc[2]包含了窗口移动或调整大小之前窗口客户区的坐标（如果窗口是子窗口，坐标相对于父窗口客户区，如果是顶层窗口，坐标相对于屏幕原点）
         *
         *   如果你处理了 WM_NCCALCSIZE，那么
         *    rgrc[0]代表包含由移动或调整大小产生的新客户区矩形的坐标。
         *    rgrc[1]包含了有效的目标矩形
         *    rgrc[2]包含了有效的源矩形
         *    最后两个矩形与 WM_NCCALCSIZE 消息的返回值结合使用，以确定要保留的窗口区域。
         *
         * 所以处理流程是：
         *      wParam 为 TRUE
         *      获取 NCCALCSIZE_PARAMS
         *      按照规则重新设置NACCALCSIZE_PARAMS
         *      返回
         *
    */
        if(msg->wParam){
            NCCALCSIZE_PARAMS* pncsp = reinterpret_cast<NCCALCSIZE_PARAMS*>(msg->lParam);

            RECT aRect;  // 获取改变前窗口 RECT
            CopyRect(&aRect, &pncsp->rgrc[1]);

            RECT bRect;  // 获取改变后窗口 RECT
            CopyRect(&bRect, &pncsp->rgrc[0]);

            // 改变后客户区大小
            RECT bcRect;
            if(this->isFullScreen() || this->isMaximized()){
                bcRect.left = bRect.left;
                bcRect.top = bRect.top;
                bcRect.right = bRect.right;
                bcRect.bottom = bRect.bottom;
            }else{
                bcRect.left = bRect.left + 8;
                bcRect.top = bRect.top;
                bcRect.right = bRect.right - 8;
                bcRect.bottom = bRect.bottom - 8;
            }

            CopyRect(&pncsp->rgrc[0], &bcRect); // 设置改变后客户区大小
            CopyRect(&pncsp->rgrc[1], &bRect);  // 设置改变后窗口大小
            CopyRect(&pncsp->rgrc[2], &aRect);  // 设置改变后源矩形大小

            /*
             * WM_NCCALCSIZE message return value
             * WVR_ALIGNTOP
             * WVR_ALIGNRIGHT
             * WVR_ALIGNLEFT
             * WVR_ALIGNBOTTOM
             * WVR_HREDRAW
             * WVR_VREDRAW
             * WVR_REDRAW
             * WVR_VALIDRECTS
            */
            *result = WVR_REDRAW;
        }
    }
        return true;
    case WM_NCHITTEST:
    {
        const int offset = 4;
        RECT clientRect;
        GetWindowRect(HWND(winId()), &clientRect);
        long x = GET_X_LPARAM(msg->lParam);
        long y = GET_Y_LPARAM(msg->lParam);

        // 左上
        if(x >= clientRect.left && x < clientRect.left + offset
                && y >= clientRect.top && y < clientRect.top + offset)
        {
            *result = HTTOPLEFT;
             return true;
        }
        //上
        if (x >= clientRect.left + offset && x <= clientRect.right - offset
                && y >= clientRect.top && y < clientRect.top + offset)
        {
            *result = HTTOP;
            return true;
        }
        //右上
        if(x > clientRect.right - offset && x <= clientRect.right
                && y >= clientRect.top && y < clientRect.top + offset)
        {
            *result = HTTOPRIGHT;
            return true;
        }

        if(hitTestCaption(QPoint(x, y))){
            *result = HTCAPTION;
            return true;
        }
        // 其它位置默认处理
        return false;
    }
    case WM_NCRBUTTONDOWN:
    {
#define LOG_MACRO(x) #x
        qInfo()<<__FUNCTION__<<LOG_MACRO(WM_NCRBUTTONDOWN);
        auto xPos = GET_X_LPARAM(msg->lParam);
        auto yPos = GET_Y_LPARAM(msg->lParam);
        HMENU sysMenu = ::GetSystemMenu((HWND)winId(), FALSE);
        ::TrackPopupMenu(sysMenu,0, xPos, yPos, NULL, (HWND)winId(), NULL);
    }
        break;
    case WM_GETMINMAXINFO:
        return false;
    {
        if (::IsZoomed(msg->hwnd)) {
            RECT frame = { 0, 0, 0, 0 };
            AdjustWindowRectEx(&frame, WS_OVERLAPPEDWINDOW, FALSE, 0);

            //record frame area data
            double dpr = this->devicePixelRatioF();

            m_frames.setLeft(abs(frame.left)/dpr+0.5);
            m_frames.setTop(abs(frame.bottom)/dpr+0.5);
            m_frames.setRight(abs(frame.right)/dpr+0.5);
            m_frames.setBottom(abs(frame.bottom)/dpr+0.5);

            QMainWindow::setContentsMargins(m_frames.left()+m_margins.left(),
                                            m_frames.top()+m_margins.top(),
                                            m_frames.right()+m_margins.right(),
                                            m_frames.bottom()+m_margins.bottom());
            m_bJustMaximized = true;
        }else {
            if (m_bJustMaximized)
            {
                QMainWindow::setContentsMargins(m_margins);
                m_frames = QMargins();
                m_bJustMaximized = false;
            }
        }
        return false;
    }
    default:
        break;
    }
    return QMainWindow::nativeEvent(eventType, message, result);
}

void QtWinFramelessWindow::showEvent(QShowEvent *event)
{
    QMainWindow::showEvent(event);
}

void QtWinFramelessWindow::setContentsMargins(const QMargins &margins)
{
    QMainWindow::setContentsMargins(margins+m_frames);
    m_margins = margins;
}
void QtWinFramelessWindow::setContentsMargins(int left, int top, int right, int bottom)
{
    QMainWindow::setContentsMargins(left+m_frames.left(),\
                                    top+m_frames.top(), \
                                    right+m_frames.right(), \
                                    bottom+m_frames.bottom());
    m_margins.setLeft(left);
    m_margins.setTop(top);
    m_margins.setRight(right);
    m_margins.setBottom(bottom);
}
QMargins QtWinFramelessWindow::contentsMargins() const
{
    QMargins margins = QMainWindow::contentsMargins();
    margins -= m_frames;
    return margins;
}
void QtWinFramelessWindow::getContentsMargins(int *left, int *top, int *right, int *bottom) const
{
    QMainWindow::getContentsMargins(left,top,right,bottom);
    if (!(left&&top&&right&&bottom)) return;
    if (isMaximized())
    {
        *left -= m_frames.left();
        *top -= m_frames.top();
        *right -= m_frames.right();
        *bottom -= m_frames.bottom();
    }
}
QRect QtWinFramelessWindow::contentsRect() const
{
    QRect rect = QMainWindow::contentsRect();
    int width = rect.width();
    int height = rect.height();
    rect.setLeft(rect.left() - m_frames.left());
    rect.setTop(rect.top() - m_frames.top());
    rect.setWidth(width);
    rect.setHeight(height);
    return rect;
}
void QtWinFramelessWindow::showFullScreen()
{
    if (isMaximized())
    {
        QMainWindow::setContentsMargins(m_margins);
        m_frames = QMargins();
    }
    QMainWindow::showFullScreen();
}

#endif //Q_OS_WIN
