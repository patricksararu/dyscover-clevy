//
// AudioLevel.cpp
//

#include "AudioLevel.h"

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <mmsystem.h>

int GetAudioVolume()
{
	DWORD dwVolume;
	MMRESULT result = waveOutGetVolume(0, &dwVolume);
	if (result != 0)
	{
		return -1;
	}

	return dwVolume & 0xFFFF;
}

bool SetAudioVolume(int value)
{
	DWORD dwVolume = MAKELONG(value, value);
	MMRESULT result = waveOutSetVolume(0, dwVolume);
	return result == 0;
}

#else // WIN32
int GetAudioVolume()
{
	return 0xFFFF;
}

bool SetAudioVolume(int value)
{
	(void)value;

	return false;
}
#endif // WIN32
