//
// App.cpp
//

#include "App.h"
#include "Keyboard.h"
#include "PreferencesDialog.h"
#include "TrayIcon.h"

bool App::OnInit()
{
    m_pPreferencesDialog = new PreferencesDialog(this);
    m_pTrayIcon = new TrayIcon(this);

    m_pKeyboard = Keyboard::create(this, m_pPreferencesDialog, PreferencesDialog::KeyboardCallback, m_pPreferencesDialog);

    return true;
}

int App::OnExit()
{
    delete m_pKeyboard;

    delete m_pTrayIcon;
    delete m_pPreferencesDialog;

    return wxApp::OnExit();
}

Key* App::Translate(KeyEventType eventType, Key key, bool shift, bool ctrl, bool alt, time_t time)
{
    return nullptr;
}

void App::ShowPreferencesDialog()
{
    m_pPreferencesDialog->Show();
}

wxIMPLEMENT_APP(App);
