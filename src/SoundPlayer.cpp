//
// SoundPlayer.cpp
//

#include <wx/filename.h>
#include <wx/sound.h>

#include "SoundPlayer.h"

static const std::string g_sSoundFilesPath = "data";

SoundPlayer::SoundPlayer()
{
}

SoundPlayer::~SoundPlayer()
{
}

void SoundPlayer::PlaySoundFile(const std::string& soundfile)
{
    wxFileName filename(g_sSoundFilesPath, soundfile);

    wxSound::Play(filename.GetFullPath(), wxSOUND_ASYNC);
}

void SoundPlayer::StopPlaying()
{
    wxSound::Stop();
}
