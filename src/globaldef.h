#ifndef GLOBALDEF_H
#define GLOBALDEF_H

#include <QMetaType>
#include <QVector>
#include <QtDebug>
#include <include/internal/cef_types.h>
///
/// \brief The TabBarCmd enum
///
enum class TabBarCmd{
    None = -1,
    NewTabPage,
    Reload,
    Mute,
    CloseTab,
    CloseRight,
    CloseOther,
};

///
/// \brief The NaviBarCmd enum
///
enum class NaviBarCmd {
    None = -1,
    Navigate,
    Back,
    HomePage,
    Forward,
    Refresh,
    StopLoading,
    ViewSiteInfo,
    AddressEdited,
    AddFavorite,
    ShowZoomBar,
    Favorite,
    History,
    Download,
    User,
    NewTabPage,
    NewWindow,
    NewInprivateWindow,
    ZoomOut,
    ZoomIn,
    FullScreen,
    Print,
    Capture,
    Inprivate,
    Find,
    TaskMgr,
    DevTool,
    Settings,
    About,
    Feedback,
    Like,
    AboutQt,
    AboutCef,
    QuitApp
};

///
/// \brief The ToolWndType enum
/// 工具栏popup窗口类型
enum class ToolWndType
{
    None,
    AddFavorite,
    Favorite,
    History,
    Download,
    UserInfo,
};

///
/// \brief The BookmarkCmd enum
///
enum class BookmarkCmd{
    None,
    Open,
    OpenInNewPage,
    OpenInNewWnd,
    OpenInInprivate,
    Changed,
    BkmkBarVisibleChanged,
};

///
/// \brief The HistoryCmd enum
///
enum class HistoryCmd{
    None,
    Open,
    OpenInNewPage,
    OpenInNewWnd,
    OpenInInprivate,
    Changed,
    Add2Favorite,
};

///
/// \brief The PageCmd enum
///
enum class PageCmd{
    None = -1,
    NeedSize,
    Created,
    Closing,
    Address,
    Title,
    FullScreen,
    StatusMessage,
    Favicon,
    LoadStart,
    LoadEnd,
    LoadingState,
    LoadingProgress,
    FocusChange,
    ZoomOut,
    ZoomIn,
    ZoomReset,
    OpenUrl,
};

typedef enum class SiteSecurityLevel{
    None,
    Http,
    Https,
    File,
    LocalScheme
}SecurityLevel;

enum class ToolWndShowMode{
    None,
    Popup,
    Dock
};

struct History{
    QString lastVisitedTime;
    QString url;
    QString title;
    int count;
};

Q_DECLARE_METATYPE(History);

typedef enum UserMenuId{
    MENU_ID_USER_OPENLINK = MENU_ID_USER_FIRST + 200,

    MENU_ID_USER_COPY,
    MENU_ID_SAVE_AS,
    MENU_ID_USER_SEARCH_SELECTED,
    MENU_ID_USER_OPEN_LINK_WINDOW,
    MENU_ID_USER_OPEN_LINK_PAGE,
    MENU_ID_USER_OPEN_LINK_INPRIVATE,
    MENU_ID_USER_PRINT,
    MENU_ID_USER_VIEW_SOURCE,
    MENU_ID_USER_SAVEAS,
    MENU_ID_USER_INSPECT_ELEMENT,
    MENU_ID_USER_SHOW_SSL_INFO,
    MENU_ID_USER_COPY_LINK_URL,
    MENU_ID_USER_OPEN_IMAGE_PAGE,
    MENU_ID_USER_SAVE_IMAGE_AS,
    MENU_ID_USER_COPY_IMAGE,
    MENU_ID_USER_COPY_IMAGE_LIINK,
    MENU_ID_USER_FRAME_VIEW_SOURCE,
    MENU_ID_USER_FRAME_REFRESH,
    MENU_ID_SHOW_SSL_INFO,
    MENU_ID_USER_EMOJI,
}cef_menu_id_t_user;

enum class CefShortcutCmd{
    Refresh,
    Fullscreen,
    DevTool,
    ZoomIn,
    ZoomReset,
    ZoomOut,
    Print,
    NewTab,
    NewWnd,
    NewPrivateWnd,
    History,
    Download,
    CloseTab,
    SwitchTab,
    NaviBack,
    NaviForward,
};

#endif // GLOBALDEF_H
