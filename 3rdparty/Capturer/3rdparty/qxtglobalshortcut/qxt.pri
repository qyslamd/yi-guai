CONFIG += c++11

DEFINES += BUILD_QXT_CORE

INCLUDEPATH += $$PWD/include

HEADERS += $$PWD/include/qxtglobal.h \
    $$PWD/include/qxtglobalshortcut_p.h \
    $$PWD/include/qxtglobalshortcut.h

SOURCES += $$PWD/src/qxtglobalshortcut.cpp

# Linux
unix:!macx {
        QT += x11extras
        CONFIG += X11
        SOURCES += $$PWD/src/qxtglobalshortcut_x11.cpp
}

# OS X
macx: {
        SOURCES += $$PWD/src/qxtglobalshortcut_mac.cpp
        QMAKE_LFLAGS += -framework Carbon -framework CoreFoundation
}

# Windows
win32: SOURCES += $$PWD/src/qxtglobalshortcut_win.cpp

win32-mingw: QMAKE_CXXFLAGS += -Wliteral-suffix
win32-msvc*: LIBS += -luser32

