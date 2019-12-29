//
// PreferencesDialog.cpp
//

#include "PreferencesDialog.h"

PreferencesDialog::PreferencesDialog(App* pApp)
    : wxDialog(nullptr, wxID_ANY, "Preferences")
{
    m_pApp = pApp;

    wxCheckBox* pSoundCheckBox = new wxCheckBox(this, wxID_ANY, "Sound");
    wxCheckBox* pSoundsCheckBox = new wxCheckBox(this, wxID_ANY, "Sounds");
    wxCheckBox* pTTSCheckBox = new wxCheckBox(this, wxID_ANY, "TTS (Text to Speech)");
    wxStaticText* pVolumeStaticText = new wxStaticText(this, wxID_ANY, "Volume");
    wxSlider* pVolumeSlider = new wxSlider(this, wxID_ANY, 100, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_VERTICAL | wxSL_INVERSE);

    wxStaticText* pActivateAfterStaticText = new wxStaticText(this, wxID_ANY, "Activate After");
    wxCheckBox* pWordCheckBox = new wxCheckBox(this, wxID_ANY, "Word");
    wxCheckBox* pSelectionSpeakerKeyCheckBox = new wxCheckBox(this, wxID_ANY, "Selection and Speaker key");
    wxCheckBox* pReadAlongCheckBox = new wxCheckBox(this, wxID_ANY, "Read Along");
    wxStaticText* pSpeedStaticText = new wxStaticText(this, wxID_ANY, "Speed");
    wxSlider* pSpeedSlider = new wxSlider(this, wxID_ANY, 0, -75, 75, wxDefaultPosition, wxDefaultSize, wxSL_VERTICAL | wxSL_INVERSE);

    wxCheckBox* pStartWithSystemCheckBox = new wxCheckBox(this, wxID_ANY, "Start with system");
    wxCheckBox* pPausedCheckBox = new wxCheckBox(this, wxID_ANY, "Paused");
    wxRadioButton* pClassicRadioButton = new wxRadioButton(this, wxID_ANY, "Classic");
    wxRadioButton* pCoverRadioButton = new wxRadioButton(this, wxID_ANY, "Cover");

    wxBoxSizer* pSoundSettingsInnerSizer = new wxBoxSizer(wxVERTICAL);
    pSoundSettingsInnerSizer->Add(pSoundCheckBox, wxSizerFlags().Border().Expand());
    pSoundSettingsInnerSizer->Add(pSoundsCheckBox, wxSizerFlags().Border().Expand());
    pSoundSettingsInnerSizer->Add(pTTSCheckBox, wxSizerFlags().Border().Expand());

    wxStaticBoxSizer* pSoundSettingsSizer = new wxStaticBoxSizer(wxHORIZONTAL, this, "Sound Settings");
    pSoundSettingsSizer->Add(pSoundSettingsInnerSizer, wxSizerFlags().Proportion(1));
    pSoundSettingsSizer->Add(pVolumeStaticText, wxSizerFlags().Border().Center());
    pSoundSettingsSizer->Add(pVolumeSlider);

    wxBoxSizer* pTTSSettingsInnerSizer = new wxBoxSizer(wxVERTICAL);
    pTTSSettingsInnerSizer->Add(pActivateAfterStaticText, wxSizerFlags().Border().Expand());
    pTTSSettingsInnerSizer->Add(pWordCheckBox, wxSizerFlags().Border().Expand());
    pTTSSettingsInnerSizer->Add(pSelectionSpeakerKeyCheckBox, wxSizerFlags().Border().Expand());
    pTTSSettingsInnerSizer->Add(pReadAlongCheckBox, wxSizerFlags().Border().Expand());

    wxStaticBoxSizer* pTTSSettingsSizer = new wxStaticBoxSizer(wxHORIZONTAL, this, "Text To Speech");
    pTTSSettingsSizer->Add(pTTSSettingsInnerSizer, wxSizerFlags().Proportion(1));
    pTTSSettingsSizer->Add(pSpeedStaticText, wxSizerFlags().Border().Center());
    pTTSSettingsSizer->Add(pSpeedSlider);

    wxBoxSizer* pOtherSettingsFirstInnerSizer = new wxBoxSizer(wxHORIZONTAL);
    pOtherSettingsFirstInnerSizer->Add(pStartWithSystemCheckBox, wxSizerFlags().Border().Expand().Proportion(1));
    pOtherSettingsFirstInnerSizer->Add(pPausedCheckBox, wxSizerFlags().Border().Expand().Proportion(1));

    wxBoxSizer* pOtherSettingsSecondInnerSizer = new wxBoxSizer(wxHORIZONTAL);
    pOtherSettingsSecondInnerSizer->Add(pClassicRadioButton, wxSizerFlags().Border().Expand().Proportion(1));
    pOtherSettingsSecondInnerSizer->Add(pCoverRadioButton, wxSizerFlags().Border().Expand().Proportion(1));

    wxStaticBoxSizer* pOtherSettingsSizer = new wxStaticBoxSizer(wxVERTICAL, this, "Other Settings");
    pOtherSettingsSizer->Add(pOtherSettingsFirstInnerSizer, wxSizerFlags().Expand());
    pOtherSettingsSizer->Add(pOtherSettingsSecondInnerSizer, wxSizerFlags().Expand());

    wxSizer* pButtonSizer = CreateButtonSizer(wxOK | wxCANCEL | wxAPPLY);

    wxBoxSizer* pRootSizer = new wxBoxSizer(wxVERTICAL);
    pRootSizer->Add(pSoundSettingsSizer, wxSizerFlags().DoubleBorder().Expand());
    pRootSizer->Add(pTTSSettingsSizer, wxSizerFlags().DoubleBorder().Expand());
    pRootSizer->Add(pOtherSettingsSizer, wxSizerFlags().DoubleBorder().Expand());
    pRootSizer->Add(pButtonSizer, wxSizerFlags().Border().Expand());

    SetSizerAndFit(pRootSizer);
}
