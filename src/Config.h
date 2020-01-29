//
// Config.h
//

#pragma once

#include <wx/string.h>

class wxFileConfig;  // TODO: Replace with #include <wx/fileconf.h>

enum class Layout
{
    DutchClassic,
    DutchKWeC,
};

bool wxFromString(const wxString& string, Layout* pLayout);
wxString wxToString(const Layout& layout);

class Config
{
public:
    Config();
    ~Config();

    bool GetSound();
    void SetSound(bool);

    bool GetSounds();
    void SetSounds(bool);

    bool GetTTS();
    void SetTTS(bool);

    long GetVolume();
    void SetVolume(long);

    bool GetWord();
    void SetWord(bool);

    bool GetSelection();
    void SetSelection(bool);

    bool GetReadAlong();
    void SetReadAlong(bool);

    long GetSpeed();
    void SetSpeed(long);

    bool GetStartWithSystem();
    void SetStartWithSystem(bool);

    bool GetPaused();
    void SetPaused(bool);

    Layout GetLayout();
    void SetLayout(Layout layout);

private:
    wxFileConfig* m_pConfig;
};
