#ifndef GLOBALDEF_H
#define GLOBALDEF_H

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

enum class ToolWndType
{
    None,
    AddFavorite,
    Favorite,
    History,
    Download,
    UserInfo,
};

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
/// \brief The PageCmd enum
///
enum class PageCmd{
    None = -1,
    Closing,
    Address,
    Title,
    FullScreen,
    StatusMessage,
    Favicon,
    LoadStart,
    LoadEnd,
    LoadingState,
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
} ;

#endif // GLOBALDEF_H
