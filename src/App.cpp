//
// App.cpp
//

#include "App.h"
#include "PreferencesDialog.h"
#include "TrayIcon.h"

bool App::OnInit()
{
    m_pPreferencesDialog = new PreferencesDialog(this);
    m_pTrayIcon = new TrayIcon(this);

    return true;
}

int App::OnExit()
{
    delete m_pTrayIcon;
    delete m_pPreferencesDialog;

    return wxApp::OnExit();
}

void App::ShowPreferencesDialog()
{
    m_pPreferencesDialog->Show();
}

wxIMPLEMENT_APP(App);
