//
// PreferencesDialog.h
//

#pragma once

#include <wx/wx.h>

class App;

class PreferencesDialog : public wxDialog
{
public:
    PreferencesDialog(App*);

private:
    App* m_pApp;
};
