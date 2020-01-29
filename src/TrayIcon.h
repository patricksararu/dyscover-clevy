//
// TrayIcon.h
//

#pragma once

#include <wx/taskbar.h>

class App;
class Config;

class TrayIcon : public wxTaskBarIcon
{
public:
    TrayIcon(App*, Config*);
    ~TrayIcon();

    void UpdateIcon();

private:
    wxDECLARE_EVENT_TABLE();

    wxMenu* CreatePopupMenu() override;
    void OnMenuSound(wxCommandEvent&);
    void OnMenuSounds(wxCommandEvent&);
    void OnMenuTTS(wxCommandEvent&);
    void OnMenuPaused(wxCommandEvent&);
    void OnMenuPreferences(wxCommandEvent&);
    void OnMenuHelp(wxCommandEvent&);
    void OnMenuExit(wxCommandEvent&);

    App* m_pApp;
    Config* m_pConfig;

    wxIconArray* m_pIcons;
};
