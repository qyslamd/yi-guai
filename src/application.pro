QT       += core gui network widgets sql

CONFIG += c++11

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
    main.cpp \
    mainwindow.cpp \
    utils/util_qt.cpp \
    utils/util_win.cc \
    widgets/AppConfigWidget.cpp \
    widgets/BookmarkWidget.cpp \
    widgets/DownloadWidget.cpp \
    widgets/QtFramelessWnd.cpp \
    widgets/HistoryWidget.cpp \
    widgets/FullScnHint.cpp \
    widgets/QtWinFrameless.cpp \
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
    widgets/TabThumbnailWidget.h \
    widgets/AppConfigWidget.h \
    widgets/QtFramelessWnd.h \
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
    OBJECTS_DIR = .obj
    MOC_DIR = .moc

    CONFIG += link_pkgconfig
    PKGCONFIG += x11
    PKGCONFIG += glib-2.0
    PKGCONFIG += gmodule-2.0 gtk+-2.0 gthread-2.0 gtk+-unix-print-2.0 gtkglext-1.0

    CEF_DEP_PATH = $$PWD/../../cef_binary_86.0.21+g6a2c8e7+chromium-86.0.4240.183_linux64
    OUTPUT_BIN_DIR = $$DESTDIR

#    QMAKE_LFLAGS += -Wl,-rpath,\'$$OUT_PWD/bin\'

    INCLUDEPATH += $$CEF_DEP_PATH
    CONFIG(debug, debug|release){
        LIBS += -L$$CEF_DEP_PATH/Debug -lcef
        LIBS += -L$$CEF_DEP_PATH/DebugBuild/libcef_dll_wrapper -lcef_dll_wrapper

#        QMAKE_POST_LINK += \
#        cp -R $$CEF_DEP_PATH/Debug/* $$OUTPUT_BIN_DIR

    }else:CONFIG(release, debug|release){
        LIBS += -L$$CEF_DEP_PATH/Release -lcef
        LIBS += -L$$CEF_DEP_PATH/ReleaseBuild/libcef_dll_wrapper -lcef_dll_wrapper

#        QMAKE_POST_LINK += \
#        cp -R $$CEF_DEP_PATH/Release/* $$OUTPUT_BIN_DIR
    }
}

RESOURCES += \
    resource.qrc

TRANSLATIONS +=$$PWD/resources/i18n/$$TARGET"_zh.ts"

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../3rdparty/qt-material-widgets/components/release/ -lcomponents
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../3rdparty/qt-material-widgets/components/debug/ -lcomponents
else:unix:!macx: LIBS += -L$$OUT_PWD/../3rdparty/qt-material-widgets/components/ -lcomponents

INCLUDEPATH += $$PWD/../3rdparty/qt-material-widgets/components
DEPENDPATH += $$PWD/../3rdparty/qt-material-widgets/components

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../3rdparty/qt-material-widgets/components/release/libcomponents.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../3rdparty/qt-material-widgets/components/debug/libcomponents.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../3rdparty/qt-material-widgets/components/release/components.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../3rdparty/qt-material-widgets/components/debug/components.lib
else:unix:!macx: PRE_TARGETDEPS += $$OUT_PWD/../3rdparty/qt-material-widgets/components/libcomponents.a
