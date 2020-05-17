//
// App.h
//

#pragma once

#include <wx/app.h>

#include "Device.h"

class Config;
class Core;
class PreferencesDialog;
class TrayIcon;

class App : public wxApp, public IDeviceListener
{
private:
    virtual bool OnInit() override;
    virtual int OnExit() override;

public:
    virtual void OnClevyKeyboardConnected() override;
    virtual void OnClevyKeyboardDisconnected() override;

    void ShowPreferencesDialog();
    void UpdatePreferencesDialog();
    void UpdateTrayIcon();
    void UpdateAudioVolume();

    bool IsClevyKeyboardPresent();

private:
    wxLocale* m_pLocale;

    Config* m_pConfig;
    Core* m_pCore;
    Device* m_pDevice;
    PreferencesDialog* m_pPreferencesDialog;
    TrayIcon* m_pTrayIcon;
};
