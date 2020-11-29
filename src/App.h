//
// App.h
//

#pragma once

#include <wx/app.h>
#include <wx/snglinst.h>

#ifdef WIN32
#include "DesktopToolbarWindows.h"
#endif // WIN32
#include "Device.h"
#include "LicensingDemo.h"

class Config;
class Core;
class PreferencesDialog;
class TrayIcon;

class App : public wxApp, public IDeviceListener, public IDemoLicensingListener
{
private:
    virtual bool OnInit() override;
    virtual int OnExit() override;

public:
    virtual void OnClevyKeyboardConnected() override;
    virtual void OnClevyKeyboardDisconnected() override;

    virtual void OnDemoTimeLimitExpired() override;

    void ShowPreferencesDialog();
    void UpdatePreferencesDialog();
    void UpdateTrayIcon();
    void UpdateAudioVolume();

    bool IsClevyKeyboardPresent();

#ifdef __LICENSING_DEMO__
    int GetDemoDaysRemaining();
#endif

private:
    wxLocale* m_pLocale;
    wxSingleInstanceChecker* m_pSingleInstanceChecker;

    Config* m_pConfig;
    Core* m_pCore;
#ifdef __LICENSING_DEMO__
    DemoLicensing* m_pDemoLicensing;
#endif
#ifdef WIN32
    DesktopToolbarWindows* m_pDesktopToolbar;
#endif // WIN32
    Device* m_pDevice;
    PreferencesDialog* m_pPreferencesDialog;
    TrayIcon* m_pTrayIcon;
};
