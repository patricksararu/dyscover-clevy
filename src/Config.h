//
// Config.h
//

#pragma once

#include <wx/datetime.h>
#include <wx/string.h>

#ifdef WIN32
#include <wx/msw/registry.h>
#endif //  WIN32

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

    wxDateTime GetDemoStarted();
    void SetDemoStarted(wxDateTime);

    bool GetDemoExpired();
    void SetDemoExpired(bool);

private:
    wxFileConfig* m_pConfig;
#ifdef WIN32
    wxRegKey* m_pWindowsAutostartRegistryKey;
#endif // WIN32
};
