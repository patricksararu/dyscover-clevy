//
// App.h
//

#pragma once

#include <wx/app.h>

class Config;
class Core;
class PreferencesDialog;
class TrayIcon;

class App : public wxApp
{
private:
    virtual bool OnInit() override;
    virtual int OnExit() override;

public:
    void ShowPreferencesDialog();
    void UpdatePreferencesDialog();
    void UpdateTrayIcon();

private:
    Config* m_pConfig;
    Core* m_pCore;
    PreferencesDialog* m_pPreferencesDialog;
    TrayIcon* m_pTrayIcon;
};
