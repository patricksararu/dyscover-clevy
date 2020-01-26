//
// Config.cpp
//

#include <wx/fileconf.h>

#include "Config.h"

static const wxString kSoundKey("/Dyscover/Sound");
static const wxString kSoundsKey("/Dyscover/Sounds");
static const wxString kTTSKey("/Dyscover/TTS");
static const wxString kVolumeKey("/Dyscover/Volume");
static const wxString kWordKey("/Dyscover/Word");
static const wxString kSelectionKey("/Dyscover/Selection");
static const wxString kReadAlongKey("/Dyscover/ReadAlong");
static const wxString kSpeedKey("/Dyscover/Speed");
static const wxString kStartWithSystemKey("/Dyscover/StartWithSystem");
static const wxString kPausedKey("/Dyscover/Paused");
static const wxString kLayoutKey("/Dyscover/Layout");

static constexpr bool kSoundDefaultValue = true;
static constexpr bool kSoundsDefaultValue = true;
static constexpr bool kTTSDefaultValue = true;
static constexpr long kVolumeDefaultValue = 100;
static constexpr bool kWordDefaultValue = true;
static constexpr bool kSelectionDefaultValue = true;
static constexpr bool kReadAlongDefaultValue = true;
static constexpr long kSpeedDefaultValue = 0;
static constexpr bool kStartWithSystemDefaultValue = false;
static constexpr bool kPausedDefaultValue = false;
static constexpr Layout kLayoutDefaultValue = Layout::DutchClassic;

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

bool Config::GetSound()
{
    return m_pConfig->ReadBool(kSoundKey, kSoundDefaultValue);
}

void Config::SetSound(bool sound)
{
    m_pConfig->Write(kSoundKey, sound);
}

bool Config::GetSounds()
{
    return m_pConfig->ReadBool(kSoundsKey, kSoundsDefaultValue);
}

void Config::SetSounds(bool sounds)
{
    m_pConfig->Write(kSoundsKey, sounds);
}

bool Config::GetTTS()
{
    return m_pConfig->ReadBool(kTTSKey, kTTSDefaultValue);
}

void Config::SetTTS(bool tts)
{
    m_pConfig->Write(kTTSKey, tts);
}

long Config::GetVolume()
{
    return m_pConfig->ReadLong(kVolumeKey, kVolumeDefaultValue);
}

void Config::SetVolume(long volume)
{
    m_pConfig->Write(kVolumeKey, volume);
}

bool Config::GetWord()
{
    return m_pConfig->ReadBool(kWordKey, kWordDefaultValue);
}

void Config::SetWord(bool word)
{
    m_pConfig->Write(kWordKey, word);
}

bool Config::GetSelection()
{
    return m_pConfig->ReadBool(kSelectionKey, kSelectionDefaultValue);
}

void Config::SetSelection(bool selection)
{
    m_pConfig->Write(kSelectionKey, selection);
}

bool Config::GetReadAlong()
{
    return m_pConfig->ReadBool(kReadAlongKey, kReadAlongDefaultValue);
}

void Config::SetReadAlong(bool readalong)
{
    m_pConfig->Write(kReadAlongKey, readalong);
}

long Config::GetSpeed()
{
    return m_pConfig->ReadLong(kSpeedKey, kSpeedDefaultValue);
}

void Config::SetSpeed(long speed)
{
    m_pConfig->Write(kSpeedKey, speed);
}

bool Config::GetStartWithSystem()
{
    return m_pConfig->ReadBool(kStartWithSystemKey, kStartWithSystemDefaultValue);
}

void Config::SetStartWithSystem(bool startWithSystem)
{
    m_pConfig->Write(kStartWithSystemKey, startWithSystem);
}

bool Config::GetPaused()
{
    return m_pConfig->ReadBool(kPausedKey, kPausedDefaultValue);
}

void Config::SetPaused(bool paused)
{
    m_pConfig->Write(kPausedKey, paused);
}

Layout Config::GetLayout()
{
    return m_pConfig->ReadObject<Layout>(kLayoutKey, kLayoutDefaultValue);
}

void Config::SetLayout(Layout layout)
{
    m_pConfig->Write(kLayoutKey, layout);
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
