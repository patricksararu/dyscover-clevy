//
// DesktopToolbarWindows.cpp
//

#include "DesktopToolbarWindows.h"

DesktopToolbarWindows::DesktopToolbarWindows()
{
    ZeroMemory(&m_appBarData, sizeof(APPBARDATA));
    //m_appBarData.
    SHAppBarMessage(ABM_NEW, &m_appBarData);
}

DesktopToolbarWindows::~DesktopToolbarWindows()
{
}
