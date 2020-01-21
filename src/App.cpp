//
// App.cpp
//

#include "App.h"
#include "Config.h"
#include "Core.h"
#include "PreferencesDialog.h"
#include "TrayIcon.h"

bool App::OnInit()
{
    m_pConfig = new Config();
    m_pCore = new Core(m_pConfig);
    m_pPreferencesDialog = new PreferencesDialog(m_pConfig);
    m_pTrayIcon = new TrayIcon(this);

    return true;
}

int App::OnExit()
{
    delete m_pTrayIcon;
    delete m_pPreferencesDialog;
    delete m_pCore;
    delete m_pConfig;

    return wxApp::OnExit();
}

void App::ShowPreferencesDialog()
{
    m_pPreferencesDialog->Show();
}

wxIMPLEMENT_APP(App);
