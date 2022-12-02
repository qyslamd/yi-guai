#include "QtWinFramelessOld.h"

#ifdef Q_OS_WIN

#include <QApplication>
#include <QPoint>
#include <QSize>
#include <QSysInfo>
#include <QtDebug>
#include <QTimer>
#include <QtWin>
#include <QPainter>
#include <windows.h>
#include <WinUser.h>
#include <windowsx.h>
#include <dwmapi.h>
#include <objidl.h> // Fixes error C2504: 'IUnknown' : base class undefined
#include <algorithm>
#include <QSysInfo>
#include <QWindow>
#include "utils/util_qt.h"

#if (QT_VERSION >= QT_VERSION_CHECK(5, 9, 0))
#include <QOperatingSystemVersion>
#else
#include <QSysInfo>
#endif

namespace Gdiplus
{
using std::min;
using std::max;
};
#include <Gdiplus.h>
#include <GdiPlusColor.h>
#pragma comment( lib, "gdiplus.lib" )
#pragma comment (lib,"Dwmapi.lib") // Adds missing library, fixes error LNK2019: unresolved external symbol __imp__DwmExtendFrameIntoClientArea
#pragma comment (lib,"user32.lib")

#ifndef WM_NCUAHDRAWCAPTION
// Not documented, only available since Windows Vista
#define WM_NCUAHDRAWCAPTION (0x00AE)
#endif

#ifndef WM_NCUAHDRAWFRAME
// Not documented, only available since Windows Vista
#define WM_NCUAHDRAWFRAME (0x00AF)
#endif

// The standard values of resize border width, resize border height and title bar height when DPI is 96.
static const int g_defaultResizeBorderWidth = 8,
g_defaultResizeBorderHeight = 8,
g_defaultTitleBarHeight = 23;

namespace Constants
{
    const char framelessMode_flag[] = "_FRAMELESSHELPER_FRAMELESS_MODE";
    const char resizeBorderWidth_flag[] = "_FRAMELESSHELPER_RESIZE_BORDER_WIDTH";
    const char resizeBorderHeight_flag[] = "_FRAMELESSHELPER_RESIZE_BORDER_HEIGHT";
    const char titleBarHeight_flag[] = "_FRAMELESSHELPER_TITLE_BAR_HEIGHT";
    const char hitTestVisibleInChrome_flag[] = "_FRAMELESSHELPER_HIT_TEST_VISIBLE_IN_CHROME";
    const char useNativeTitleBar_flag[] = "_FRAMELESSHELPER_USE_NATIVE_TITLE_BAR";
    const char preserveNativeFrame_flag[] = "_FRAMELESSHELPER_PRESERVE_NATIVE_WINDOW_FRAME";
    const char forcePreserveNativeFrame_flag[] = "_FRAMELESSHELPER_FORCE_PRESERVE_NATIVE_WINDOW_FRAME";
    const char windowFixedSize_flag[] = "_FRAMELESSHELPER_WINDOW_FIXED_SIZE";

}

enum class SystemMetric
{
    ResizeBorderWidth,
    ResizeBorderHeight,
    TitleBarHeight
};

static int getSystemMetric(const QWindow *window,
                           const SystemMetric metric,
                           const bool dpiScale,
                           const bool forceSystemValue = false)
{
    Q_ASSERT(window);
    if (!window) {
        return 0;
    }
    switch (metric) {
    case SystemMetric::ResizeBorderWidth: {
        const int rbw = window->property(Constants::resizeBorderWidth_flag).toInt();
        if ((rbw > 0) && !forceSystemValue) {
            return qRound(static_cast<qreal>(rbw) * (dpiScale ? window->devicePixelRatio() : 1.0));
        } else {
            const int result = GetSystemMetrics(SM_CXSIZEFRAME) + GetSystemMetrics(SM_CXPADDEDBORDER);
            if (result > 0) {
                if (dpiScale) {
                    return result;
                } else {
                    return qRound(static_cast<qreal>(result) / window->devicePixelRatio());
                }
            } else {
                if (dpiScale) {
                    return qRound(static_cast<qreal>(g_defaultResizeBorderWidth) * window->devicePixelRatio());
                } else {
                    return g_defaultResizeBorderWidth;
                }
            }
        }
    }
    case SystemMetric::ResizeBorderHeight: {
        const int rbh = window->property(Constants::resizeBorderHeight_flag).toInt();
        if ((rbh > 0) && !forceSystemValue) {
            return qRound(static_cast<qreal>(rbh) * (dpiScale ? window->devicePixelRatio() : 1.0));
        } else {
            // There is no "SM_CYPADDEDBORDER".
            const int result = GetSystemMetrics(SM_CYSIZEFRAME) + GetSystemMetrics(SM_CXPADDEDBORDER);
            if (result > 0) {
                if (dpiScale) {
                    return result;
                } else {
                    return qRound(static_cast<qreal>(result) / window->devicePixelRatio());
                }
            } else {
                if (dpiScale) {
                    return qRound(static_cast<qreal>(g_defaultResizeBorderHeight) * window->devicePixelRatio());
                } else {
                    return g_defaultResizeBorderHeight;
                }
            }
        }
    }
    case SystemMetric::TitleBarHeight: {
        const int tbh = window->property(Constants::titleBarHeight_flag).toInt();
        if ((tbh > 0) && !forceSystemValue) {
            return qRound(static_cast<qreal>(tbh) * (dpiScale ? window->devicePixelRatio() : 1.0));
        } else {
            const int result = GetSystemMetrics(SM_CYCAPTION);
            if (result > 0) {
                if (dpiScale) {
                    return result;
                } else {
                    return qRound(static_cast<qreal>(result) / window->devicePixelRatio());
                }
            } else {
                if (dpiScale) {
                    return qRound(static_cast<qreal>(g_defaultTitleBarHeight) * window->devicePixelRatio());
                } else {
                    return g_defaultTitleBarHeight;
                }
            }
        }
    }
    }
    return 0;
}

static bool isWin8OrGreater()
{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 9, 0))
    return QOperatingSystemVersion::current() >= QOperatingSystemVersion::Windows8;
#else
    return QSysInfo::WindowsVersion >= QSysInfo::WV_WINDOWS8;
#endif
}

static bool isWin7OrLower()
{
    return !isWin8OrGreater();
}

static bool isWin8Point1OrGreater()
{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 9, 0))
    return QOperatingSystemVersion::current() >= QOperatingSystemVersion::Windows8_1;
#else
    return QSysInfo::WindowsVersion >= QSysInfo::WV_WINDOWS8_1;
#endif
}

static bool isDwmCompositionAvailable()
{
    // DWM Composition is always enabled and can't be disabled since Windows 8.
    if (isWin8OrGreater()) {
        return true;
    }
    BOOL enabled = FALSE;
    if (FAILED(DwmIsCompositionEnabled(&enabled))) {
        qWarning() << "DwmIsCompositionEnabled() failed.";
        return false;
    }
    return (enabled != FALSE);
}

static void InvalidateNC(HWND handle)
{
    if(handle){
        SendMessage(handle, WM_NCPAINT, 0, 0);
    }
}

QColor QtWinFramelessWindowOld::gTitleBarColor;
QtWinFramelessWindowOld::QtWinFramelessWindowOld(QWidget *parent)
    : QMainWindow(parent),
      m_borderWidth(5),
      m_bJustMaximized(false)
{
    setWindowFlags(Qt::Window |
                   Qt::FramelessWindowHint |
                   Qt::WindowTitleHint |
                   Qt::WindowSystemMenuHint |
                   Qt::WindowMinMaxButtonsHint |
                   Qt::WindowCloseButtonHint |
                   Qt::WindowFullscreenButtonHint);
    HWND hwnd = (HWND)this->winId();
    DWORD style = ::GetWindowLong(hwnd, GWL_STYLE);
    ::SetWindowLong(hwnd, GWL_STYLE, style | WS_MAXIMIZEBOX | WS_THICKFRAME | WS_CAPTION);
    if(isDwmCompositionAvailable()){
        const MARGINS shadow = { 1, 1, 1, 1 };
        DwmExtendFrameIntoClientArea(HWND(winId()), &shadow);
        qInfo()<<"\033[34m[Dwm:]"<<"dwm composition available,then extended frame in to client area"<<"\033[0m";
    }else{
        qInfo()<<__FUNCTION__<<" dwm composition unavailable!";
    }
}

QColor QtWinFramelessWindowOld::TitleBarCor()
{
    return gTitleBarColor;
}

bool QtWinFramelessWindowOld::event(QEvent *event)
{
    if(event->type() == QEvent::WindowStateChange){
        if(isWin7OrLower()){
            SendMessage((HWND)this->winId(), WM_NCPAINT, 0, 0);
        }
    }
    return QMainWindow::event(event);
}

void QtWinFramelessWindowOld::setResizeableAreaWidth(int width)
{
    if (1 > width) width = 1;
    m_borderWidth = width;
}

bool QtWinFramelessWindowOld::nativeEventWin7(const QByteArray &eventType, void *message, long *result)
{
    // Workaround for known bug -> check Qt forum :
    // https://forum.qt.io/topic/93141/qtablewidget-itemselectionchanged/13
#if (QT_VERSION == QT_VERSION_CHECK(5, 11, 1))
    MSG* msg = *reinterpret_cast<MSG**>(message);
#else
    MSG* msg = reinterpret_cast<MSG*>(message);
#endif

    switch (msg->message)
    {
    case WM_DWMCOLORIZATIONCOLORCHANGED:
    {
        // 只是获取颜色改变通知，不处理，返回false
        emit DwmCompsitionChanged();
        return false;
    }
    case WM_DPICHANGED:
    {
        auto xdpi = LOWORD(msg->wParam);
        auto ydpi = HIWORD(msg->wParam);
        emit dpiChanged(xdpi, ydpi);
        return false;
    }
    case WM_NCMOUSELEAVE:
        *result = 0;
        return true;

        /*发现有2个问题：
     *   1、点击右上角的系统按钮区域会出现系统按钮
     *   2、当切换程序的时候窗体会恢复默认样式。
     *需要处理WM_NCACTIVATE 和 WM_NCLBUTTONDOWN 这两个消息，解决上面2个问题。*/
    case WM_NCACTIVATE:
    {
        *result = 1;
        InvalidateNC(msg->hwnd);
        return true;
    }
    case WM_NCLBUTTONDOWN:
    {
        InvalidateNC(msg->hwnd);
        *result = 0;
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

            // 填充改变后客户区大小
            RECT bcRect;
            bcRect.left = bRect.left;
            bcRect.top = bRect.top;
            bcRect.right = bRect.right;
            bcRect.bottom = bRect.bottom;

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
        // 先调用默认处理，看现在的位置上是什么返回值
        const LRESULT originalRet = DefWindowProcW(msg->hwnd, msg->message, msg->wParam, msg->lParam);
        // 如果是在系统按钮上，让返回值为客户区,后边用Qt的button模拟
        if(originalRet == HTCLOSE ||
                originalRet == HTMAXBUTTON ||
                originalRet == HTMINBUTTON ||
                originalRet == HTHELP){
            InvalidateNC(msg->hwnd);
            *result = HTCLIENT;
            return true;
        }

        const LONG border_width = m_borderWidth;
        RECT winrect;
        GetWindowRect(HWND(winId()), &winrect);

        long x = GET_X_LPARAM(msg->lParam);
        long y = GET_Y_LPARAM(msg->lParam);

        bool canResizeWidth = minimumWidth() != maximumWidth();
        bool canResizeHeight = minimumHeight() != maximumHeight();

        if(canResizeWidth)
        {
            *result = HTCLIENT;

            //left border
            if (x >= winrect.left && x < winrect.left + border_width
                    && y > winrect.top + border_width
                    && y < winrect.bottom - border_width)
            {
                *result = HTLEFT;
                return true;
            }
            //right border
            if (x < winrect.right && x >= winrect.right - border_width
                    && y > winrect.top + border_width
                    && y < winrect.bottom - border_width)
            {
                *result = HTRIGHT;
                return true;
            }
        }
        if(canResizeHeight)
        {
            //bottom border
            if (y < winrect.bottom && y >= winrect.bottom - border_width
                    && x > winrect.left + border_width
                    && x < winrect.right - border_width)
            {
                *result = HTBOTTOM;
                return true;
            }
            //top border
            if (y >= winrect.top && y < winrect.top + border_width
                    && x > winrect.left + border_width
                    && x < winrect.right - border_width)
            {
                *result = HTTOP;
                return true;
            }
        }
        if(canResizeWidth && canResizeHeight)
        {
            //bottom left corner
            if (x >= winrect.left && x < winrect.left + border_width &&
                    y < winrect.bottom && y >= winrect.bottom - border_width)
            {
                *result = HTBOTTOMLEFT;
                return true;
            }
            //bottom right corner
            if (x < winrect.right && x >= winrect.right - border_width &&
                    y < winrect.bottom && y >= winrect.bottom - border_width)
            {
                *result = HTBOTTOMRIGHT;
                return true;
            }
            //top left corner
            if (x >= winrect.left && x < winrect.left + border_width &&
                    y >= winrect.top && y < winrect.top + border_width)
            {
                *result = HTTOPLEFT;
                return true;
            }

            //top right corner
            if (x < winrect.right && x >= winrect.right - border_width &&
                    y >= winrect.top && y < winrect.top + border_width)
            {
                *result = HTTOPRIGHT;
                return true;
            }
        }
        return false;
    } //end case WM_NCHITTEST

    case WM_GETMINMAXINFO:
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
    case WM_NCUAHDRAWCAPTION:
    case WM_NCUAHDRAWFRAME:
    {
        *result = 0;
        return true;
    }
    case WM_NCPAINT:
    {
        *result = 0;
        return true;
    }
    default:
        break;
    }
    return QMainWindow::nativeEvent(eventType, message, result);
}

bool QtWinFramelessWindowOld::nativeEventWin8_OrNewer(const QByteArray &eventType, void *message, long *result)
{
    // Workaround for known bug -> check Qt forum :
    // https://forum.qt.io/topic/93141/qtablewidget-itemselectionchanged/13
#if (QT_VERSION == QT_VERSION_CHECK(5, 11, 1))
    MSG* msg = *reinterpret_cast<MSG**>(message);
#else
    MSG* msg = reinterpret_cast<MSG*>(message);
#endif

    switch (msg->message)
    {
    case WM_DWMCOLORIZATIONCOLORCHANGED:
    {
        // 只是获取颜色改变通知，不处理，返回false
        emit DwmCompsitionChanged();
        return false;
    }
    case WM_DPICHANGED:
    {
        auto xdpi = LOWORD(msg->wParam);
        auto ydpi = HIWORD(msg->wParam);
        emit dpiChanged(xdpi, ydpi);
        return false;
    }
    case WM_NCMOUSELEAVE:
        *result = 0;
        return true;
    case WM_NCCALCSIZE:
    {
        if (!msg->wParam) {
            *result = 0;
            return true;
        }

        bool nonClientAreaExists = false;
        NCCALCSIZE_PARAMS* pncsp = reinterpret_cast<NCCALCSIZE_PARAMS*>(msg->lParam);
        const auto clientRect = &(pncsp->rgrc[0]);
        auto window = windowHandle();
        if (IsMaximized(msg->hwnd) && (window->windowState() != Qt::WindowFullScreen)) {
            // When a window is maximized, its size is actually a little bit more
            // than the monitor's work area. The window is positioned and sized in
            // such a way that the resize handles are outside of the monitor and
            // then the window is clipped to the monitor so that the resize handle
            // do not appear because you don't need them (because you can't resize
            // a window when it's maximized unless you restore it).
            const int rbh = getSystemMetric(window, SystemMetric::ResizeBorderHeight, true);
            //            clientRect->top += rbh;
            clientRect->bottom -= rbh;
            const int rbw = getSystemMetric(window, SystemMetric::ResizeBorderWidth, true);
            //            clientRect->left += rbw;
            clientRect->right -= rbw;
            nonClientAreaExists = true;
        }else{
            if (pncsp->rgrc[0].top != 0){
                pncsp->rgrc[0].bottom += 1;
                //                static bool flag = false;
                //                if(flag){
                //                    pncsp->rgrc[0].bottom -= 1;
                //                }else{
                //                    pncsp->rgrc[0].bottom += 1;
                //                }
                //                flag = !flag;
            }
        }


        *result = nonClientAreaExists ? 0 : WVR_REDRAW;
        return true;

    }
    case WM_NCHITTEST:
    {
        const LONG border_width = m_borderWidth;
        RECT winrect;
        GetWindowRect(HWND(winId()), &winrect);

        long x = GET_X_LPARAM(msg->lParam);
        long y = GET_Y_LPARAM(msg->lParam);

        bool canResizeWidth = minimumWidth() != maximumWidth();
        bool canResizeHeight = minimumHeight() != maximumHeight();

        if(canResizeWidth)
        {
            *result = HTCLIENT;

            //left border
            if (x >= winrect.left && x < winrect.left + border_width
                    && y > winrect.top + border_width
                    && y < winrect.bottom - border_width)
            {
                *result = HTLEFT;
                return true;
            }
            //right border
            if (x < winrect.right && x >= winrect.right - border_width
                    && y > winrect.top + border_width
                    && y < winrect.bottom - border_width)
            {
                *result = HTRIGHT;
                return true;
            }
        }
        if(canResizeHeight)
        {
            //bottom border
            if (y < winrect.bottom && y >= winrect.bottom - border_width
                    && x > winrect.left + border_width
                    && x < winrect.right - border_width)
            {
                *result = HTBOTTOM;
                return true;
            }
            //top border
            if (y >= winrect.top && y < winrect.top + border_width
                    && x > winrect.left + border_width
                    && x < winrect.right - border_width)
            {
                *result = HTTOP;
                return true;
            }
        }
        if(canResizeWidth && canResizeHeight)
        {
            //bottom left corner
            if (x >= winrect.left && x < winrect.left + border_width &&
                    y < winrect.bottom && y >= winrect.bottom - border_width)
            {
                *result = HTBOTTOMLEFT;
                return true;
            }
            //bottom right corner
            if (x < winrect.right && x >= winrect.right - border_width &&
                    y < winrect.bottom && y >= winrect.bottom - border_width)
            {
                *result = HTBOTTOMRIGHT;
                return true;
            }
            //top left corner
            if (x >= winrect.left && x < winrect.left + border_width &&
                    y >= winrect.top && y < winrect.top + border_width)
            {
                *result = HTTOPLEFT;
                return true;
            }

            //top right corner
            if (x < winrect.right && x >= winrect.right - border_width &&
                    y >= winrect.top && y < winrect.top + border_width)
            {
                *result = HTTOPRIGHT;
                return true;
            }
        }
        return false;
    } //end case WM_NCHITTEST

    case WM_GETMINMAXINFO:
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
        //    case WM_NCUAHDRAWCAPTION:
        //    case WM_NCUAHDRAWFRAME:
        //    {
        //        if(QSysInfo::WV_WINDOWS7 == QSysInfo::windowsVersion())
        //        {
        //            *result = 0;
        //            return true;
        //        }
        //    }
        //    case WM_NCPAINT:
        //    {
        //        if(QSysInfo::WV_WINDOWS7 == QSysInfo::windowsVersion())
        //        {
        //            *result = 0;
        //            return true;
        //        }else{
        //            break;
        //        }
        //    }
    default:
        break;
    }
    return QMainWindow::nativeEvent(eventType, message, result);
}

bool QtWinFramelessWindowOld::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
    if(isWin8OrGreater()){
        return nativeEventWin8_OrNewer(eventType, message, result);
    }else{
        return nativeEventWin7(eventType, message, result);
    }
}

void QtWinFramelessWindowOld::setContentsMargins(const QMargins &margins)
{
    QMainWindow::setContentsMargins(margins+m_frames);
    m_margins = margins;
}
void QtWinFramelessWindowOld::setContentsMargins(int left, int top, int right, int bottom)
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
QMargins QtWinFramelessWindowOld::contentsMargins() const
{
    QMargins margins = QMainWindow::contentsMargins();
    margins -= m_frames;
    return margins;
}
void QtWinFramelessWindowOld::getContentsMargins(int *left, int *top, int *right, int *bottom) const
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
QRect QtWinFramelessWindowOld::contentsRect() const
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
void QtWinFramelessWindowOld::showFullScreen()
{
    if (isMaximized())
    {
        QMainWindow::setContentsMargins(m_margins);
        m_frames = QMargins();
    }
    QMainWindow::showFullScreen();
}

void QtWinFramelessWindowOld::paintEvent(QPaintEvent *event)
{
    QMainWindow::paintEvent(event);

    QColor color(0xCECECE), borderColor(Qt::darkGray);
    if(!isInprivate()){
        if(UtilQt::dwmColorPrevalence())
        {
            color = QtWin::realColorizationColor();
            borderColor =  color;
        }
    }
    gTitleBarColor = color;
    QPainter p(this);
    if(QSysInfo::windowsVersion() == QSysInfo::WV_WINDOWS7){
        p.save();
        QPainterPath path;
        path.addRect(QRect(0, 0, NormalMarginsWin7.left(), height()));
        path.addRect(QRect(0, 0, width(), NormalMarginsWin7.top()));
        path.addRect(QRect(width() - NormalMarginsWin7.right(), 0, NormalMarginsWin7.right(), height()));
        path.addRect(QRect(0, height()- NormalMarginsWin7.bottom(), width(), NormalMarginsWin7.bottom()));
        p.fillPath(path, color);
        p.restore();
    }

    // 绘制边框
    p.save();
    QPen pen(QBrush(borderColor), 1);
    p.setPen(pen);
    p.drawRect(this->rect().marginsAdded(QMargins(0,0,-1,-1)));
    p.restore();
}
#endif //Q_OS_WIN

