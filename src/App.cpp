//
// App.cpp
//

#include "App.h"
#include "Core.h"
#include "PreferencesDialog.h"
#include "TrayIcon.h"

bool App::OnInit()
{
    m_pCore = new Core();
    m_pPreferencesDialog = new PreferencesDialog(this);
    m_pTrayIcon = new TrayIcon(this);

    return true;
}

int App::OnExit()
{
    delete m_pTrayIcon;
    delete m_pPreferencesDialog;
    delete m_pCore;

    return wxApp::OnExit();
}

void App::ShowPreferencesDialog()
{
    m_pPreferencesDialog->Show();
}

wxIMPLEMENT_APP(App);
