#ifndef UTIL_GTK_H
#define UTIL_GTK_H

#if defined(__linux__) || defined(__linux)

#include <include/base/cef_macros.h>
#include <include/base/cef_platform_thread.h>

class ScopedGdkThreadsEnter {
 public:
  ScopedGdkThreadsEnter();
  ~ScopedGdkThreadsEnter();

 private:
  bool take_lock_;

  static base::PlatformThreadId locked_thread_;

  DISALLOW_COPY_AND_ASSIGN(ScopedGdkThreadsEnter);
};
#endif

#endif // UTIL_GTK_H
