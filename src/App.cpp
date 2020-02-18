//
// App.cpp
//

#include "App.h"
#include "Config.h"
#include "Core.h"
#include "PreferencesDialog.h"
#include "ResourceLoader.h"
#include "TrayIcon.h"

bool App::OnInit()
{
    m_pLocale = new wxLocale(wxLANGUAGE_DUTCH);
    m_pLocale->AddCatalogLookupPathPrefix(GetTranslationsPath());
    m_pLocale->AddCatalog("Clevy");

    m_pConfig = new Config();
    m_pCore = new Core(m_pConfig);
    m_pPreferencesDialog = new PreferencesDialog(this, m_pConfig);
    m_pTrayIcon = new TrayIcon(this, m_pConfig);

    return true;
}

int App::OnExit()
{
    delete m_pTrayIcon;
    delete m_pPreferencesDialog;
    delete m_pCore;
    delete m_pConfig;

    delete m_pLocale;

    return wxApp::OnExit();
}

void App::ShowPreferencesDialog()
{
    m_pPreferencesDialog->Show();
}

void App::UpdatePreferencesDialog()
{
    m_pPreferencesDialog->TransferDataToWindow();
}

void App::UpdateTrayIcon()
{
    m_pTrayIcon->UpdateIcon();
}

wxIMPLEMENT_APP(App);
