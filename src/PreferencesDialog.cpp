//
// PreferencesDialog.cpp
//

#include "Config.h"
#include "PreferencesDialog.h"

PreferencesDialog::PreferencesDialog(Config* pConfig)
    : wxDialog(nullptr, wxID_ANY, "Preferences")
{
    m_pConfig = pConfig;

    m_pSoundCheckBox = new wxCheckBox(this, wxID_ANY, "Sound");
    m_pSoundsCheckBox = new wxCheckBox(this, wxID_ANY, "Sounds");
    m_pTTSCheckBox = new wxCheckBox(this, wxID_ANY, "TTS (Text to Speech)");
    m_pVolumeStaticText = new wxStaticText(this, wxID_ANY, "Volume");
    m_pVolumeSlider = new wxSlider(this, wxID_ANY, 100, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_VERTICAL | wxSL_INVERSE);

    m_pActivateAfterStaticText = new wxStaticText(this, wxID_ANY, "Activate After");
    m_pWordCheckBox = new wxCheckBox(this, wxID_ANY, "Word");
    m_pSelectionSpeakerKeyCheckBox = new wxCheckBox(this, wxID_ANY, "Selection and Speaker key");
    m_pReadAlongCheckBox = new wxCheckBox(this, wxID_ANY, "Read Along");
    m_pSpeedStaticText = new wxStaticText(this, wxID_ANY, "Speed");
    m_pSpeedSlider = new wxSlider(this, wxID_ANY, 0, -75, 75, wxDefaultPosition, wxDefaultSize, wxSL_VERTICAL | wxSL_INVERSE);

    m_pStartWithSystemCheckBox = new wxCheckBox(this, wxID_ANY, "Start with system");
    m_pPausedCheckBox = new wxCheckBox(this, wxID_ANY, "Paused");
    m_pClassicRadioButton = new wxRadioButton(this, wxID_ANY, "Classic");
    m_pCoverRadioButton = new wxRadioButton(this, wxID_ANY, "Cover");

    wxBoxSizer* pSoundSettingsInnerSizer = new wxBoxSizer(wxVERTICAL);
    pSoundSettingsInnerSizer->Add(m_pSoundCheckBox, wxSizerFlags().Border().Expand());
    pSoundSettingsInnerSizer->Add(m_pSoundsCheckBox, wxSizerFlags().Border().Expand());
    pSoundSettingsInnerSizer->Add(m_pTTSCheckBox, wxSizerFlags().Border().Expand());

    wxStaticBoxSizer* pSoundSettingsSizer = new wxStaticBoxSizer(wxHORIZONTAL, this, "Sound Settings");
    pSoundSettingsSizer->Add(pSoundSettingsInnerSizer, wxSizerFlags().Proportion(1));
    pSoundSettingsSizer->Add(m_pVolumeStaticText, wxSizerFlags().Border().Center());
    pSoundSettingsSizer->Add(m_pVolumeSlider);

    wxBoxSizer* pTTSSettingsInnerSizer = new wxBoxSizer(wxVERTICAL);
    pTTSSettingsInnerSizer->Add(m_pActivateAfterStaticText, wxSizerFlags().Border().Expand());
    pTTSSettingsInnerSizer->Add(m_pWordCheckBox, wxSizerFlags().Border().Expand());
    pTTSSettingsInnerSizer->Add(m_pSelectionSpeakerKeyCheckBox, wxSizerFlags().Border().Expand());
    pTTSSettingsInnerSizer->Add(m_pReadAlongCheckBox, wxSizerFlags().Border().Expand());

    wxStaticBoxSizer* pTTSSettingsSizer = new wxStaticBoxSizer(wxHORIZONTAL, this, "Text To Speech");
    pTTSSettingsSizer->Add(pTTSSettingsInnerSizer, wxSizerFlags().Proportion(1));
    pTTSSettingsSizer->Add(m_pSpeedStaticText, wxSizerFlags().Border().Center());
    pTTSSettingsSizer->Add(m_pSpeedSlider);

    wxBoxSizer* pOtherSettingsFirstInnerSizer = new wxBoxSizer(wxHORIZONTAL);
    pOtherSettingsFirstInnerSizer->Add(m_pStartWithSystemCheckBox, wxSizerFlags().Border().Expand().Proportion(1));
    pOtherSettingsFirstInnerSizer->Add(m_pPausedCheckBox, wxSizerFlags().Border().Expand().Proportion(1));

    wxBoxSizer* pOtherSettingsSecondInnerSizer = new wxBoxSizer(wxHORIZONTAL);
    pOtherSettingsSecondInnerSizer->Add(m_pClassicRadioButton, wxSizerFlags().Border().Expand().Proportion(1));
    pOtherSettingsSecondInnerSizer->Add(m_pCoverRadioButton, wxSizerFlags().Border().Expand().Proportion(1));

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

bool PreferencesDialog::TransferDataToWindow()
{
    m_pSoundCheckBox->SetValue(m_pConfig->sound);
    m_pSoundsCheckBox->SetValue(m_pConfig->sounds);
    m_pTTSCheckBox->SetValue(m_pConfig->tts);
    m_pVolumeSlider->SetValue(m_pConfig->volume);

    m_pWordCheckBox->SetValue(m_pConfig->word);
    m_pSelectionSpeakerKeyCheckBox->SetValue(m_pConfig->selection);
    m_pReadAlongCheckBox->SetValue(m_pConfig->readalong);
    m_pSpeedSlider->SetValue(m_pConfig->speed);

    m_pStartWithSystemCheckBox->SetValue(m_pConfig->startwithsystem);
    m_pPausedCheckBox->SetValue(m_pConfig->paused);
    m_pClassicRadioButton->SetValue(m_pConfig->layout == Layout::DutchClassic);
    m_pCoverRadioButton->SetValue(m_pConfig->layout == Layout::DutchKWeC);

    return true;
}

bool PreferencesDialog::TransferDataFromWindow()
{
    m_pConfig->sound = m_pSoundCheckBox->GetValue();
    m_pConfig->sounds = m_pSoundsCheckBox->GetValue();
    m_pConfig->tts = m_pTTSCheckBox->GetValue();
    m_pConfig->volume = m_pVolumeSlider->GetValue();

    m_pConfig->word = m_pWordCheckBox->GetValue();
    m_pConfig->selection = m_pSelectionSpeakerKeyCheckBox->GetValue();
    m_pConfig->readalong = m_pReadAlongCheckBox->GetValue();
    m_pConfig->speed = m_pSpeedSlider->GetValue();

    m_pConfig->startwithsystem = m_pStartWithSystemCheckBox->GetValue();
    m_pConfig->paused = m_pPausedCheckBox->GetValue();
    m_pConfig->layout = m_pClassicRadioButton->GetValue() ? Layout::DutchClassic : Layout::DutchKWeC;

    return true;
}
