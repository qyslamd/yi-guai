#ifndef GLOBALDEF_H
#define GLOBALDEF_H

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
    NewTabPage,
    NewWindow,
    NewInprivateWindow,
    ZoomIn,
    ZoomReset,
    ZoomOut,
};

enum class PageCmd{
    None = -1,
    Closing,
    Address,
    Title,
    LoadingState,
};

#endif // GLOBALDEF_H
