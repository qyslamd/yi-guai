QT       += core gui network widgets sql

#CONFIG += c++14

TARGET = YiGuai

include($$PWD/browser/browser.pri)

win32{
    QT += winextras
    VERSION = 0.0.1.12

    RC_ICONS = corgi.ico
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
    browser_page.cpp \
    dialogs/aboutdialog.cpp \
    dialogs/pagedialog.cpp \
    main_linux.cpp \
    main_win.cpp \
    managers/AppCfgManager.cpp \
    managers/BookmarkManager.cpp \
    managers/DisplayMgr.cpp \
    managers/FaviconManager.cpp \
    managers/HistoryManager.cpp \
    managers/MainWindowManager.cpp \
    popup_browser.cpp \
    popups/AddBookmarkPopup.cpp \
    popups/InprivatePopup.cpp \
    popups/PopupBase.cpp \
    popups/PopupGeneral.cpp \
    popups/SiteInfoPopup.cpp \
    popups/StyledMenu.cpp \
    popups/UserInfoPopup.cpp \
    popups/ZoomPopup.cpp \
    toolbars/AddressBar.cpp \
    toolbars/BookmarkBar.cpp \
    toolbars/FindBar.cpp \
    toolbars/NavigationBar.cpp \
    toolbars/NotificationBar.cpp \
    toolbars/SearchBar.cpp \
    toolbars/TabBar.cpp \
    toolbars/TabPageBar.cpp \
    toolbars/TabbarStyle.cpp \
    dialogs/alertdialog.cpp \
    cef_qwidget.cpp \
    mainwindow.cpp \
    utils/util_qt.cpp \
    utils/util_win.cc \
    widgets/AppConfigWidget.cpp \
    widgets/BookmarkWidget.cpp \
    widgets/DownloadWidget.cpp \
    widgets/HistoryWidget.cpp \
    widgets/FullScnHint.cpp \
    widgets/QtWinFrameless.cpp \
    widgets/QtWinFramelessOld.cpp \
    widgets/TabThumbnailWidget.cpp


HEADERS += \
    browser_page.h \
    dialogs/aboutdialog.h \
    dialogs/pagedialog.h \
    globaldef.h \
    managers/AppCfgManager.h \
    managers/BookmarkManager.h \
    managers/DisplayMgr.h \
    managers/FaviconManager.h \
    managers/HistoryManager.h \
    managers/MainWindowManager.h \
    popup_browser.h \
    popups/AddBookmarkPopup.h \
    popups/InprivatePopup.h \
    popups/PopupBase.h \
    popups/PopupGeneral.h \
    popups/SiteInfoPopup.h \
    popups/StyledMenu.h \
    popups/UserInfoPopup.h \
    popups/ZoomPopup.h \
    toolbars/AddressBar.h \
    toolbars/BookmarkBar.h \
    toolbars/FindBar.h \
    toolbars/NavigationBar.h \
    toolbars/NotificationBar.h \
    toolbars/SearchBar.h \
    toolbars/TabBar.h \
    toolbars/TabPageBar.h \
    toolbars/TabbarStyle.h \
    dialogs/alertdialog.h \
    cef_qwidget.h \
    mainwindow.h \
    utils/util_qt.h \
    widgets/BookmarkWidget.h \
    widgets/DownloadWidget.h \
    widgets/HistoryWidget.h \
    widgets/FullscnHint.h \
    widgets/QtWinFrameless.h \
    widgets/QtWinFramelessOld.h \
    widgets/TabThumbnailWidget.h \
    widgets/AppConfigWidget.h \
    utils/windowskeyboardcodes.h

FORMS += \
    dialogs/aboutdialog.ui \
    dialogs/alertdialog.ui \
    dialogs/pagedialog.ui \
    popup_browser.ui \
    popups/AddBookmarkPopup.ui \
    popups/InprivatePopup.ui \
    popups/PopupBase.ui \
    popups/SiteInfoPopup.ui \
    popups/UserInfoPopup.ui \
    popups/ZoomPopup.ui \
    toolbars/FindBar.ui \
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
    DESTDIR = $$OUT_PWD/bin

    HEADERS += \
        utils/util_win.h

    equals(QT_ARCH,i386){
        CEF_DEP_PATH = $$PWD/../../cef_depends/$$CEF_VERSION/x86
    }

    equals(QT_ARCH,x86_64){
        CEF_DEP_PATH = $$PWD/../../cef_depends/$$CEF_VERSION/x64
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

unix:!macx{
    HEADERS += \
            utils/util_gtk.h

    SOURCES += \
    utils/util_gtk.cpp

    CONFIG += no_keywords

    DESTDIR = $$OUT_PWD/bin

    OBJECTS_DIR = .obj
    MOC_DIR = .moc

    CONFIG += link_pkgconfig
    PKGCONFIG += x11
    PKGCONFIG += glib-2.0
    PKGCONFIG += gmodule-2.0 gtk+-3.0 gthread-2.0 gtk+-unix-print-3.0 xi

    CEF_DEP_PATH = $$PWD/../../../files/cef_binary_99.2.15+g71e9523+chromium-99.0.4844.84_linux64
    OUTPUT_BIN_DIR = $$DESTDIR

    QMAKE_LFLAGS += -Wl,-rpath,\'$$OUT_PWD/bin\'

    INCLUDEPATH += $$CEF_DEP_PATH
    CONFIG(debug, debug|release){
        message("SB debug")
        LIBS += -L$$CEF_DEP_PATH/Debug -lcef
        LIBS += -L$$CEF_DEP_PATH/build-debug/libcef_dll_wrapper -lcef_dll_wrapper
    }else:CONFIG(release, debug|release){
        DEFINES += NDEBUG
        LIBS += $$CEF_DEP_PATH/Release/libcef.so
        LIBS += $$CEF_DEP_PATH/build-release/libcef_dll_wrapper/libcef_dll_wrapper.a
    }
}

RESOURCES += \
    resource.qrc

TRANSLATIONS +=$$PWD/resources/i18n/$$TARGET"_zh.ts"
