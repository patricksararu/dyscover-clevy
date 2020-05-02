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
    ID_STATUS = wxID_HIGHEST + 1,
    ID_METHOD_DYSCOVER,
    ID_METHOD_KWEC,
    ID_ENABLED,
    ID_AUTOSTART,
    ID_LETTERS_AND_NUMBERS,
    ID_LETTER_COMBINATIONS,
    ID_WORDS,
    ID_SENTENCES,
    ID_SELECTION,
    ID_VOLUME,
    ID_SPEED,
};

PreferencesDialog::PreferencesDialog(App* pApp, Config* pConfig)
    : wxDialog(nullptr, wxID_ANY, _("Preferences"))
{
    SetIcon(LoadDyscoverIcon());

    m_pApp = pApp;
    m_pConfig = pConfig;

    m_pKeyboardStatusLabel = new wxStaticText(this, ID_STATUS, _("Status:"));
    m_pKeyboardStatusValue = new wxStaticText(this, ID_STATUS, wxEmptyString);
    m_pKeyboardMethodLabel = new wxStaticText(this, wxID_ANY, _("Method:"));
    m_pKeyboardMethodDyscover = new wxRadioButton(this, ID_METHOD_DYSCOVER, _("Dyscover"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP);
    m_pKeyboardMethodKWeC = new wxRadioButton(this, ID_METHOD_KWEC, _("KWeC"));

    m_pSoftwareEnabled = new wxCheckBox(this, ID_ENABLED, _("Software enabled"));
    m_pSoftwareAutostart = new wxCheckBox(this, ID_AUTOSTART, _("Auto-start with system"));

    m_pSoundLettersAndNumbers = new wxCheckBox(this, ID_LETTERS_AND_NUMBERS, _("Letters and numbers"));
    m_pSoundLetterCombinations = new wxCheckBox(this, ID_LETTER_COMBINATIONS, _("Letter combinations"));
    m_pSoundWords = new wxCheckBox(this, ID_WORDS, _("Words"));
    m_pSoundSentences = new wxCheckBox(this, ID_SENTENCES, _("Sentences"));
    m_pSoundSelection = new wxCheckBox(this, ID_SELECTION, _("Selection"));
    m_pSoundVolumeLabel = new wxStaticText(this, wxID_ANY, _("Volume"));
    m_pSoundVolume = new wxSlider(this, ID_VOLUME, 100, 0, 100);
    m_pSoundSpeedLabel = new wxStaticText(this, wxID_ANY, _("Speed"));
    m_pSoundSpeed = new wxSlider(this, ID_SPEED, 0, -75, 75);

    wxBoxSizer* pKeyboardStatusSizer = new wxBoxSizer(wxHORIZONTAL);
    pKeyboardStatusSizer->Add(m_pKeyboardStatusLabel, wxSizerFlags().Border(wxRIGHT));
    pKeyboardStatusSizer->Add(m_pKeyboardStatusValue, wxSizerFlags().Expand());

    wxBoxSizer* pKeyboardMethodSizer = new wxBoxSizer(wxHORIZONTAL);
    pKeyboardMethodSizer->Add(m_pKeyboardMethodLabel, wxSizerFlags().Border(wxRIGHT));
    pKeyboardMethodSizer->Add(m_pKeyboardMethodDyscover, wxSizerFlags().Proportion(1));
    pKeyboardMethodSizer->Add(m_pKeyboardMethodKWeC, wxSizerFlags().Proportion(1));

    wxStaticBoxSizer* pKeyboardSectionSizer = new wxStaticBoxSizer(wxVERTICAL, this, _("Keyboard"));
    pKeyboardSectionSizer->Add(pKeyboardStatusSizer, wxSizerFlags().Border().Expand());
    pKeyboardSectionSizer->Add(pKeyboardMethodSizer, wxSizerFlags().Border().Expand());

    wxStaticBoxSizer* pGeneralSectionSizer = new wxStaticBoxSizer(wxVERTICAL, this, _("General"));
    pGeneralSectionSizer->Add(m_pSoftwareEnabled, wxSizerFlags().Border().Expand());
    pGeneralSectionSizer->Add(m_pSoftwareAutostart, wxSizerFlags().Border().Expand());

    wxStaticBoxSizer* pSoundSectionSizer = new wxStaticBoxSizer(wxVERTICAL, this, _("Sound"));
    pSoundSectionSizer->Add(m_pSoundLettersAndNumbers, wxSizerFlags().Border().Expand());
    pSoundSectionSizer->Add(m_pSoundLetterCombinations, wxSizerFlags().Border().Expand());
    pSoundSectionSizer->Add(m_pSoundWords, wxSizerFlags().Border().Expand());
    pSoundSectionSizer->Add(m_pSoundSentences, wxSizerFlags().Border().Expand());
    pSoundSectionSizer->Add(m_pSoundSelection, wxSizerFlags().Border().Expand());
    pSoundSectionSizer->AddSpacer(10);
    pSoundSectionSizer->Add(m_pSoundVolumeLabel, wxSizerFlags().Border(wxLEFT | wxRIGHT).Expand());
    pSoundSectionSizer->Add(m_pSoundVolume, wxSizerFlags().Expand());
    pSoundSectionSizer->AddSpacer(10);
    pSoundSectionSizer->Add(m_pSoundSpeedLabel, wxSizerFlags().Border(wxLEFT | wxRIGHT).Expand());
    pSoundSectionSizer->Add(m_pSoundSpeed, wxSizerFlags().Border(wxBOTTOM).Expand());

    wxBoxSizer* pRootSizer = new wxBoxSizer(wxVERTICAL);
    pRootSizer->Add(pKeyboardSectionSizer, wxSizerFlags().DoubleBorder().Expand());
    pRootSizer->Add(pGeneralSectionSizer, wxSizerFlags().DoubleHorzBorder().Expand());
    pRootSizer->Add(pSoundSectionSizer, wxSizerFlags().DoubleBorder().Expand());

    SetSizerAndFit(pRootSizer);
}

bool PreferencesDialog::TransferDataToWindow()
{
    if (m_pApp->IsClevyKeyboardPresent())
    {
        m_pKeyboardStatusValue->SetLabelText(_("Keyboard connected"));
    }
    else
    {
        m_pKeyboardStatusValue->SetLabelText(_("Keyboard not connected"));
    }

    m_pKeyboardMethodDyscover->SetValue(m_pConfig->GetLayout() == Layout::DutchClassic);
    m_pKeyboardMethodKWeC->SetValue(m_pConfig->GetLayout() == Layout::DutchKWeC);

    m_pSoftwareEnabled->SetValue(m_pConfig->GetEnabled());
    m_pSoftwareAutostart->SetValue(m_pConfig->GetAutostart());

    m_pSoundLettersAndNumbers->SetValue(m_pConfig->GetLettersAndNumbers());
    m_pSoundLetterCombinations->SetValue(m_pConfig->GetLetterCombinations());
    m_pSoundWords->SetValue(m_pConfig->GetWords());
    m_pSoundSentences->SetValue(m_pConfig->GetSentences());
    m_pSoundSelection->SetValue(m_pConfig->GetSelection());
    m_pSoundVolume->SetValue(m_pConfig->GetVolume());
    m_pSoundSpeed->SetValue(m_pConfig->GetSpeed());

    return true;
}

void PreferencesDialog::OnKeyboardMethodChanged(wxCommandEvent&)
{
    m_pConfig->SetLayout(m_pKeyboardMethodDyscover->GetValue() ? Layout::DutchClassic : Layout::DutchKWeC);
}

void PreferencesDialog::OnSoftwareEnabledChanged(wxCommandEvent&)
{
    m_pConfig->SetEnabled(m_pSoftwareEnabled->GetValue());

    m_pApp->UpdateTrayIcon();
}

void PreferencesDialog::OnSoftwareAutoStartChanged(wxCommandEvent&)
{
    m_pConfig->SetAutostart(m_pSoftwareAutostart->GetValue());
}

void PreferencesDialog::OnSoundLettersAndNumbersChanged(wxCommandEvent&)
{
    m_pConfig->SetLettersAndNumbers(m_pSoundLettersAndNumbers->GetValue());
}

void PreferencesDialog::OnSoundLetterCombinationsChanged(wxCommandEvent&)
{
    m_pConfig->SetLetterCombinations(m_pSoundLetterCombinations->GetValue());
}

void PreferencesDialog::OnSoundWordsChanged(wxCommandEvent&)
{
    m_pConfig->SetWords(m_pSoundWords->GetValue());
}

void PreferencesDialog::OnSoundSentencesChanged(wxCommandEvent&)
{
    m_pConfig->SetSentences(m_pSoundSentences->GetValue());
}

void PreferencesDialog::OnSoundSelectionChanged(wxCommandEvent&)
{
    m_pConfig->SetSelection(m_pSoundSelection->GetValue());
}

void PreferencesDialog::OnSoundVolumeChanged(wxCommandEvent&)
{
    m_pConfig->SetVolume(m_pSoundVolume->GetValue());

    m_pApp->UpdateAudioVolume();
}

void PreferencesDialog::OnSoundSpeedChanged(wxCommandEvent&)
{
    m_pConfig->SetSpeed(m_pSoundSpeed->GetValue());
}

wxBEGIN_EVENT_TABLE(PreferencesDialog, wxDialog)
    EVT_RADIOBUTTON(ID_METHOD_DYSCOVER, PreferencesDialog::OnKeyboardMethodChanged)
    EVT_RADIOBUTTON(ID_METHOD_KWEC, PreferencesDialog::OnKeyboardMethodChanged)

    EVT_CHECKBOX(ID_ENABLED, PreferencesDialog::OnSoftwareEnabledChanged)
    EVT_CHECKBOX(ID_AUTOSTART, PreferencesDialog::OnSoftwareAutoStartChanged)

    EVT_CHECKBOX(ID_LETTERS_AND_NUMBERS, PreferencesDialog::OnSoundLettersAndNumbersChanged)
    EVT_CHECKBOX(ID_LETTER_COMBINATIONS, PreferencesDialog::OnSoundLetterCombinationsChanged)
    EVT_CHECKBOX(ID_WORDS, PreferencesDialog::OnSoundWordsChanged)
    EVT_CHECKBOX(ID_SENTENCES, PreferencesDialog::OnSoundSentencesChanged)
    EVT_CHECKBOX(ID_SELECTION, PreferencesDialog::OnSoundSelectionChanged)
    EVT_SLIDER(ID_VOLUME, PreferencesDialog::OnSoundVolumeChanged)
    EVT_SLIDER(ID_SPEED, PreferencesDialog::OnSoundSpeedChanged)
wxEND_EVENT_TABLE()
