//
// Config.cpp
//

#include <wx/fileconf.h>

#ifdef WIN32
#include <wx/stdpaths.h>
#endif // WIN32

#include "Config.h"

static const wxString kLayoutKey("/Dyscover/Layout");
static const wxString kEnabledKey("/Dyscover/Enabled");
static const wxString kAutostartKey("/Dyscover/Autostart");
static const wxString kLettersAndNumbersKey("/Dyscover/LettersAndNumbers");
static const wxString kLetterCombinationsKey("/Dyscover/LetterCombinations");
static const wxString kWordsKey("/Dyscover/Words");
static const wxString kSentencesKey("/Dyscover/Sentences");
static const wxString kSelectionKey("/Dyscover/Selection");
static const wxString kSpeedKey("/Dyscover/Speed");
static const wxString kDemoStartedKey("/Dyscover/DemoStarted");
static const wxString kDemoExpiredKey("/Dyscover/DemoExpired");

static constexpr Layout kLayoutDefaultValue = Layout::DutchClassic;
static constexpr bool kEnabledDefaultValue = true;
static constexpr bool kAutostartDefaultValue = false;
static constexpr bool kLettersAndNumbersDefaultValue = true;
static constexpr bool kLetterCombinationsDefaultValue = true;
static constexpr bool kWordsDefaultValue = true;
static constexpr bool kSentencesDefaultValue = true;
static constexpr bool kSelectionDefaultValue = true;
static constexpr long kSpeedDefaultValue = 0;
static const wxDateTime kDemoStartedDefaultValue;
static constexpr bool kDemoExpiredDefaultValue = false;

static const wxString kLayoutValueClassic("Classic");
static const wxString kLayoutValueKWeC("Cover");

static const wxString kWindowsRegistryAutostartKeyName("ClevyDyscover4");

Config::Config()
{
    m_pConfig = new wxFileConfig("Dyscover", "Clevy", "ClevyDyscover.ini");
#ifdef WIN32
    m_pWindowsAutostartRegistryKey = new wxRegKey(wxRegKey::HKCU, "Software\\Microsoft\\Windows\\CurrentVersion\\Run");
#endif // WIN32
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
#ifdef WIN32
    return m_pWindowsAutostartRegistryKey->HasValue(kWindowsRegistryAutostartKeyName);
#else
    return m_pConfig->ReadBool(kAutostartKey, kAutostartDefaultValue);
#endif // WIN32
}

void Config::SetAutostart(bool value)
{
#ifdef WIN32
    if (value)
    {
        m_pWindowsAutostartRegistryKey->SetValue(kWindowsRegistryAutostartKeyName, wxStandardPaths::Get().GetExecutablePath());
    }
    else
    {
        m_pWindowsAutostartRegistryKey->DeleteValue(kWindowsRegistryAutostartKeyName);
    }
#else
    m_pConfig->Write(kAutostartKey, value);
#endif // WIN32
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

long Config::GetSpeed()
{
    return m_pConfig->ReadLong(kSpeedKey, kSpeedDefaultValue);
}

void Config::SetSpeed(long value)
{
    m_pConfig->Write(kSpeedKey, value);
}

wxDateTime Config::GetDemoStarted()
{
    return m_pConfig->ReadObject<wxDateTime>(kDemoStartedKey, kDemoStartedDefaultValue);
}

void Config::SetDemoStarted(wxDateTime value)
{
    m_pConfig->Write(kDemoStartedKey, value);
}

bool Config::GetDemoExpired()
{
    return m_pConfig->ReadBool(kDemoExpiredKey, kDemoExpiredDefaultValue);
}

void Config::SetDemoExpired(bool value)
{
    m_pConfig->Write(kDemoExpiredKey, value);
}

bool wxFromString(const wxString& string, wxDateTime* pDateTime)
{
    wxDateTime datetime;
    if (datetime.ParseISOCombined(string))
    {
        *pDateTime = datetime;
        return true;
    }

    return false;
}

wxString wxToString(const wxDateTime& datetime)
{
    return datetime.FormatISOCombined();
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
