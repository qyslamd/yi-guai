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
    Forward,
    Refresh,
    StopLoading,
    ViewSiteInfo,
    NewTabPage,
    NewWindow,
    NewInprivateWindow,
    ZoomIn,
    ZoomReset,
    ZoomOut,
};

///
/// \brief The PageCmd enum
///
enum class PageCmd{
    None = -1,
    Closing,
    Address,
    Title,
    Favicon,
    LoadingState,
    FocusChange,
};

#endif // GLOBALDEF_H
