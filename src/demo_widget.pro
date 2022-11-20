QT       += core gui network widgets sql

CONFIG += c++11

TARGET = DemoWidget
include($$PWD/browser/browser.pri)

msvc {
    QMAKE_CFLAGS += /utf-8
    QMAKE_CXXFLAGS += /utf-8
}
win32{
    QT  += winextras
    DESTDIR = $$OUT_PWD/bin

    HEADERS += \
        utils/util_win.h

    equals(QT_ARCH,i386){
        CEF_DEP_PATH = $$PWD/../../cef_depends/cef_4240_chromium_86/x86
    }

    equals(QT_ARCH,x86_64){
        CEF_DEP_PATH = $$PWD/../../cef_depends/cef_4240_chromium_86/x64
    }

    MANI_PATH = $$CEF_DEP_PATH/manifests
    WIN32_CEF_DEP_PATH      = $$replace(CEF_DEP_PATH, /, \\)
    WIN32_PWD               = $$replace(PWD,/,\\)
    WIN32_OUT_PWD           = $$replace(OUT_PWD, /, \\)
    WIN32_BIN_DIR           = $$WIN32_OUT_PWD\\bin

    INCLUDEPATH += $$CEF_DEP_PATH
    LIBS += user32.lib \
            Gdi32.lib

    CONFIG(debug, debug|release){
        LIBS += -L$$CEF_DEP_PATH/bin/Debug -llibcef
        LIBS += -L$$CEF_DEP_PATH/libcef_dll_wrapper/MDd -llibcef_dll_wrapper

        QMAKE_POST_LINK += \
        mt -nologo -manifest $$MANI_PATH/cefclient.exe.manifest $$MANI_PATH/compatibility.manifest -outputresource:$$OUT_PWD/bin/$$TARGET".exe" $$escape_expand(\n\t)
        QMAKE_POST_LINK += \
        xcopy $$WIN32_CEF_DEP_PATH\\bin\\Debug $$WIN32_BIN_DIR /y /e /h &&\
        xcopy $$WIN32_CEF_DEP_PATH\\Resources  $$WIN32_BIN_DIR /y /e /h

    }else:CONFIG(release, debug|release){
        LIBS += -L$$CEF_DEP_PATH/bin/Release -llibcef
        LIBS += -L$$CEF_DEP_PATH/libcef_dll_wrapper/MD -llibcef_dll_wrapper

        QMAKE_POST_LINK +=\
        xcopy $$WIN32_CEF_DEP_PATH\\bin\\Release $$WIN32_BIN_DIR /y /e /h &&\
        xcopy $$WIN32_CEF_DEP_PATH\\Resources  $$WIN32_BIN_DIR /y /e /h
    }
}

HEADERS += \
    demo_cef_qwidget.h \
    utils/util_qt.h \
    utils/windowskeyboardcodes.h

SOURCES += \
    demo_cef_qwidget.cpp \
    demo_main.cpp \
    utils/util_qt.cpp \
    utils/util_win.cc
