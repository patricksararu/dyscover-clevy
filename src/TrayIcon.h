//
// TrayIcon.h
//

#pragma once

#include <wx/taskbar.h>

class TrayIcon : public wxTaskBarIcon
{
public:
    TrayIcon();

private:
    wxDECLARE_EVENT_TABLE();

    wxMenu* CreatePopupMenu() override;
    void OnMenuExit(wxCommandEvent&);
};
