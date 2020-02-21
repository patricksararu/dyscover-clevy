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

    wxCheckBox* m_pSoundCheckBox;
    wxCheckBox* m_pSoundsCheckBox;
    wxCheckBox* m_pTTSCheckBox;
    wxStaticText* m_pVolumeStaticText;
    wxSlider* m_pVolumeSlider;

    wxStaticText* m_pActivateAfterStaticText;
    wxCheckBox* m_pWordCheckBox;
    wxCheckBox* m_pSentenceCheckBox;
    wxCheckBox* m_pSelectionSpeakerKeyCheckBox;
    wxCheckBox* m_pReadAlongCheckBox;
    wxStaticText* m_pSpeedStaticText;
    wxSlider* m_pSpeedSlider;

    wxCheckBox* m_pStartWithSystemCheckBox;
    wxCheckBox* m_pPausedCheckBox;
    wxRadioButton* m_pClassicRadioButton;
    wxRadioButton* m_pCoverRadioButton;

    void OnSoundChanged(wxCommandEvent&);
    void OnSoundsChanged(wxCommandEvent&);
    void OnTTSChanged(wxCommandEvent&);
    void OnVolumeChanged(wxCommandEvent&);

    void OnWordChanged(wxCommandEvent&);
    void OnSentenceChanged(wxCommandEvent&);
    void OnSelectionSpeakerKeyChanged(wxCommandEvent&);
    void OnReadAlongChanged(wxCommandEvent&);
    void OnSpeedChanged(wxCommandEvent&);

    void OnStartWithSystemChanged(wxCommandEvent&);
    void OnPausedChanged(wxCommandEvent&);
    void OnLayoutChanged(wxCommandEvent&);
};
