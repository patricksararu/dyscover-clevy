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
    wxDECLARE_EVENT_TABLE();

    App* m_pApp;

    wxListBox* m_pListBox;

    void OnKeyboardEvent(wxThreadEvent& event);

public:
    static void KeyboardCallback(KeyEventType, int, int, int, int, void*);
};
