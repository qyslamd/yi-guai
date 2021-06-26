TEMPLATE = app
CONFIG += c++11

SOURCES += \
    main.cpp \
    $$PWD/../src/browser/cef_app_other.cpp \
    $$PWD/../src/browser/cef_app_render.cpp \
    $$PWD/../src/browser/client_app.cpp

HEADERS += \
    $$PWD/../src/browser/cef_app_other.h\
    $$PWD/../src/browser/cef_app_render.h \
    $$PWD/../src/browser/client_app.h

DESTDIR = $$OUT_PWD/../src/bin
unix:!macx{
    CONFIG += link_pkgconfig
    PKGCONFIG += x11
    PKGCONFIG += glib-2.0
    PKGCONFIG += gmodule-2.0 gtk+-2.0 gthread-2.0 gtk+-unix-print-2.0 gtkglext-1.0

    CEF_DEP_PATH = $$PWD/../../cef_binary_86.0.21+g6a2c8e7+chromium-86.0.4240.183_linux64

#    QMAKE_LFLAGS += -Wl,-rpath,\'$$OUT_PWD/../src/bin\'

    INCLUDEPATH += $$CEF_DEP_PATH
    CONFIG(debug, debug|release){
        LIBS += -L$$CEF_DEP_PATH/Debug -lcef
        LIBS += -L$$CEF_DEP_PATH/DebugBuild/libcef_dll_wrapper -lcef_dll_wrapper

    }else:CONFIG(release, debug|release){
        LIBS += -L$$CEF_DEP_PATH/Release -lcef
        LIBS += -L$$CEF_DEP_PATH/ReleaseBuild/libcef_dll_wrapper -lcef_dll_wrapper
    }
}

