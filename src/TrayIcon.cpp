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
    ID_ENABLED = wxID_HIGHEST + 1,
    ID_LETTERS_AND_NUMBERS,
    ID_LETTER_COMBINATIONS,
    ID_WORDS,
    ID_SENTENCES,
    ID_SELECTION,
};

TrayIcon::TrayIcon(App* pApp, Config* pConfig)
{
    m_pApp = pApp;
    m_pConfig = pConfig;

    m_icons = LoadDyscoverIcons();

    UpdateIcon();
}

TrayIcon::~TrayIcon()
{
    RemoveIcon();
}

void TrayIcon::UpdateIcon()
{
#ifdef __LICENSING_FULL__
    int iconIndex = m_pApp->IsClevyKeyboardPresent() && m_pConfig->GetEnabled() ? 0 : 5;
#else
    int iconIndex = m_pConfig->GetEnabled() ? 0 : 5;
#endif
    SetIcon(m_icons[iconIndex], _("Clevy Dyscover"));
}

wxMenu* TrayIcon::CreatePopupMenu()
{
    wxMenu* pMenu = new wxMenu();
    pMenu->AppendCheckItem(ID_ENABLED, _("Enabled"));
    pMenu->AppendSeparator();
    pMenu->AppendCheckItem(ID_LETTERS_AND_NUMBERS, _("Letters and numbers"));
    pMenu->AppendCheckItem(ID_LETTER_COMBINATIONS, _("Letter combinations"));
    pMenu->AppendCheckItem(ID_WORDS, _("Words"));
    pMenu->AppendCheckItem(ID_SENTENCES, _("Sentences"));
    pMenu->AppendCheckItem(ID_SELECTION, _("Selection"));
    pMenu->AppendSeparator();
    pMenu->Append(wxID_PREFERENCES, _("Settings"));
    pMenu->Append(wxID_HELP, _("Manual"));
    pMenu->Append(wxID_EXIT, _("Exit"));
    pMenu->Check(ID_ENABLED, m_pConfig->GetEnabled());
    pMenu->Check(ID_LETTERS_AND_NUMBERS, m_pConfig->GetLettersAndNumbers());
    pMenu->Check(ID_LETTER_COMBINATIONS, m_pConfig->GetLetterCombinations());
    pMenu->Check(ID_WORDS, m_pConfig->GetWords());
    pMenu->Check(ID_SENTENCES, m_pConfig->GetSentences());
    pMenu->Check(ID_SELECTION, m_pConfig->GetSelection());
    return pMenu;
}

void TrayIcon::OnMenuEnabled(wxCommandEvent&)
{
    m_pConfig->SetEnabled(!m_pConfig->GetEnabled());

    m_pApp->UpdatePreferencesDialog();

    UpdateIcon();
}

void TrayIcon::OnMenuLettersAndNumbers(wxCommandEvent&)
{
    m_pConfig->SetLettersAndNumbers(!m_pConfig->GetLettersAndNumbers());

    m_pApp->UpdatePreferencesDialog();
}

void TrayIcon::OnMenuLetterCombinations(wxCommandEvent&)
{
    m_pConfig->SetLetterCombinations(!m_pConfig->GetLetterCombinations());

    m_pApp->UpdatePreferencesDialog();
}

void TrayIcon::OnMenuWords(wxCommandEvent&)
{
    m_pConfig->SetWords(!m_pConfig->GetWords());

    m_pApp->UpdatePreferencesDialog();
}

void TrayIcon::OnMenuSentences(wxCommandEvent&)
{
    m_pConfig->SetSentences(!m_pConfig->GetSentences());

    m_pApp->UpdatePreferencesDialog();
}

void TrayIcon::OnMenuSelection(wxCommandEvent&)
{
    m_pConfig->SetSelection(!m_pConfig->GetSelection());

    m_pApp->UpdatePreferencesDialog();
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
    EVT_MENU(ID_ENABLED, TrayIcon::OnMenuEnabled)
    EVT_MENU(ID_LETTERS_AND_NUMBERS, TrayIcon::OnMenuLettersAndNumbers)
    EVT_MENU(ID_LETTER_COMBINATIONS, TrayIcon::OnMenuLetterCombinations)
    EVT_MENU(ID_WORDS, TrayIcon::OnMenuWords)
    EVT_MENU(ID_SENTENCES, TrayIcon::OnMenuSentences)
    EVT_MENU(ID_SELECTION, TrayIcon::OnMenuSelection)
    EVT_MENU(wxID_PREFERENCES, TrayIcon::OnMenuPreferences)
    EVT_MENU(wxID_HELP, TrayIcon::OnMenuHelp)
    EVT_MENU(wxID_EXIT, TrayIcon::OnMenuExit)
wxEND_EVENT_TABLE()
