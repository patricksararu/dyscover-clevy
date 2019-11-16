//
// PreferencesDialog.cpp
//

#include "Keyboard.h"
#include "PreferencesDialog.h"

wxBEGIN_EVENT_TABLE(PreferencesDialog, wxDialog)
    EVT_THREAD(wxEVT_KEY, PreferencesDialog::OnKeyboardEvent)
wxEND_EVENT_TABLE()

PreferencesDialog::PreferencesDialog(App* pApp)
    : wxDialog(nullptr, wxID_ANY, "Preferences", wxDefaultPosition, wxSize(800, 600), wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER)
{
    m_pApp = pApp;

    m_pListBox = new wxListBox(this, wxID_ANY);
}

void PreferencesDialog::OnKeyboardEvent(wxThreadEvent &event)
{
    wxLogDebug("PreferencesDialog::OnKeyboardEvent(event = %p)", event.GetRefData());
}

void PreferencesDialog::KeyboardCallback(KeyEventType type, int keyCode, int scanCode, int flags, int time, void* p)
{
    PreferencesDialog* pThis = (PreferencesDialog*)p;
    pThis->m_pListBox->Append(wxString::Format("key event: type = %d, key = %d (\'%c\'), scancode = %d, flags = 0x%02x, time = %d", type, keyCode, keyCode, scanCode, flags, time));
    pThis->m_pListBox->SetSelection(pThis->m_pListBox->GetCount() - 1);
}
