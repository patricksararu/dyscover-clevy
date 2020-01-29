//
// PreferencesDialog.h
//

#pragma once

#include <wx/wx.h>

class App;
class Config;

class PreferencesDialog : public wxDialog
{
public:
    PreferencesDialog(App*, Config*);

private:
    App* m_pApp;
    Config* m_pConfig;

    wxCheckBox* m_pSoundCheckBox;
    wxCheckBox* m_pSoundsCheckBox;
    wxCheckBox* m_pTTSCheckBox;
    wxStaticText* m_pVolumeStaticText;
    wxSlider* m_pVolumeSlider;

    wxStaticText* m_pActivateAfterStaticText;
    wxCheckBox* m_pWordCheckBox;
    wxCheckBox* m_pSelectionSpeakerKeyCheckBox;
    wxCheckBox* m_pReadAlongCheckBox;
    wxStaticText* m_pSpeedStaticText;
    wxSlider* m_pSpeedSlider;

    wxCheckBox* m_pStartWithSystemCheckBox;
    wxCheckBox* m_pPausedCheckBox;
    wxRadioButton* m_pClassicRadioButton;
    wxRadioButton* m_pCoverRadioButton;

    virtual bool TransferDataToWindow() override;
    virtual bool TransferDataFromWindow() override;
};
