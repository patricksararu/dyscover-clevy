// ---------------------------------------------------------------------------

#pragma hdrstop

#include "WaveOut.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// This is the callback function that the wave out API will call when
// it has played some audio.
// Increments our free block counter when we're done with a block.
void CALLBACK wave_out_callback(HWAVEOUT hWaveOut, UINT uMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2)
{
	// Cast back our user data to our own callback structure
	struct audio_state *audiostate = (struct audio_state *)dwInstance;
	switch (uMsg)
	{
	case WOM_DONE:
		// One block finished, increase free blocks in the queue
		InterlockedIncrement(&audiostate->blocks_free);
		break;
	default:
		// Ignore other messages
		break;
	}
}

// This sets up our audio device and audio queue.
// Call before playing any audio.
// Returns TRUE on success, FALSE on failure.
int audio_init(struct audio_state *audiostate, size_t num_blocks, long samplerate_hz)
{
	// Safe initial values, so we don't leave the structure with dangling
	// pointers/handles on failure
	audiostate->blocks = nullptr;
	audiostate->dev    = nullptr; // INVALID_HANDLE_VALUE;

	// Set up audio
	const char sample_size = sizeof(short);
	WAVEFORMATEX waveformat;
	memset(&waveformat, 0, sizeof(waveformat));
	waveformat.wFormatTag      = WAVE_FORMAT_PCM;
	waveformat.nChannels       = 1;
	waveformat.nSamplesPerSec  = samplerate_hz;
	waveformat.nAvgBytesPerSec = sample_size * samplerate_hz;
	waveformat.nBlockAlign     = sample_size;
	waveformat.wBitsPerSample  = sample_size * 8;
	waveformat.cbSize          = 0;
	// Pass our audio callback and callback data as we set up audio
	MMRESULT mmres = waveOutOpen(&audiostate->dev, WAVE_MAPPER, &waveformat, (DWORD_PTR) & wave_out_callback, (DWORD_PTR)audiostate, CALLBACK_FUNCTION);
	if (mmres != MMSYSERR_NOERROR)
		return FALSE;

	// Set up our audio queue. All blocks start as cleared.
	audiostate->blocks = (WAVEHDR *) malloc(num_blocks *sizeof(WAVEHDR));
	if (audiostate->blocks == nullptr)
	{
		waveOutClose(audiostate->dev);
		audiostate->dev = nullptr; // INVALID_HANDLE_VALUE;
		return FALSE;           // malloc failed, give up
	}
	audiostate->num_blocks = num_blocks;
	audiostate->blocks_free = num_blocks;
	audiostate->next_block  = 0;
	memset(audiostate->blocks, 0, num_blocks *sizeof(WAVEHDR));
	return TRUE; // success
}

// This uninitializes our audio device and audio queue.
// Call when we're done.
// Will wait for any audio in the queue to play to completion.
void audio_finish(struct audio_state *audiostate)
{
	// Wait for all enqueued data to be played
	while (audiostate->blocks_free < audiostate->num_blocks)
		Sleep(50);

	// Uninitialize each block as needed, and free buffers
	int i;
	for (i = 0; i < audiostate->num_blocks; ++i)
	{
		WAVEHDR *block = &audiostate->blocks[i];
		if (block->dwFlags &WHDR_PREPARED)
			waveOutUnprepareHeader(audiostate->dev, block, sizeof(WAVEHDR));
		if (block->lpData)
			free(block->lpData);
	}

	// Close audio device
	waveOutClose(audiostate->dev);

	// Free and clear
	free(audiostate->blocks);
	audiostate->blocks      = nullptr;
	audiostate->num_blocks  = 0;
	audiostate->blocks_free = 0;
	audiostate->dev         = nullptr; // INVALID_HANDLE_VALUE;
}
