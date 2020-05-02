//
// App.cpp
//

#include <wx/splash.h>

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
    m_pLocale->AddCatalog("Dyscover");

    wxSplashScreen splashScreen(LoadSplashBitmap(), wxSPLASH_CENTRE_ON_SCREEN | wxSPLASH_TIMEOUT, 5000, nullptr, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSTAY_ON_TOP);

    m_pConfig = new Config();
    m_pCore = new Core(this, m_pConfig);
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

void App::UpdateAudioVolume()
{
    m_pCore->UpdateAudioVolume();
}

bool App::IsClevyKeyboardPresent()
{
    return m_pCore->IsClevyKeyboardPresent();
}

wxIMPLEMENT_APP(App);
