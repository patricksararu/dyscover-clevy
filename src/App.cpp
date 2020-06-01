//
// App.cpp
//

#include <wx/msgdlg.h>
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

    wxTranslations* pTranslations = wxTranslations::Get();
    pTranslations->SetLoader(new wxResourceTranslationsLoader());
    pTranslations->AddCatalog("Dyscover");

    m_pConfig = new Config();

#ifdef __LICENSING_DEMO__
    m_pDemoLicensing = new DemoLicensing(m_pConfig, this);
    if (m_pDemoLicensing->HasDemoLicenseExpired())
    {
        wxMessageBox(_("Your 30-day demo license has expired! Please purchase a license."), _("Clevy Dyscover demo license expired"), wxSTAY_ON_TOP);

        // Clean up now because OnExit() won't be called since we are returning false here.
        delete m_pDemoLicensing;
        delete m_pConfig;
        delete m_pLocale;

        return false;
    }
#endif

    wxSplashScreen splashScreen(LoadSplashBitmap(), wxSPLASH_CENTRE_ON_SCREEN | wxSPLASH_TIMEOUT, 5000, nullptr, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSTAY_ON_TOP);

    m_pDevice = Device::Create(this);
    m_pCore = new Core(this, m_pConfig, m_pDevice);
    m_pPreferencesDialog = new PreferencesDialog(this, m_pConfig);
    m_pTrayIcon = new TrayIcon(this, m_pConfig);

    return true;
}

int App::OnExit()
{
    delete m_pTrayIcon;
    delete m_pPreferencesDialog;
    delete m_pDevice;
#ifdef __LICENSING_DEMO__
    delete m_pDemoLicensing;
#endif
    delete m_pCore;
    delete m_pConfig;

    delete m_pLocale;

    return wxApp::OnExit();
}

void App::OnClevyKeyboardConnected()
{
    UpdatePreferencesDialog();

#ifdef __LICENSING_FULL__
    m_pTrayIcon->UpdateIcon();
#endif

    m_pCore->OnClevyKeyboardConnected();
}

void App::OnClevyKeyboardDisconnected()
{
    UpdatePreferencesDialog();

#ifdef __LICENSING_FULL__
    m_pTrayIcon->UpdateIcon();
#endif

    m_pCore->OnClevyKeyboardDisconnected();
}

void App::OnDemoTimeLimitExpired()
{
    wxMessageBox(_("Your 30 minute demo has expired. Please purchase a license."), _("Clevy Dyscover demo license"), wxSTAY_ON_TOP);

    Exit();
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
    return m_pDevice->IsClevyKeyboardPresent();
}

#ifdef __LICENSING_DEMO__
int App::GetDemoDaysRemaining()
{
    return m_pDemoLicensing->GetDaysRemaining();
}
#endif

wxIMPLEMENT_APP(App);
