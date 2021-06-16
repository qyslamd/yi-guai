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


#define LOG_MACRO(x) #x

QtWinFramelessWindow::QtWinFramelessWindow(QWidget *parent)
    : QMainWindow(parent)
{
}

bool QtWinFramelessWindow::event(QEvent *ev)
{
    if(ev->type() == QEvent::WindowStateChange){
        if(this->isMaximized()){
            maximized_ = true;
        }
    }
    return QMainWindow::event(ev);
}

bool QtWinFramelessWindow::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
    //Workaround for known bug -> check Qt forum : https://forum.qt.io/topic/93141/qtablewidget-itemselectionchanged/13
    #if (QT_VERSION == QT_VERSION_CHECK(5, 11, 1))
    MSG* msg = *reinterpret_cast<MSG**>(message);
    #else
    MSG* msg = reinterpret_cast<MSG*>(message);
    #endif
//    qInfo()<<__FUNCTION__<<QString::number(msg->message,16);

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
            if(this->isFullScreen()){
                bcRect.left = bRect.left;
                bcRect.top = bRect.top;
                bcRect.right = bRect.right;
                bcRect.bottom = bRect.bottom;
            }else{
                bcRect.left = bRect.left + 8;
                bcRect.top = bRect.top + 0;
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
        qInfo()<<__FUNCTION__<<LOG_MACRO(WM_NCRBUTTONDOWN);
        auto xPos = GET_X_LPARAM(msg->lParam);
        auto yPos = GET_Y_LPARAM(msg->lParam);
        HMENU sysMenu = ::GetSystemMenu((HWND)winId(), FALSE);
        ::TrackPopupMenu(sysMenu,0, xPos, yPos, NULL, (HWND)winId(), NULL);
    }
        break;
    case WM_GETMINMAXINFO:
    {
    }
        return false;
    default:
        break;
    }
    return QMainWindow::nativeEvent(eventType, message, result);
}

bool QtWinFramelessWindow::hitTestCaption(const QPoint &gPos)
{
    Q_UNUSED(gPos);
    return false;
}

#endif //Q_OS_WIN
