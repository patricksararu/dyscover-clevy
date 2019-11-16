//
// App.h
//

#pragma once

#include <wx/wx.h>

#include "Keyboard.h"

class PreferencesDialog;
class TrayIcon;

//class Keyboard;
//enum class KeyEventType;

class App : public wxApp, private IKeyTranslator
{
private:
    virtual bool OnInit() override;
    virtual int OnExit() override;

    virtual Key* Translate(KeyEventType eventType, Key key, bool shift, bool ctrl, bool alt, time_t time) override;

public:
    void ShowPreferencesDialog();

private:
    PreferencesDialog* m_pPreferencesDialog;
    TrayIcon* m_pTrayIcon;

    Keyboard* m_pKeyboard;
};
