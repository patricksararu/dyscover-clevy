//---------------------------------------------------------------------------

#include <memory>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <mmsystem.h>

#pragma hdrstop

#include "Audio.h"
#include "Debug.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

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
	wavefmt.nChannels = channels;
	wavefmt.nSamplesPerSec = samplerate;
	wavefmt.nAvgBytesPerSec = channels * samplerate * samplesize;
	wavefmt.nBlockAlign = channels * samplesize;
	wavefmt.wBitsPerSample = 8 * samplesize;
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
		pWaveHdr->dwBufferLength = audiodatalen;
	}
	else
	{
		// If there are no available buffers, create a new buffer.
		pWaveHdr = (WAVEHDR*)malloc(sizeof(WAVEHDR));
		ZeroMemory(pWaveHdr, sizeof(WAVEHDR));
		pWaveHdr->lpData = (LPSTR)malloc(audiodatalen);
		pWaveHdr->dwBufferLength = audiodatalen;
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
	if (uMsg == WOM_DONE)
	{
		Audio* pThis = (Audio*)dwInstance;
		WAVEHDR* pWaveHdr = (WAVEHDR*)dwParam1;

		// Add this audio buffer to queue of available buffers.
		pThis->m_waveHdrs.Enqueue(pWaveHdr);
	}
}
