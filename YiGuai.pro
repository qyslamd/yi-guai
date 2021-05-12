QT       += core gui network widgets

CONFIG += c++11

DESTDIR = $$OUT_PWD/bin
TARGET = YiGuai

win32{
    VERSION = 4.0.2.666
    RC_ICONS = icon_64px.ico
    # 公司名称
    QMAKE_TARGET_COMPANY = "zhouyohu@1663.com"
    # 产品名称
    QMAKE_TARGET_PRODUCT = "YiGuai Web Browser"
    # 文件说明
#    QMAKE_TARGET_DESCRIPTION = "YiGuai browser based on Qt 5.14.2 (MSVC 2019, 32 bit) and CEF (chromium-86.0.4240.183)"
    # 版权信息
    QMAKE_TARGET_COPYRIGHT = "Copyright 2008-2016 The Qt Company Ltd. All rights reserved."
    # 中文（简体）
    RC_LANG = 0x0004
}

SOURCES += \
    managers/AppCfgManager.cpp \
    managers/CefSettingsManager.cpp \
    managers/FaviconManager.cpp \
    managers/MainWindowManager.cpp \
    page.cpp \
    popup.cpp \
    popups/HistoryPopup.cpp \
    popups/PopupBase.cpp \
    popups/SiteInfoPopup.cpp \
    toolbars/AddressBar.cpp \
    toolbars/BookmarkBar.cpp \
    toolbars/NavigateBar.cpp \
    toolbars/PagesTabBar.cpp \
    toolbars/TabbarStyle.cpp \
    widgets/Dp_Widget_Caption.cpp \
    widgets/FramelessWindow.cpp \
    dialogs/alertdialog.cpp \
    browser/browser_window.cpp \
    browser/cef_app_browser.cpp \
    browser/cef_client_handler.cpp \
    cef_qwidget.cpp \
    main.cpp \
    browser/message_loop/main_message_loop.cc \
    browser/message_loop/main_message_loop_external_pump.cc \
    browser/message_loop/main_message_loop_external_pump_win.cc \
    browser/message_loop/main_message_loop_multithreaded_win.cc \
    browser/message_loop/main_message_loop_std.cc \
    mainwindow.cpp \
    utils/util_qt.cpp \
    utils/util_win.cc \
    widgets/TabThumbnailWidget.cpp \
    widgets/framewidget.cpp


HEADERS += \
    globaldef.h \
    managers/AppCfgManager.h \
    managers/CefSettingsManager.h \
    managers/FaviconManager.h \
    managers/MainWindowManager.h \
    page.h \
    popup.h \
    popups/HistoryPopup.h \
    popups/PopupBase.h \
    popups/SiteInfoPopup.h \
    toolbars/AddressBar.h \
    toolbars/BookmarkBar.h \
    toolbars/NavigateBar.h \
    toolbars/PagesTabBar.h \
    toolbars/TabbarStyle.h \
    widgets/Dp_Widget_Caption.h \
    widgets/FramelessWindow.h \
    dialogs/alertdialog.h \
    browser/browser_window.h \
    browser/cef_app_browser.h \
    browser/cef_client_handler.h \
    cef_qwidget.h \
    browser/client_types.h \
    browser/message_loop/main_message_loop.h \
    browser/message_loop/main_message_loop_external_pump.h \
    browser/message_loop/main_message_loop_multithreaded_win.h \
    browser/message_loop/main_message_loop_std.h \
    mainwindow.h \
    utils/util_qt.h \
    utils/util_win.h \
    widgets/TabThumbnailWidget.h \
    widgets/framewidget.h

FORMS += \
    dialogs/alertdialog.ui \
    popup.ui \
    popups/HistoryPopup.ui \
    popups/PopupBase.ui \
    widgets/TabThumbnailWidget.ui \
    widgets/framewidget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

msvc {
    QMAKE_CFLAGS += /utf-8
    QMAKE_CXXFLAGS += /utf-8
}
win32{
    QT += winextras

    equals(QT_ARCH,i386){
        CEF_DEP_PATH = $$PWD/../cef_depends/cef_4240_chromium_86/x86
    }else{
        CEF_DEP_PATH = $$PWD/../cef_depends/cef_4240_chromium_86/x64
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

RESOURCES += \
    resource.qrc

TRANSLATIONS +=$$PWD/resources/i18n/$$TARGET"_zh.ts"
