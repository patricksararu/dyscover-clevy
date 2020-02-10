//
// SoundPlayer.cpp
//

#include <wx/filename.h>
#include <wx/sound.h>

#include "ResourceLoader.h"
#include "SoundPlayer.h"

SoundPlayer::SoundPlayer()
{
    m_soundFilesPath = GetSoundFilesPath();
}

SoundPlayer::~SoundPlayer()
{
}

void SoundPlayer::PlaySoundFile(const std::string& soundfile)
{
    wxFileName filename(m_soundFilesPath, soundfile);

    wxSound::Play(filename.GetFullPath(), wxSOUND_ASYNC);
}

void SoundPlayer::StopPlaying()
{
    wxSound::Stop();
}
