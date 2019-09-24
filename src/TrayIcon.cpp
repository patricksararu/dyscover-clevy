//
// TrayIcon.cpp
//

#include "App.h"
#include "TrayIcon.h"

TrayIcon::TrayIcon(App* pApp)
{
#ifdef __APPLE__
    wxIcon icon("ClevyIcon.icns", wxBITMAP_TYPE_ICON);
#else
    wxIcon icon("ClevyIcon.ico", wxBITMAP_TYPE_ICO);
#endif
    SetIcon(icon, "Clevy");

    m_pApp = pApp;
}

TrayIcon::~TrayIcon()
{
    RemoveIcon();
}

wxMenu* TrayIcon::CreatePopupMenu()
{
    wxMenu* pMenu = new wxMenu();
    pMenu->Append(wxID_PREFERENCES);
    pMenu->Append(wxID_EXIT);
    return pMenu;
}

void TrayIcon::OnMenuPreferences(wxCommandEvent&)
{
    m_pApp->ShowPreferencesDialog();
}

void TrayIcon::OnMenuExit(wxCommandEvent&)
{
    m_pApp->Exit();
}

wxBEGIN_EVENT_TABLE(TrayIcon, wxTaskBarIcon)
    EVT_MENU(wxID_PREFERENCES, TrayIcon::OnMenuPreferences)
    EVT_MENU(wxID_EXIT, TrayIcon::OnMenuExit)
wxEND_EVENT_TABLE()
