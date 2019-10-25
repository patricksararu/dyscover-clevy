//
// PreferencesDialog.h
//

#pragma once

#include <wx/wx.h>

class App;
class Keyboard;
enum class KeyEventType;

class PreferencesDialog : public wxDialog
{
public:
    PreferencesDialog(App*);

private:
    App* m_pApp;
    Keyboard* m_pKeyboard;

    wxListBox* m_pListBox;

    //void OnKeyboardCallback();

    static void KeyboardCallback(KeyEventType, int, int, int, int, void*);
};
