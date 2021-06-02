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
    ZoomReset
};

#endif // GLOBALDEF_H
