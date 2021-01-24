//
// Audio.h
//

#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <mmsystem.h>

#include "Queue.h"

class Audio
{
public:
	Audio();
	~Audio();

	bool Open(int channels, int samplerate, int samplesize);
	void Close();

	static int GetVolume();
	static bool SetVolume(int value);

	bool Write(const void* audiodata, size_t audiodatalen);
	void Stop();

private:
	HWAVEOUT m_hWaveOut;
	Queue<WAVEHDR*> m_waveHdrs;

	static void CALLBACK WaveOutCallback(HWAVEOUT, UINT, DWORD_PTR, DWORD_PTR, DWORD_PTR);
};
