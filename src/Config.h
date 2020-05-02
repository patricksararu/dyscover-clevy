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

    Layout GetLayout();
    void SetLayout(Layout);

    bool GetEnabled();
    void SetEnabled(bool);

    bool GetAutostart();
    void SetAutostart(bool);

    bool GetLettersAndNumbers();
    void SetLettersAndNumbers(bool);

    bool GetLetterCombinations();
    void SetLetterCombinations(bool);

    bool GetWords();
    void SetWords(bool);

    bool GetSentences();
    void SetSentences(bool);

    bool GetSelection();
    void SetSelection(bool);

    long GetSpeed();
    void SetSpeed(long);

    long GetVolume();
    void SetVolume(long);

private:
    wxFileConfig* m_pConfig;
};
