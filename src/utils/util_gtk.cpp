#include "util_gtk.h"

#if defined(__linux__) || defined(__linux)
#include <include/base/cef_logging.h>
#include <gdk/gdk.h>

base::PlatformThreadId ScopedGdkThreadsEnter::locked_thread_ =
    kInvalidPlatformThreadId;

ScopedGdkThreadsEnter::ScopedGdkThreadsEnter() {
  // The GDK lock is not reentrant, so don't try to lock again if the current
  // thread already holds it.
  base::PlatformThreadId current_thread = base::PlatformThread::CurrentId();
  take_lock_ = current_thread != locked_thread_;

  if (take_lock_) {
    gdk_threads_enter();
    locked_thread_ = current_thread;
  }
}

ScopedGdkThreadsEnter::~ScopedGdkThreadsEnter() {
  if (take_lock_) {
    locked_thread_ = kInvalidPlatformThreadId;
    gdk_threads_leave();
  }
}


//Display* X11GetDisplay(QWidget* widget)
//{
//  Q_ASSERT_X(widget, "X11GetDisplay", "Invalid parameter widget");
//  if (!widget) {
//    qWarning("Invalid parameter widget");
//    return nullptr;
//  }

//  auto platformInterface = QApplication::platformNativeInterface();
//  Q_ASSERT_X(platformInterface, "X11GetDisplay", "Failed to get platform native interface");
//  if (!platformInterface) {
//    qWarning("Failed to get platform native interface");
//    return nullptr;
//  }

//  auto screen = widget->window()->windowHandle()->screen();
//  Q_ASSERT_X(screen, "X11GetDisplay", "Failed to get screen");
//  if (!screen) {
//    qWarning("Failed to get screen");
//    return nullptr;
//  }

////  return (Display*)platformInterface->nativeResourceForScreen("display", screen);
//  return (Display*)screen->handle();
//  return nullptr;
//}

//void RemapWindow(QWidget* widget, QWindow* window)
//{
//  if (::XMapWindow(X11GetDisplay(widget), window->winId()) <= 0)
//    qWarning() << __FUNCTION__ << "Failed to move input focus";
//}

#endif
