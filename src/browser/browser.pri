
#CEF_VERSION = cef_4240_chromium_86
CEF_VERSION = cef_4389_chromium_89
#CEF_VERSION = cef_4844_chromium_99

SOURCES += \
    $$PWD/cef_app_other.cpp \
    $$PWD/cef_app_render.cpp \
    $$PWD/client_app.cpp \
    $$PWD/client_switches.cc \
    $$PWD/message_loop/main_message_loop_externam_pump_qt.cc \
    $$PWD/scheme_handler.cpp \
    $$PWD/cef_app_browser.cpp \
    $$PWD/cef_client_handler.cpp \
    $$PWD/message_loop/main_message_loop.cc \
    $$PWD/message_loop/main_message_loop_external_pump.cc \
    $$PWD/message_loop/main_message_loop_std.cc \
    $$PWD/CefManager.cpp

HEADERS += \
    $$PWD/cef_app_other.h \
    $$PWD/cef_app_render.h \
    $$PWD/client_app.h \
    $$PWD/cef_app_browser.h \
    $$PWD/cef_client_handler.h \
    $$PWD/client_switches.h \
    $$PWD/scheme_handler.h \
    $$PWD/message_loop/main_message_loop.h \
    $$PWD/message_loop/main_message_loop_external_pump.h \
    $$PWD/message_loop/main_message_loop_std.h \
    $$PWD/CefManager.h

win32{
    SOURCES += \
    $$PWD/message_loop/main_message_loop_external_pump_win.cc \
    $$PWD/message_loop/main_message_loop_multithreaded_win.cc

    HEADERS += \
    $$PWD/message_loop/main_message_loop_multithreaded_win.h
}

unix:!macx{
    SOURCES += \
    $$PWD/message_loop/main_message_loop_external_pump_linux.cc \
    $$PWD/message_loop/main_message_loop_multithreaded_gtk.cc

    HEADERS += \
    $$PWD/message_loop/main_message_loop_multithreaded_gtk.h
}
