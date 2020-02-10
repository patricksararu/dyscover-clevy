//
// SoundPlayer.h
//

#pragma once

#include <string>

class SoundPlayer
{
public:
	SoundPlayer();
	~SoundPlayer();

	void PlaySoundFile(const std::string& filename);
	void StopPlaying();

private:
	std::string m_soundFilesPath;
};
