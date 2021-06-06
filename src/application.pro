QT       += core gui network widgets

CONFIG += c++11

DESTDIR = $$OUT_PWD/bin
TARGET = YiGuai

win32{
    QT += winextras
    VERSION = 0.0.1.12

    RC_ICONS = icon_64px.ico
    # 公司名称
    QMAKE_TARGET_COMPANY = "zhouyohu@163.com"
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
    browser/client_switches.cc \
    browser/scheme_handler.cpp \
    managers/AddrInputManager.cpp \
    managers/AppCfgManager.cpp \
    managers/CefManager.cpp \
    managers/EventManager.cpp \
    managers/FaviconManager.cpp \
    managers/MainWindowManager.cpp \
    page.cpp \
    popup.cpp \
    popups/AddToFavoritePopup.cpp \
    popups/InprivatePopup.cpp \
    popups/PopupBase.cpp \
    popups/PopupGeneral.cpp \
    popups/SiteInfoPopup.cpp \
    popups/UserInfoPopup.cpp \
    popups/ZoomPopup.cpp \
    test/test.cpp \
    toolbars/AddressBar.cpp \
    toolbars/BookmarkBar.cpp \
    toolbars/NavigateBar.cpp \
    toolbars/NotificationBar.cpp \
    toolbars/TabBar.cpp \
    toolbars/TabPagesBar.cpp \
    toolbars/TabbarStyle.cpp \
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
    widgets/AppConfigWidget.cpp \
    widgets/BookmarkWidget.cpp \
    widgets/DownloadWidget.cpp \
    widgets/FramelessWidget.cpp \
    widgets/HistoryWidget.cpp \
    widgets/FullscnHint.cpp \
    widgets/QtWinFrameless.cpp \
    widgets/TabThumbnailWidget.cpp


HEADERS += \
    browser/client_switches.h \
    browser/scheme_handler.h \
    globaldef.h \
    managers/AddrInputManager.h \
    managers/AppCfgManager.h \
    managers/CefManager.h \
    managers/EventManager.h \
    managers/FaviconManager.h \
    managers/MainWindowManager.h \
    page.h \
    popup.h \
    popups/AddToFavoritePopup.h \
    popups/InprivatePopup.h \
    popups/PopupBase.h \
    popups/PopupGeneral.h \
    popups/SiteInfoPopup.h \
    popups/UserInfoPopup.h \
    popups/ZoomPopup.h \
    test/test.h \
    toolbars/AddressBar.h \
    toolbars/BookmarkBar.h \
    toolbars/NavigateBar.h \
    toolbars/NotificationBar.h \
    toolbars/TabBar.h \
    toolbars/TabPagesBar.h \
    toolbars/TabbarStyle.h \
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
    widgets/BookmarkWidget.h \
    widgets/DownloadWidget.h \
    widgets/HistoryWidget.h \
    widgets/FullscnHint.h \
    widgets/QtWinFrameless.h \
    widgets/TabThumbnailWidget.h \
    widgets/AppConfigWidget.h \
    widgets/framelesswidget.h

FORMS += \
    dialogs/alertdialog.ui \
    popup.ui \
    popups/AddToFavoritePopup.ui \
    popups/InprivatePopup.ui \
    popups/PopupBase.ui \
    popups/SiteInfoPopup.ui \
    popups/UserInfoPopup.ui \
    popups/ZoomPopup.ui \
    test/test.ui \
    toolbars/NotificationBar.ui \
    widgets/BookmarkWidget.ui \
    widgets/DownloadWidget.ui \
    widgets/HistoryWidget.ui \
    widgets/FullscnHint.ui \
    widgets/TabThumbnailWidget.ui \
    widgets/AppConfigWidget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

msvc {
    QMAKE_CFLAGS += /utf-8
    QMAKE_CXXFLAGS += /utf-8
}
win32{
    equals(QT_ARCH,i386){
        CEF_DEP_PATH = $$PWD/../../cef_depends/cef_4240_chromium_86/x86
    }else{
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

RESOURCES += \
    resource.qrc

TRANSLATIONS +=$$PWD/resources/i18n/$$TARGET"_zh.ts"