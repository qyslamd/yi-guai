#include "QtWinFrameless.h"
#include <QApplication>
#include <QtDebug>
#include <QScreen>
#include <QWindow>

#include "utils/util_qt.h"

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

#if (QT_VERSION >= QT_VERSION_CHECK(5, 9, 0))
#include <QOperatingSystemVersion>
#else
#include <QSysInfo>
#endif

#define LOG_MACRO(x) #x

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

static void InvalidateNC(HWND handle)
{
    if(handle){
        SendMessage(handle, WM_NCPAINT, 0, 0);
    }
}

QtWinFramelessWindow::QtWinFramelessWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // QWindow的screenChanged信号的触发得是Frameless属性的窗口，我TMD的
#if 1
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
#endif
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
    case WM_NCMOUSELEAVE:
    {
        if (isWin7OrLower()) {
            *result = 0;
            return true;
        } else {
            return false;
        }
    }
    case WM_NCACTIVATE:
    {
        if (isWin7OrLower()) {
            *result = 1;
            InvalidateNC(msg->hwnd);
            return true;
        } else {
            return false;
        }
    }
    case WM_NCLBUTTONDOWN:
    {
        if (isWin7OrLower()) {
            InvalidateNC(msg->hwnd);
            *result = 0;
            return false;
        } else {
            return false;
        }
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
                int offset = UtilQt::isWindows10() ? 8 : 4;
                bcRect.left = bRect.left + offset;
                bcRect.top = bRect.top + 0;
                bcRect.right = bRect.right - offset;
                bcRect.bottom = bRect.bottom - offset;
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

void QtWinFramelessWindow::moveEvent(QMoveEvent *event)
{
    QMainWindow::moveEvent(event);
}

bool QtWinFramelessWindow::hitTestCaption(const QPoint &gPos)
{
    Q_UNUSED(gPos);
    return false;
}

#elif defined (Q_OS_LINUX1)
#include <QHBoxLayout>
#include <QHelpEvent>
#include <QToolTip>
#include <QPainter>
#include <QtMath>

QtWinFramelessWindow::QtWinFramelessWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowFlag(Qt::FramelessWindowHint);
    setMouseTracking(true);
    setMinimumSize(300,300);
    setContentsMargins(FrameWidth,FrameWidth,FrameWidth,FrameWidth);
}

bool QtWinFramelessWindow::event(QEvent *e)
{
    if (e->type() == QEvent::ToolTip)
    {
        QHelpEvent *helpEvent = static_cast<QHelpEvent *>(e);
        auto button = buttonAt(helpEvent->pos());
        if (button != CaptionButtons::Button_None) {
            QString str;
            switch (button) {
            case CaptionButtons::Button_Mini:
                str = tr("minimize");
                break;
            case CaptionButtons::Button_NormalMax:

                str = this->window()->isMaximized()
                        ? tr("restore")
                        : tr("maximize");
                break;
            case CaptionButtons::Button_Close:
                str = tr("close");
                break;
            default:
                Q_UNREACHABLE();
            }
            QToolTip::showText(helpEvent->globalPos(), str);
        } else {
            QToolTip::hideText();
            e->ignore();
        }

        return true;
    }
    return QMainWindow::event(e);
}

bool QtWinFramelessWindow::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
    return QMainWindow::nativeEvent(eventType, message, result);
}

void QtWinFramelessWindow::paintEvent(QPaintEvent *event)
{
    QMainWindow::paintEvent(event);
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);

    /*--------------------------------------------------
    *   draw caption background
    *--------------------------------------------------*/
    p.save();
    QRect captionRect{FrameWidth,
                FrameWidth,
                width() - 2 * FrameWidth,
                CaptionHeight};
    p.fillRect(captionRect, QColor(0xCECECE));
    p.restore();

    /*--------------------------------------------------
    *   draw client background
    *--------------------------------------------------*/
    p.save();
    QRect clientRect{FrameWidth,
                FrameWidth + CaptionHeight,
                width() - 2 * FrameWidth,
                height() - 2 * FrameWidth - CaptionHeight};
    p.fillRect(clientRect,
                     QBrush(Qt::white));
    p.restore();

    /*--------------------------------------------------
    *   draw window shadow
    *--------------------------------------------------*/
//    drawShadow(&p);

    /*--------------------------------------------------
    *   draw buttons
    *--------------------------------------------------*/
    drawButtons(&p);
}

void QtWinFramelessWindow::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        if(region_ != Region::HT_Client
                && region_ != Region::HT_MinButton
                && region_ != Region::HT_MaxButton
                && region_ != Region::HT_CloseButton)
        {
            mouse_left_btn_pressed_ = true;
            mouse_left_btn_press_pos_ = event->pos();
        }else if(region_ == Region::HT_MinButton)
        {
            min_button_press_ = true;
        }else if(region_ == Region::HT_MaxButton)
        {
            max_buttton_press_ = true;
        }else if(region_ == Region::HT_CloseButton){
            close_button_press_ = true;
        }
    }
}

void QtWinFramelessWindow::mouseMoveEvent(QMouseEvent *event)
{
    if(!mouse_left_btn_pressed_){
        judgeRegion(event->pos());
    }else{
        QPoint gloPoint = event->globalPos();
        QRect rect = this->rect();
        QPoint tl = mapToGlobal(rect.topLeft());
        QPoint rb = mapToGlobal(rect.bottomRight());

        QRect rMove(tl, rb);
        switch (region_) {
        case Region::HT_Left:
        {
            if(rb.x() - gloPoint.x() <= this->minimumWidth())
                rMove.setX(tl.x());
            else
                rMove.setX(gloPoint.x());
            setGeometry(rMove);
        }
            break;
        case Region::HT_TopLeft:
        {
            if(rb.x() - gloPoint.x() <= this->minimumWidth())
                rMove.setX(tl.x());
            else
                rMove.setX(gloPoint.x());
            if(rb.y() - gloPoint.y() <= this->minimumHeight())
                rMove.setY(tl.y());
            else
                rMove.setY(gloPoint.y());
            setGeometry(rMove);
        }
            break;
        case Region::HT_Top:
        {
            if(rb.y() - gloPoint.y() <= this->minimumHeight())
                rMove.setY(tl.y());
            else
                rMove.setY(gloPoint.y());
            setGeometry(rMove);
        }
            break;
        case Region::HT_Caption:
        {
            move(event->pos() - mouse_left_btn_press_pos_ + pos());
        }
            break;
        case Region::HT_TopRight:
        {
            rMove.setWidth(gloPoint.x() - tl.x());
            rMove.setY(gloPoint.y());
            setGeometry(rMove);
        }
            break;
        case Region::HT_Right:
        {
            rMove.setWidth(gloPoint.x() - tl.x());
            setGeometry(rMove);
        }
            break;
        case Region::HT_BottomRight:
        {
            rMove.setWidth(gloPoint.x() - tl.x());
            rMove.setHeight(gloPoint.y() - tl.y());
            setGeometry(rMove);
        }
            break;
        case Region::HT_Bottom:
        {
            rMove.setHeight(gloPoint.y() - tl.y());
            setGeometry(rMove);
        }
            break;
        case Region::HT_BottomLeft:
        {
            rMove.setX(gloPoint.x());
            rMove.setHeight(gloPoint.y() - tl.y());
            setGeometry(rMove);
        }
            break;
        default:
            break;
        }
    }
    QMainWindow::mouseMoveEvent(event);
}

void QtWinFramelessWindow::mouseReleaseEvent(QMouseEvent *event)
{
    QMainWindow::mouseReleaseEvent(event);
    if(event->button() == Qt::LeftButton)
    {
        mouse_left_btn_pressed_ = false;
        mouse_left_btn_press_pos_ = QPoint();

        if(region_ == Region::HT_MinButton)
        {
            if(min_button_press_)
            {
                showMinimized();
                min_button_press_ = false;
            }
        }else if(region_ == Region::HT_MaxButton)
        {
            if(max_buttton_press_){
                if(this->windowState() == Qt::WindowNoState){
                    showMaximized();
                }else{
                    if(isMaximized()){
                        showNormal();
                    }
                }
                max_buttton_press_ = false;
            }
        }else if(region_ == Region::HT_CloseButton){
            if(close_button_press_){
                close();
                close_button_press_ = false;
            }
        }
    }
}

void QtWinFramelessWindow::leaveEvent(QEvent *event)
{
    clearButtonHover();
    setCursor(Qt::ArrowCursor);
    QMainWindow::leaveEvent(event);
}

void QtWinFramelessWindow::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::WindowStateChange){
        auto windowState = this->windowState();
        if(windowState == Qt::WindowMaximized || windowState == Qt::WindowFullScreen){
            setFrameWidth(0);
        }else{
            setFrameWidth(10);
        }
    }
}

void QtWinFramelessWindow::setFrameWidth(int width)
{
    if(width < 0){
        FrameWidth = 0;
    }
    FrameWidth = width;

//    setContentsMargins(FrameWidth,
//                                FrameWidth + CaptionHeight,
//                                FrameWidth,
//                                FrameWidth);
}

void QtWinFramelessWindow::judgeRegion(const QPoint &windowPos)
{
    clearButtonHover();

    if(QRect{0, FrameWidth, FrameWidth, height() - 2 * FrameWidth}.contains(windowPos))
    {
        region_ = Region::HT_Left;
        setCursor(Qt::SizeHorCursor);
    }else if(QRect{0, 0, FrameWidth + 4, FrameWidth + 4}.contains(windowPos))
    {
        region_ = Region::HT_TopLeft;
        setCursor(Qt::SizeFDiagCursor);
    }else if(QRect{FrameWidth, 0, width() - 2 * FrameWidth, FrameWidth}.contains(windowPos))
    {
        region_ = Region::HT_Top;
        setCursor(Qt::SizeVerCursor);
    }
    else if(QRect{FrameWidth, FrameWidth, width() - 2 * FrameWidth, CaptionHeight}.contains(windowPos))
    {
        if(btnRect(CaptionButtons::Button_Mini).contains(windowPos)){
            min_button_hover_ = true;
            region_ = Region::HT_MinButton;
        }else if(btnRect(CaptionButtons::Button_NormalMax).contains(windowPos))
        {
            max_button_hover_ = true;
            region_ = Region::HT_MaxButton;
        }else if(btnRect(CaptionButtons::Button_Close).contains(windowPos))
        {
            close_button_hover_ = true;
            region_ = Region::HT_CloseButton;
        }else{
            region_ = Region::HT_Caption;
        }
        update();
        setCursor(Qt::ArrowCursor);
    }
    else if(QRect{width() - FrameWidth, 0, FrameWidth, FrameWidth}.contains(windowPos))
    {
        region_ = Region::HT_TopRight;
        setCursor(Qt::SizeBDiagCursor);
    }
    else if(QRect{width() - FrameWidth, FrameWidth, FrameWidth, height() - 2 * FrameWidth}.contains(windowPos))
    {
        region_ = Region::HT_Right;
        setCursor(Qt::SizeHorCursor);
    }
    else if(QRect{width() - FrameWidth, height() - FrameWidth, FrameWidth, FrameWidth}.contains(windowPos))
    {
        region_ = Region::HT_BottomRight;
        setCursor(Qt::SizeFDiagCursor);
    }
    else if(QRect{FrameWidth, height() - FrameWidth, width() - 2 * FrameWidth, FrameWidth}.contains(windowPos))
    {
        region_ = Region::HT_Bottom;
        setCursor(Qt::SizeVerCursor);
    }
    else if(QRect{0, height() - FrameWidth, FrameWidth, FrameWidth}.contains(windowPos))
    {
        region_ = Region::HT_BottomLeft;
        setCursor(Qt::SizeBDiagCursor);
    }
    else
    {
        region_ = Region::HT_Client;
        setCursor(Qt::ArrowCursor);
    }
}

QRectF QtWinFramelessWindow::btnRect(QtWinFramelessWindow::CaptionButtons button)
{
    int BtnWidth = 45;
    int BtnHeight = 28;
    int BtnSpacing = 1;

//    int dpi = this->window()->logicalDpiX();
//    switch (dpi) {
//    case 96:
//        BtnWidth = 45;
//        BtnHeight = 30;
//        break;
//    case 120:
//        BtnWidth = 56;
//        BtnHeight = 40;
//        break;
//    case 144:
//        BtnWidth = 68;
//        BtnHeight = 42;
//        break;
//    case 168:
//        BtnWidth = 78;
//        BtnHeight = 52;
//        break;
//    default:
//        BtnWidth = 45;
//        BtnHeight = 28;
//        break;
//    }

    QRect captionRect{FrameWidth, FrameWidth, width() - 2 * FrameWidth, CaptionHeight};
    int y = captionRect.y();
    QRectF rectClose(captionRect.x() + captionRect.width() - BtnWidth,
                     y,
                     BtnWidth,
                     BtnHeight);
    QRectF rectMax(rectClose.x() - BtnSpacing - BtnWidth,
                   y,
                   BtnWidth,
                   BtnHeight);
    QRectF rectMini(rectMax.x() - BtnSpacing - BtnWidth,
                    y,
                    BtnWidth,
                    BtnHeight);

    switch (button) {
    case CaptionButtons::Button_Close:
        return rectClose;
        case CaptionButtons::Button_NormalMax:
        return rectMax;
    case CaptionButtons::Button_Mini:
        return rectMini;
    default:
        break;
    }
    return QRect();

}

QPixmap QtWinFramelessWindow::btnPixmap(QtWinFramelessWindow::CaptionButtons button)
{
    QColor color(Qt::black);
    QSize size(10, 10);
    QPixmap pix(size);
    pix.fill(Qt::transparent);
    QSizeF sizeF(size.width() *1.0,  size.height() * 1.0);
    QPainter p(&pix);
    switch (button) {
    case CaptionButtons::Button_Mini:
    {
        p.save();
        QPen pen(QBrush(color), 1.0);
        p.setPen(pen);

        p.drawLine(QPointF(0, sizeF.height() / 2),
                   QPointF(sizeF.width(), sizeF.height() / 2));

        p.restore();
    }
        break;
    case CaptionButtons::Button_NormalMax:
    {
        // max
        p.save();
        QPen pen(QBrush(color), 1.0);
        p.setPen(pen);

        p.drawRect(QRectF(0.0, 0.0, sizeF.width() - 1.0, sizeF.height() - 1.0));

        p.restore();
    }
        break;
    case CaptionButtons::Button_Close:
    {
        p.save();
        p.setRenderHint(QPainter::Antialiasing);
        QPen pen(QBrush(color), 1.0);
        p.setPen(pen);

        QRectF rect(0,0,size.width(), size.height());
        p.drawLine(QPointF(rect.x(), rect.y()),
                   QPointF(rect.x() + rect.width(), rect.y() + rect.height())
                   );
        p.drawLine(QPointF(rect.x() + rect.width(), rect.y()),
                   QPointF(rect.x(), rect.y() + rect.height())
                   );

        p.restore();
    }
        break;
    default:
        break;
    }
    return pix;
}

QtWinFramelessWindow::CaptionButtons QtWinFramelessWindow::buttonAt(const QPoint &pos)
{
    if(btnRect(CaptionButtons::Button_Mini).contains(pos)){
        return CaptionButtons::Button_Mini;
    }else if(btnRect(CaptionButtons::Button_NormalMax).contains(pos)){
        return CaptionButtons::Button_NormalMax;
    }else if(btnRect(CaptionButtons::Button_Close).contains(pos)){
        return CaptionButtons::Button_Close;
    }
    return CaptionButtons::Button_None;
}

void QtWinFramelessWindow::drawShadow(QPainter *p)
{
    if(FrameWidth == 0){
        return;
    }
    p->save();
    QColor color(100, 100, 100, 30);
    for (int i = 0; i < FrameWidth; i++)
    {
        color.setAlpha(static_cast<int>(120 - qSqrt(i) * 40));
        p->setPen(color);
        p->drawRoundedRect(FrameWidth - i,
                                FrameWidth - i,
                                width() - (FrameWidth - i) * 2,
                                height() - (FrameWidth - i) * 2,
                                i,
                                i);
    }
    p->restore();
}

void QtWinFramelessWindow::drawButtons(QPainter *p)
{
    p->save();
    QRectF rectMini = btnRect(CaptionButtons::Button_Mini);
    QRectF rectMax = btnRect(CaptionButtons::Button_NormalMax);
    QRectF rectClose = btnRect(CaptionButtons::Button_Close);
    if(min_button_hover_){
        QColor color(0xB9B9B9);
        color.setAlphaF(0.7);
        p->fillRect(rectMini, color);
    }
    if(max_button_hover_){
        QColor color(0xB9B9B9);
        color.setAlphaF(0.7);
        p->fillRect(rectMax, color);
    }
    if(close_button_hover_){
        p->fillRect(rectClose, QColor(0xE81123));
    }
    QPixmap pix = btnPixmap(CaptionButtons::Button_Mini);
    p->drawPixmap(QPointF(rectMini.x() + (rectMini.width() - pix.width()) / 2,
                         rectMini.y() + (rectMini.height() - pix.height()) / 2),
                 pix);
    pix = btnPixmap(CaptionButtons::Button_NormalMax);
    p->drawPixmap(QPointF(rectMax.x() + (rectMax.width() - pix.width()) / 2,
                         rectMax.y() + (rectMax.height() - pix.height()) / 2),
                 pix);
    pix = btnPixmap(CaptionButtons::Button_Close);
    p->drawPixmap(QPointF(rectClose.x() + (rectClose.width() - pix.width()) / 2,
                         rectClose.y() + (rectClose.height() - pix.height()) / 2),
                 pix);

    p->restore();
}

void QtWinFramelessWindow::clearButtonHover()
{
    min_button_hover_ = false;
    max_button_hover_ = false;
    close_button_hover_ = false;
    update();
}

#endif // Q_OS_WIN
