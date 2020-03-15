//
// Audio.h
//

#pragma once

#include <portaudio.h>

class Audio
{
public:
	Audio();
	~Audio();

	bool Open(int channels, int samplerate, PaSampleFormat sampleformat);
	void Close();

	bool Write(const void* audiodata, unsigned long audiodatalen);
	void Stop();

private:
	PaStream* m_pStream;
};
