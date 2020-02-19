//
// ResourceLoader.cpp
//

#include <wx/filename.h>
#include <wx/stdpaths.h>

#include "ResourceLoader.h"

static const wxString kSoundFilesPath("audio");
static const wxString kTTSDataPath("tts");
static const wxString kTranslationsPath("lang");

wxIcon LoadIcon(const wxString& name)
{
#ifdef __WINDOWS__
    return wxIcon(name, wxBITMAP_TYPE_ICO_RESOURCE);
#elif defined __APPLE__
    wxIcon icon(name + ".icns", wxBITMAP_TYPE_ICON);
#else
    wxIcon icon(name + ".ico", wxBITMAP_TYPE_ICO);
#endif
}

wxIcon LoadDyscoverIcon()
{
    return LoadIcon("Icon");
}

wxIconArray LoadDyscoverIcons()
{
    wxIconArray icons;

    for (int i = 0; i <= 6; i++)
    {
        icons.Add(LoadIcon(wxString::Format("Icon%d", i)));
    }

    return icons;
}

wxBitmap LoadSplashBitmap()
{
    return wxBitmap("Splash", wxBITMAP_TYPE_BMP_RESOURCE);
}

wxString GetExecutablePath()
{
    wxFileName filename(wxStandardPaths::Get().GetExecutablePath());
    return filename.GetPath();
}

wxString GetSoundFilesPath()
{
    wxFileName filename(GetExecutablePath(), kSoundFilesPath);
    return filename.GetFullPath();
}

wxString GetTTSDataPath()
{
    wxFileName filename(GetExecutablePath(), kTTSDataPath);
    return filename.GetFullPath();
}

wxString GetTranslationsPath()
{
    wxFileName filename(GetExecutablePath(), kTranslationsPath);
    return filename.GetFullPath();
}
