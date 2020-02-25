//
// ResourceLoader.h
//

#pragma once

#include <wx/icon.h>
#include <wx/iconbndl.h>

wxIcon LoadIcon(const wxString&);
wxIcon LoadDyscoverIcon();
wxIconArray LoadDyscoverIcons();

wxString GetExecutablePath();
wxString GetSoundFilesPath();
wxString GetTTSDataPath();
wxString GetTranslationsPath();
