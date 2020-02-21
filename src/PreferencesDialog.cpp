//
// PreferencesDialog.cpp
//

#include <wx/sizer.h>

#include "App.h"
#include "Config.h"
#include "PreferencesDialog.h"
#include "ResourceLoader.h"

enum
{
    ID_SOUND = wxID_HIGHEST + 1,
    ID_SOUNDS,
    ID_TTS,
    ID_VOLUME,
    ID_WORD,
    ID_SENTENCE,
    ID_SELECTIONSPEAKERKEY,
    ID_READALONG,
    ID_SPEED,
    ID_STARTWITHSYSTEM,
    ID_PAUSED,
    ID_CLASSIC,
    ID_COVER,
};

PreferencesDialog::PreferencesDialog(App* pApp, Config* pConfig)
    : wxDialog(nullptr, wxID_ANY, _("Preferences"))
{
    SetIcon(LoadClevyIcon());

    m_pApp = pApp;
    m_pConfig = pConfig;

    m_pSoundCheckBox = new wxCheckBox(this, ID_SOUND, _("Sound"));
    m_pSoundsCheckBox = new wxCheckBox(this, ID_SOUNDS, _("Sounds"));
    m_pTTSCheckBox = new wxCheckBox(this, ID_TTS, _("TTS (Text to Speech)"));
    m_pVolumeStaticText = new wxStaticText(this, wxID_ANY, _("Volume"));
    m_pVolumeSlider = new wxSlider(this, ID_VOLUME, 100, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_VERTICAL | wxSL_INVERSE);

    m_pActivateAfterStaticText = new wxStaticText(this, wxID_ANY, _("Activate After"));
    m_pWordCheckBox = new wxCheckBox(this, ID_WORD, _("Word"));
    m_pSentenceCheckBox = new wxCheckBox(this, ID_SENTENCE, _("Sentence"));
    m_pSelectionSpeakerKeyCheckBox = new wxCheckBox(this, ID_SELECTIONSPEAKERKEY, _("Selection and Speaker key"));
    m_pReadAlongCheckBox = new wxCheckBox(this, ID_READALONG, _("Read Along"));
    m_pSpeedStaticText = new wxStaticText(this, wxID_ANY, _("Speed"));
    m_pSpeedSlider = new wxSlider(this, ID_SPEED, 0, -75, 75, wxDefaultPosition, wxDefaultSize, wxSL_VERTICAL | wxSL_INVERSE);

    m_pStartWithSystemCheckBox = new wxCheckBox(this, ID_STARTWITHSYSTEM, _("Start with system"));
    m_pPausedCheckBox = new wxCheckBox(this, ID_PAUSED, _("Paused"));
    m_pClassicRadioButton = new wxRadioButton(this, ID_CLASSIC, _("Classic"));
    m_pCoverRadioButton = new wxRadioButton(this, ID_COVER, _("Cover"));

    wxBoxSizer* pSoundSettingsInnerSizer = new wxBoxSizer(wxVERTICAL);
    pSoundSettingsInnerSizer->Add(m_pSoundCheckBox, wxSizerFlags().Border().Expand());
    pSoundSettingsInnerSizer->Add(m_pSoundsCheckBox, wxSizerFlags().Border().Expand());
    pSoundSettingsInnerSizer->Add(m_pTTSCheckBox, wxSizerFlags().Border().Expand());

    wxStaticBoxSizer* pSoundSettingsSizer = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Sound Settings"));
    pSoundSettingsSizer->Add(pSoundSettingsInnerSizer, wxSizerFlags().Proportion(1));
    pSoundSettingsSizer->Add(m_pVolumeStaticText, wxSizerFlags().Border().Center());
    pSoundSettingsSizer->Add(m_pVolumeSlider, wxSizerFlags().Center());

    wxBoxSizer* pTTSSettingsInnerSizer = new wxBoxSizer(wxVERTICAL);
    pTTSSettingsInnerSizer->Add(m_pActivateAfterStaticText, wxSizerFlags().Border().Expand());
    pTTSSettingsInnerSizer->Add(m_pWordCheckBox, wxSizerFlags().Border().Expand());
    pTTSSettingsInnerSizer->Add(m_pSentenceCheckBox, wxSizerFlags().Border().Expand());
    pTTSSettingsInnerSizer->Add(m_pSelectionSpeakerKeyCheckBox, wxSizerFlags().Border().Expand());
    pTTSSettingsInnerSizer->Add(m_pReadAlongCheckBox, wxSizerFlags().Border().Expand());

    wxStaticBoxSizer* pTTSSettingsSizer = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Text To Speech"));
    pTTSSettingsSizer->Add(pTTSSettingsInnerSizer, wxSizerFlags().Proportion(1));
    pTTSSettingsSizer->Add(m_pSpeedStaticText, wxSizerFlags().Border().Center());
    pTTSSettingsSizer->Add(m_pSpeedSlider, wxSizerFlags().Center());

    wxBoxSizer* pOtherSettingsFirstInnerSizer = new wxBoxSizer(wxHORIZONTAL);
    pOtherSettingsFirstInnerSizer->Add(m_pStartWithSystemCheckBox, wxSizerFlags().Border().Expand().Proportion(1));
    pOtherSettingsFirstInnerSizer->Add(m_pPausedCheckBox, wxSizerFlags().Border().Expand().Proportion(1));

    wxBoxSizer* pOtherSettingsSecondInnerSizer = new wxBoxSizer(wxHORIZONTAL);
    pOtherSettingsSecondInnerSizer->Add(m_pClassicRadioButton, wxSizerFlags().Border().Expand().Proportion(1));
    pOtherSettingsSecondInnerSizer->Add(m_pCoverRadioButton, wxSizerFlags().Border().Expand().Proportion(1));

    wxStaticBoxSizer* pOtherSettingsSizer = new wxStaticBoxSizer(wxVERTICAL, this, _("Other Settings"));
    pOtherSettingsSizer->Add(pOtherSettingsFirstInnerSizer, wxSizerFlags().Expand());
    pOtherSettingsSizer->Add(pOtherSettingsSecondInnerSizer, wxSizerFlags().Expand());

    wxSizer* pButtonSizer = CreateButtonSizer(wxCLOSE);

    wxBoxSizer* pRootSizer = new wxBoxSizer(wxVERTICAL);
    pRootSizer->Add(pSoundSettingsSizer, wxSizerFlags().DoubleBorder().Expand());
    pRootSizer->Add(pTTSSettingsSizer, wxSizerFlags().DoubleBorder().Expand());
    pRootSizer->Add(pOtherSettingsSizer, wxSizerFlags().DoubleBorder().Expand());
    pRootSizer->Add(pButtonSizer, wxSizerFlags().Border().Expand());

    SetSizerAndFit(pRootSizer);
}

bool PreferencesDialog::TransferDataToWindow()
{
    m_pSoundCheckBox->SetValue(m_pConfig->GetSound());
    m_pSoundsCheckBox->SetValue(m_pConfig->GetSounds());
    m_pTTSCheckBox->SetValue(m_pConfig->GetTTS());
    m_pVolumeSlider->SetValue(m_pConfig->GetVolume());

    m_pWordCheckBox->SetValue(m_pConfig->GetWord());
    m_pSentenceCheckBox->SetValue(m_pConfig->GetSentence());
    m_pSelectionSpeakerKeyCheckBox->SetValue(m_pConfig->GetSelection());
    m_pReadAlongCheckBox->SetValue(m_pConfig->GetReadAlong());
    m_pSpeedSlider->SetValue(m_pConfig->GetSpeed());

    m_pStartWithSystemCheckBox->SetValue(m_pConfig->GetStartWithSystem());
    m_pPausedCheckBox->SetValue(m_pConfig->GetPaused());
    m_pClassicRadioButton->SetValue(m_pConfig->GetLayout() == Layout::DutchClassic);
    m_pCoverRadioButton->SetValue(m_pConfig->GetLayout() == Layout::DutchKWeC);

    return true;
}

void PreferencesDialog::OnSoundChanged(wxCommandEvent&)
{
    m_pConfig->SetSound(m_pSoundCheckBox->GetValue());

    m_pApp->UpdateTrayIcon();
}

void PreferencesDialog::OnSoundsChanged(wxCommandEvent&)
{
    m_pConfig->SetSounds(m_pSoundsCheckBox->GetValue());

    m_pApp->UpdateTrayIcon();
}

void PreferencesDialog::OnTTSChanged(wxCommandEvent&)
{
    m_pConfig->SetTTS(m_pTTSCheckBox->GetValue());

    m_pApp->UpdateTrayIcon();
}

void PreferencesDialog::OnVolumeChanged(wxCommandEvent&)
{
    m_pConfig->SetVolume(m_pVolumeSlider->GetValue());
}

void PreferencesDialog::OnWordChanged(wxCommandEvent&)
{
    m_pConfig->SetWord(m_pWordCheckBox->GetValue());
}

void PreferencesDialog::OnSentenceChanged(wxCommandEvent&)
{
    m_pConfig->SetSentence(m_pSentenceCheckBox->GetValue());
}

void PreferencesDialog::OnSelectionSpeakerKeyChanged(wxCommandEvent&)
{
    m_pConfig->SetSelection(m_pSelectionSpeakerKeyCheckBox->GetValue());
}

void PreferencesDialog::OnReadAlongChanged(wxCommandEvent&)
{
    m_pConfig->SetReadAlong(m_pReadAlongCheckBox->GetValue());
}

void PreferencesDialog::OnSpeedChanged(wxCommandEvent&)
{
    m_pConfig->SetSpeed(m_pSpeedSlider->GetValue());
}

void PreferencesDialog::OnStartWithSystemChanged(wxCommandEvent&)
{
    m_pConfig->SetStartWithSystem(m_pStartWithSystemCheckBox->GetValue());
}

void PreferencesDialog::OnPausedChanged(wxCommandEvent&)
{
    m_pConfig->SetPaused(m_pPausedCheckBox->GetValue());

    m_pApp->UpdateTrayIcon();
}

void PreferencesDialog::OnLayoutChanged(wxCommandEvent&)
{
    m_pConfig->SetLayout(m_pClassicRadioButton->GetValue() ? Layout::DutchClassic : Layout::DutchKWeC);
}

wxBEGIN_EVENT_TABLE(PreferencesDialog, wxDialog)
    EVT_CHECKBOX(ID_SOUND, PreferencesDialog::OnSoundChanged)
    EVT_CHECKBOX(ID_SOUNDS, PreferencesDialog::OnSoundsChanged)
    EVT_CHECKBOX(ID_TTS, PreferencesDialog::OnTTSChanged)
    EVT_SLIDER(ID_VOLUME, PreferencesDialog::OnVolumeChanged)

    EVT_CHECKBOX(ID_WORD, PreferencesDialog::OnWordChanged)
    EVT_CHECKBOX(ID_SENTENCE, PreferencesDialog::OnSentenceChanged)
    EVT_CHECKBOX(ID_SELECTIONSPEAKERKEY, PreferencesDialog::OnSelectionSpeakerKeyChanged)
    EVT_CHECKBOX(ID_READALONG, PreferencesDialog::OnReadAlongChanged)
    EVT_SLIDER(ID_SPEED, PreferencesDialog::OnSpeedChanged)

    EVT_CHECKBOX(ID_STARTWITHSYSTEM, PreferencesDialog::OnStartWithSystemChanged)
    EVT_CHECKBOX(ID_PAUSED, PreferencesDialog::OnPausedChanged)
    EVT_RADIOBUTTON(ID_CLASSIC, PreferencesDialog::OnLayoutChanged)
    EVT_RADIOBUTTON(ID_COVER, PreferencesDialog::OnLayoutChanged)
wxEND_EVENT_TABLE()
