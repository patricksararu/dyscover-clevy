//
// ResourceLoader.h
//

#pragma once

#include <wx/bitmap.h>
#include <wx/icon.h>
#include <wx/iconbndl.h>

wxIcon LoadIcon(const wxString&);
wxIcon LoadDyscoverIcon();
wxIconArray LoadDyscoverIcons();
wxBitmap LoadSplashBitmap();

wxString GetExecutablePath();
wxString GetSoundFilesPath();
wxString GetTTSDataPath();
wxString GetTranslationsPath();
