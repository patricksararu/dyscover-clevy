//---------------------------------------------------------------------------

#ifndef AudioH
#define AudioH

#include <Queue.h>

//---------------------------------------------------------------------------

class Audio
{
public:
	Audio();
	~Audio();

	bool Open(int channels, int samplerate, int samplesize);
	void Close();

	int GetVolume();
	bool SetVolume(int value);

	bool Write(const void* audiodata, size_t audiodatalen);
	void Stop();

private:
	HWAVEOUT m_hWaveOut;
	Queue<WAVEHDR*> m_waveHdrs;

	static void CALLBACK WaveOutCallback(HWAVEOUT, UINT, DWORD_PTR, DWORD_PTR, DWORD_PTR);
};

#endif
