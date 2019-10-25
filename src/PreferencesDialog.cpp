//
// PreferencesDialog.cpp
//

#include "PreferencesDialog.h"
#include "Keyboard.h"

PreferencesDialog::PreferencesDialog(App* pApp)
    : wxDialog(nullptr, wxID_ANY, "Preferences", wxDefaultPosition, wxSize(800, 600), wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER)
{
    m_pApp = pApp;

    m_pKeyboard = Keyboard::create(PreferencesDialog::KeyboardCallback, this);

    m_pListBox = new wxListBox(this, wxID_ANY);
}

void PreferencesDialog::KeyboardCallback(KeyEventType type, int keyCode, int scanCode, int flags, int time, void* p)
{
    PreferencesDialog* pThis = (PreferencesDialog*)p;
    pThis->m_pListBox->Append(wxString::Format("key event: type = %d, key = %d (\'%c\'), scancode = %d, flags = 0x%02x, time = %d", type, keyCode, keyCode, scanCode, flags, time));
    pThis->m_pListBox->SetSelection(pThis->m_pListBox->GetCount() - 1);
}
