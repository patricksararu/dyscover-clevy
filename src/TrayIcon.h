//
// TrayIcon.h
//

#pragma once

#include <wx/taskbar.h>

class App;

class TrayIcon : public wxTaskBarIcon
{
public:
    TrayIcon(App*);
    ~TrayIcon();

private:
    wxDECLARE_EVENT_TABLE();

    wxMenu* CreatePopupMenu() override;
    void OnMenuPreferences(wxCommandEvent&);
    void OnMenuExit(wxCommandEvent&);

    App* m_pApp;
};
