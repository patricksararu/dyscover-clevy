//
// TrayIcon.cpp
//

#include <wx/wx.h>

#include "TrayIcon.h"

TrayIcon::TrayIcon()
{
#ifdef __APPLE__
    wxIcon icon("../res/ClevyIcon.icns", wxBITMAP_TYPE_ICON);
#else
    wxIcon icon("../res/ClevyIcon.ico", wxBITMAP_TYPE_ICO);
#endif
    SetIcon(icon, "Clevy");
}

wxMenu* TrayIcon::CreatePopupMenu()
{
    wxMenu* pMenu = new wxMenu();
    pMenu->Append(wxID_EXIT);
    return pMenu;
}

void TrayIcon::OnMenuExit(wxCommandEvent&)
{
    RemoveIcon();

    wxExit();
}

wxBEGIN_EVENT_TABLE(TrayIcon, wxTaskBarIcon)
    EVT_MENU(wxID_EXIT, TrayIcon::OnMenuExit)
wxEND_EVENT_TABLE()
