//
// PreferencesDialog.cpp
//

#include "PreferencesDialog.h"

PreferencesDialog::PreferencesDialog(App* pApp)
    : wxDialog(nullptr, wxID_ANY, "Preferences")
{
    m_pApp = pApp;
}
