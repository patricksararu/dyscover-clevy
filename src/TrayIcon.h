//
// TrayIcon.h
//

#pragma once

#include <wx/icon.h>
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
    void OnMenuEnabled(wxCommandEvent&);
    void OnMenuLettersAndNumbers(wxCommandEvent&);
    void OnMenuLetterCombinations(wxCommandEvent&);
    void OnMenuWords(wxCommandEvent&);
    void OnMenuSentences(wxCommandEvent&);
    void OnMenuSelection(wxCommandEvent&);
    void OnMenuPreferences(wxCommandEvent&);
    void OnMenuHelp(wxCommandEvent&);
    void OnMenuExit(wxCommandEvent&);

    App* m_pApp;
    Config* m_pConfig;

    wxIconArray m_icons;
};
