QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    browser_window.cpp \
    cef_app_browser.cpp \
    cef_client_handler.cpp \
    cef_qwidget.cpp \
    main.cpp \
    message_loop/main_message_loop.cc \
    message_loop/main_message_loop_external_pump.cc \
    message_loop/main_message_loop_external_pump_win.cc \
    message_loop/main_message_loop_multithreaded_win.cc \
    message_loop/main_message_loop_std.cc \
    mainwindow.cpp \
    util_win.cc


HEADERS += \
    browser_window.h \
    cef_app_browser.h \
    cef_client_handler.h \
    cef_qwidget.h \
    client_types.h \
    message_loop/main_message_loop.h \
    message_loop/main_message_loop_external_pump.h \
    message_loop/main_message_loop_multithreaded_win.h \
    message_loop/main_message_loop_std.h \
    mainwindow.h \
    util_win.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


win32{
    equals(QT_ARCH,i386){
        CEF_DEP_PATH = $$PWD/../cef_depends/cef_4240_chromium_86/x86
    }else{
        CEF_DEP_PATH = $$PWD/../cef_depends/cef_4240_chromium_86/x64
    }

    MANI_PATH = $$CEF_DEP_PATH/manifests

    INCLUDEPATH += $$CEF_DEP_PATH
    LIBS += user32.lib \
            Gdi32.lib

    CONFIG(debug, debug|release){
        DESTDIR = $$CEF_DEP_PATH/bin/Debug

        LIBS += -L$$CEF_DEP_PATH/bin/Debug -llibcef
        LIBS += -L$$CEF_DEP_PATH/libcef_dll_wrapper/MDd -llibcef_dll_wrapper

        QMAKE_POST_LINK += mt -nologo -manifest $$MANI_PATH/cefclient.exe.manifest $$MANI_PATH/compatibility.manifest -outputresource:$$CEF_DEP_PATH/bin/Debug/$$TARGET".exe" $$escape_expand(\n\t)

    }else:CONFIG(release, debug|release){
        DESTDIR = $$CEF_DEP_PATH/bin/Release

        LIBS += -L$$CEF_DEP_PATH/bin/Release -llibcef
        LIBS += -L$$CEF_DEP_PATH/libcef_dll_wrapper/MD -llibcef_dll_wrapper
    }
}
