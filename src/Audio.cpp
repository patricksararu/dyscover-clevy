//
// Audio.cpp
//

#include <memory>

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#include "Audio.h"
#include "Debug.h"

#ifdef  __BORLANDC__
#pragma package(smart_init)
#endif

Audio::Audio() : m_hWaveOut(0)
{
}

Audio::~Audio()
{
	Close();
}

bool Audio::Open(int channels, int samplerate, int samplesize)
{
	WAVEFORMATEX wavefmt;
	ZeroMemory(&wavefmt, sizeof(WAVEFORMATEX));
	wavefmt.wFormatTag = WAVE_FORMAT_PCM;
	wavefmt.nChannels = static_cast<WORD>(channels);
	wavefmt.nSamplesPerSec = static_cast<WORD>(samplerate);
	wavefmt.nAvgBytesPerSec = channels * samplerate * samplesize;
	wavefmt.nBlockAlign = static_cast<WORD>(channels * samplesize);
	wavefmt.wBitsPerSample = static_cast<WORD>(8 * samplesize);
	wavefmt.cbSize = 0;

	MMRESULT result = waveOutOpen(&m_hWaveOut, WAVE_MAPPER, &wavefmt, (DWORD_PTR)WaveOutCallback, (DWORD_PTR)this, CALLBACK_FUNCTION);
	return result == 0;
}

void Audio::Close()
{
	waveOutReset(m_hWaveOut);

	// TODO: Wait for all buffers to finish playing.

	// Delete all buffers.
	WAVEHDR* pWaveHdr;
	while (m_waveHdrs.TryDequeue(pWaveHdr))
	{
		waveOutUnprepareHeader(m_hWaveOut, pWaveHdr, sizeof(WAVEHDR));

		free(pWaveHdr->lpData);

		free(pWaveHdr);
	}

	waveOutClose(m_hWaveOut);
}

int Audio::GetVolume()
{
	DWORD dwVolume;
	MMRESULT result = waveOutGetVolume(m_hWaveOut, &dwVolume);
	if (result != 0)
	{
		return -1;
	}

	return dwVolume & 0xFFFF;
}

bool Audio::SetVolume(int value)
{
	DWORD dwVolume = MAKELONG(value, value);
	MMRESULT result = waveOutSetVolume(m_hWaveOut, dwVolume);
	return result == 0;
}

bool Audio::Write(const void* audiodata, size_t audiodatalen)
{
	// Take an existing buffer from queue of available buffers and reuse it.
	WAVEHDR* pWaveHdr;
	if (m_waveHdrs.TryDequeue(pWaveHdr))
	{
		// Unprepare it.
		waveOutUnprepareHeader(m_hWaveOut, pWaveHdr, sizeof(WAVEHDR));

		// Resize it.
		pWaveHdr->lpData = (LPSTR)realloc(pWaveHdr->lpData, audiodatalen);
		pWaveHdr->dwBufferLength = static_cast<DWORD>(audiodatalen);
	}
	else
	{
		// If there are no available buffers, create a new buffer.
		pWaveHdr = (WAVEHDR*)malloc(sizeof(WAVEHDR));
		ZeroMemory(pWaveHdr, sizeof(WAVEHDR));
		pWaveHdr->lpData = (LPSTR)malloc(audiodatalen);
		pWaveHdr->dwBufferLength = static_cast<DWORD>(audiodatalen);
	}

	if (pWaveHdr->lpData == nullptr)
	{
		// We're out of memory. Clean up and signalize error to caller.
		free(pWaveHdr);
		return false;
	}

	memcpy(pWaveHdr->lpData, audiodata, audiodatalen);

	waveOutPrepareHeader(m_hWaveOut, pWaveHdr, sizeof(WAVEHDR));

	waveOutWrite(m_hWaveOut, pWaveHdr, sizeof(WAVEHDR));

	return true;
}

void Audio::Stop()
{
	waveOutReset(m_hWaveOut);
}

void CALLBACK Audio::WaveOutCallback(HWAVEOUT hWaveOut, UINT uMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2)
{
	(void)hWaveOut;
	(void)dwParam2;

	if (uMsg == WOM_DONE)
	{
		Audio* pThis = (Audio*)dwInstance;
		WAVEHDR* pWaveHdr = (WAVEHDR*)dwParam1;

		// Add this audio buffer to queue of available buffers.
		pThis->m_waveHdrs.Enqueue(pWaveHdr);
	}
}
