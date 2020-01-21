//
// TrayIcon.cpp
//

#include <wx/utils.h>

#include "App.h"
#include "Config.h"
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
#ifdef __APPLE__
    wxIcon icon("ClevyIcon.icns", wxBITMAP_TYPE_ICON);
#else
    wxIcon icon("ClevyIcon.ico", wxBITMAP_TYPE_ICO);
#endif
    SetIcon(icon, "Clevy");

    m_pApp = pApp;
    m_pConfig = pConfig;
}

TrayIcon::~TrayIcon()
{
    RemoveIcon();
}

wxMenu* TrayIcon::CreatePopupMenu()
{
    wxMenu* pMenu = new wxMenu();
    pMenu->AppendCheckItem(ID_SOUND, "Sound");
    pMenu->AppendCheckItem(ID_SOUNDS, "Sounds");
    pMenu->AppendCheckItem(ID_TTS, "TTS");
    pMenu->AppendSeparator();
    pMenu->AppendCheckItem(ID_PAUSED, "Paused");
    pMenu->AppendSeparator();
    pMenu->Append(wxID_PREFERENCES);
    pMenu->Append(wxID_HELP);
    pMenu->Append(wxID_EXIT);
    pMenu->Check(ID_SOUND, m_pConfig->sound);
    pMenu->Check(ID_SOUNDS, m_pConfig->sounds);
    pMenu->Check(ID_TTS, m_pConfig->tts);
    pMenu->Check(ID_PAUSED, m_pConfig->paused);
    return pMenu;
}

void TrayIcon::OnMenuSound(wxCommandEvent&)
{
    m_pConfig->sound = !m_pConfig->sound;
}

void TrayIcon::OnMenuSounds(wxCommandEvent&)
{
    m_pConfig->sounds = !m_pConfig->sounds;
}

void TrayIcon::OnMenuTTS(wxCommandEvent&)
{
    m_pConfig->tts = !m_pConfig->tts;
}

void TrayIcon::OnMenuPaused(wxCommandEvent&)
{
    m_pConfig->paused = !m_pConfig->paused;
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
