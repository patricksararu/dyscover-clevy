//
// App.h
//

#pragma once

#include <wx/wx.h>

class PreferencesDialog;
class TrayIcon;

class App : public wxApp
{
private:
    virtual bool OnInit() override;
    virtual int OnExit() override;

public:
    void ShowPreferencesDialog();

private:
    PreferencesDialog* m_pPreferencesDialog;
    TrayIcon* m_pTrayIcon;
};
