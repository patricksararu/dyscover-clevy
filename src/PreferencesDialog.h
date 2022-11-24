//
// PreferencesDialog.h
//

#pragma once

#include <wx/checkbox.h>
#include <wx/dialog.h>
#include <wx/radiobut.h>
#include <wx/slider.h>
#include <wx/stattext.h>

class App;
class Config;

class PreferencesDialog : public wxDialog
{
public:
    PreferencesDialog(App*, Config*);

    virtual bool TransferDataToWindow() override;

private:
    wxDECLARE_EVENT_TABLE();

    App* m_pApp;
    Config* m_pConfig;

    wxStaticText* m_pKeyboardStatusLabel;
    wxStaticText* m_pKeyboardStatusValue;
    wxStaticText* m_pKeyboardMethodLabel;
    wxRadioButton* m_pKeyboardMethodDyscover;
    wxRadioButton* m_pKeyboardMethodDefault;
#ifdef __LANGUAGE_NL__
    wxRadioButton* m_pKeyboardMethodKWeC;
#endif

    wxCheckBox* m_pSoftwareEnabled;
    wxCheckBox* m_pSoftwareAutostart;

    wxCheckBox* m_pSoundLetters;
    wxCheckBox* m_pSoundWords;
    wxCheckBox* m_pSoundSentences;
    wxCheckBox* m_pSoundSelection;
    wxStaticText* m_pSoundVolumeLabel;
    wxSlider* m_pSoundVolume;
    wxStaticText* m_pSoundSpeedLabel;
    wxSlider* m_pSoundSpeed;

    wxStaticText* m_pVersionLabel;
    wxStaticText* m_pVersionValue;
    wxStaticText* m_pVersionLanguage;

#ifdef __LICENSING_DEMO__
    wxStaticText* m_pDemoDaysRemaining;
#endif
#ifdef __LICENSING_NONE__
    wxStaticText* m_pLicensingDisabledWarning;
#endif

    void OnKeyboardMethodChanged(wxCommandEvent&);

    void OnSoftwareEnabledChanged(wxCommandEvent&);
    void OnSoftwareAutoStartChanged(wxCommandEvent&);

    void OnSoundLettersChanged(wxCommandEvent&);
    void OnSoundWordsChanged(wxCommandEvent&);
    void OnSoundSentencesChanged(wxCommandEvent&);
    void OnSoundSelectionChanged(wxCommandEvent&);
    void OnSoundVolumeChanged(wxCommandEvent&);
    void OnSoundSpeedChanged(wxCommandEvent&);
};
