//
// App.h
//

#pragma once

#include <wx/wx.h>

class KeyboardWindows;
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
    KeyboardWindows* m_pKeyboardWindows;
    PreferencesDialog* m_pPreferencesDialog;
    TrayIcon* m_pTrayIcon;
};
