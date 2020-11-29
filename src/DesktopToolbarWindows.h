//
// DesktopToolbarWindows.h
//

#pragma once

#include <wx/frame.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <shellapi.h>

class DesktopToolbarWindows : public wxFrame
{
public:
	DesktopToolbarWindows();
	~DesktopToolbarWindows();

private:
	APPBARDATA m_appBarData;

	static void AppBarCallback(HWND, UINT, LPARAM);
};
