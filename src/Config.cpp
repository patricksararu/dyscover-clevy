//
// Config.cpp
//

#include <wx/fileconf.h>

#include "Config.h"

static const wxString kLayoutKey("/Dyscover/Layout");
static const wxString kEnabledKey("/Dyscover/Enabled");
static const wxString kAutostartKey("/Dyscover/Autostart");
static const wxString kLettersAndNumbersKey("/Dyscover/LettersAndNumbers");
static const wxString kLetterCombinationsKey("/Dyscover/LetterCombinations");
static const wxString kWordsKey("/Dyscover/Words");
static const wxString kSentencesKey("/Dyscover/Sentences");
static const wxString kSelectionKey("/Dyscover/Selection");
static const wxString kVolumeKey("/Dyscover/Volume");
static const wxString kSpeedKey("/Dyscover/Speed");

static constexpr Layout kLayoutDefaultValue = Layout::DutchClassic;
static constexpr bool kEnabledDefaultValue = true;
static constexpr bool kAutostartDefaultValue = false;
static constexpr bool kLettersAndNumbersDefaultValue = true;
static constexpr bool kLetterCombinationsDefaultValue = true;
static constexpr bool kWordsDefaultValue = true;
static constexpr bool kSentencesDefaultValue = true;
static constexpr bool kSelectionDefaultValue = true;
static constexpr long kVolumeDefaultValue = 100;
static constexpr long kSpeedDefaultValue = 0;

static const wxString kLayoutValueClassic("Classic");
static const wxString kLayoutValueKWeC("Cover");

Config::Config()
{
    m_pConfig = new wxFileConfig("Dyscover", "Clevy", "ClevyDyscover.ini");
}

Config::~Config()
{
    delete m_pConfig;
}

Layout Config::GetLayout()
{
    return m_pConfig->ReadObject<Layout>(kLayoutKey, kLayoutDefaultValue);
}

void Config::SetLayout(Layout value)
{
    m_pConfig->Write(kLayoutKey, value);
}

bool Config::GetEnabled()
{
    return m_pConfig->ReadBool(kEnabledKey, kEnabledDefaultValue);
}

void Config::SetEnabled(bool value)
{
    m_pConfig->Write(kEnabledKey, value);
}

bool Config::GetAutostart()
{
    return m_pConfig->ReadBool(kAutostartKey, kAutostartDefaultValue);
}

void Config::SetAutostart(bool value)
{
    m_pConfig->Write(kAutostartKey, value);
}

bool Config::GetLettersAndNumbers()
{
    return m_pConfig->ReadBool(kLettersAndNumbersKey, kLettersAndNumbersDefaultValue);
}

void Config::SetLettersAndNumbers(bool value)
{
    m_pConfig->Write(kLettersAndNumbersKey, value);
}

bool Config::GetLetterCombinations()
{
    return m_pConfig->ReadBool(kLetterCombinationsKey, kLetterCombinationsDefaultValue);
}

void Config::SetLetterCombinations(bool value)
{
    m_pConfig->Write(kLetterCombinationsKey, value);
}

bool Config::GetWords()
{
    return m_pConfig->ReadBool(kWordsKey, kWordsDefaultValue);
}

void Config::SetWords(bool value)
{
    m_pConfig->Write(kWordsKey, value);
}

bool Config::GetSentences()
{
    return m_pConfig->ReadBool(kSentencesKey, kSentencesDefaultValue);
}

void Config::SetSentences(bool value)
{
    m_pConfig->Write(kSentencesKey, value);
}

bool Config::GetSelection()
{
    return m_pConfig->ReadBool(kSelectionKey, kSelectionDefaultValue);
}

void Config::SetSelection(bool value)
{
    m_pConfig->Write(kSelectionKey, value);
}

long Config::GetVolume()
{
    return m_pConfig->ReadLong(kVolumeKey, kVolumeDefaultValue);
}

void Config::SetVolume(long value)
{
    m_pConfig->Write(kVolumeKey, value);
}

long Config::GetSpeed()
{
    return m_pConfig->ReadLong(kSpeedKey, kSpeedDefaultValue);
}

void Config::SetSpeed(long value)
{
    m_pConfig->Write(kSpeedKey, value);
}

bool wxFromString(const wxString& string, Layout* pLayout)
{
    if (string == kLayoutValueClassic)
    {
        *pLayout = Layout::DutchClassic;
        return true;
    }

    if (string == kLayoutValueKWeC)
    {
        *pLayout = Layout::DutchKWeC;
        return true;
    }

    return false;
}

wxString wxToString(const Layout& layout)
{
    switch (layout)
    {
    case Layout::DutchClassic:
        return kLayoutValueClassic;
    case Layout::DutchKWeC:
        return kLayoutValueKWeC;
    default:
        return wxEmptyString;
    }
}
