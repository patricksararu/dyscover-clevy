//
// TrayIcon.cpp
//

#include <wx/menu.h>
#include <wx/utils.h>

#include "App.h"
#include "Config.h"
#include "ResourceLoader.h"
#include "TrayIcon.h"

enum
{
    ID_SOUND = wxID_HIGHEST + 1,
    ID_SOUNDS,
    ID_TTS,
    ID_PAUSED,
};

TrayIcon::TrayIcon(App* pApp, Config* pConfig)
{
    m_pApp = pApp;
    m_pConfig = pConfig;

    m_icons = LoadClevyIcons();

    UpdateIcon();
}

TrayIcon::~TrayIcon()
{
    RemoveIcon();
}

void TrayIcon::UpdateIcon()
{
    int iconIndex;

    if (m_pConfig->GetPaused())
    {
        iconIndex = 6;
    }
    else if (m_pConfig->GetSound())
    {
        if (m_pConfig->GetSounds())
        {
            iconIndex = m_pConfig->GetTTS() ? 0 : 1;
        }
        else
        {
            iconIndex = m_pConfig->GetTTS() ? 2 : 3;
        }
    }
    else
    {
        iconIndex = m_pConfig->GetSounds() && !m_pConfig->GetTTS() ? 4 : 5;
    }

    SetIcon(m_icons[iconIndex], _("Clevy"));
}

wxMenu* TrayIcon::CreatePopupMenu()
{
    wxMenu* pMenu = new wxMenu();
    pMenu->AppendCheckItem(ID_SOUND, _("Sound"));
    pMenu->AppendCheckItem(ID_SOUNDS, _("Sounds"));
    pMenu->AppendCheckItem(ID_TTS, _("TTS"));
    pMenu->AppendSeparator();
    pMenu->AppendCheckItem(ID_PAUSED, _("Paused"));
    pMenu->AppendSeparator();
    pMenu->Append(wxID_PREFERENCES, _("Settings"));
    pMenu->Append(wxID_HELP, _("Manual"));
    pMenu->Append(wxID_EXIT, _("Exit"));
    pMenu->Check(ID_SOUND, m_pConfig->GetSound());
    pMenu->Check(ID_SOUNDS, m_pConfig->GetSounds());
    pMenu->Check(ID_TTS, m_pConfig->GetTTS());
    pMenu->Check(ID_PAUSED, m_pConfig->GetPaused());
    return pMenu;
}

void TrayIcon::OnMenuSound(wxCommandEvent&)
{
    m_pConfig->SetSound(!m_pConfig->GetSound());

    m_pApp->UpdatePreferencesDialog();

    UpdateIcon();
}

void TrayIcon::OnMenuSounds(wxCommandEvent&)
{
    m_pConfig->SetSounds(!m_pConfig->GetSounds());

    m_pApp->UpdatePreferencesDialog();

    UpdateIcon();
}

void TrayIcon::OnMenuTTS(wxCommandEvent&)
{
    m_pConfig->SetTTS(!m_pConfig->GetTTS());

    m_pApp->UpdatePreferencesDialog();

    UpdateIcon();
}

void TrayIcon::OnMenuPaused(wxCommandEvent&)
{
    m_pConfig->SetPaused(!m_pConfig->GetPaused());

    m_pApp->UpdatePreferencesDialog();

    UpdateIcon();
}

void TrayIcon::OnMenuPreferences(wxCommandEvent&)
{
    m_pApp->ShowPreferencesDialog();
}

void TrayIcon::OnMenuHelp(wxCommandEvent&)
{
    wxLaunchDefaultBrowser("http://www.clevy.nl/dyscover2-handleiding");
}

void TrayIcon::OnMenuExit(wxCommandEvent&)
{
    m_pApp->Exit();
}

wxBEGIN_EVENT_TABLE(TrayIcon, wxTaskBarIcon)
    EVT_MENU(ID_SOUND, TrayIcon::OnMenuSound)
    EVT_MENU(ID_SOUNDS, TrayIcon::OnMenuSounds)
    EVT_MENU(ID_TTS, TrayIcon::OnMenuTTS)
    EVT_MENU(ID_PAUSED, TrayIcon::OnMenuPaused)
    EVT_MENU(wxID_PREFERENCES, TrayIcon::OnMenuPreferences)
    EVT_MENU(wxID_HELP, TrayIcon::OnMenuHelp)
    EVT_MENU(wxID_EXIT, TrayIcon::OnMenuExit)
wxEND_EVENT_TABLE()
