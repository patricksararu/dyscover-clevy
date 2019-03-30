// ---------------------------------------------------------------------------
#ifndef WaveOutH
#define WaveOutH
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
#include <Windows.h>
#include <mmsystem.h>
// ---------------------------------------------------------------------------

// This is the audio state that we use with our callbacks both from the
// TTS engine and for the Windows audio (waveout) API.
// Can be expanded to contain any extra data that we like.
struct audio_state
{
	WAVEHDR *     blocks;      // audio queue blocks
	size_t        num_blocks;  // allocated size of blocks[] (if non-nullptr)
	volatile long blocks_free; // free blocks in queue, use Interlocked ops
	long          next_block;  // index into blocks[]
	HWAVEOUT      dev;         // audio device or INVALID_HANDLE_VALUE
};

// This is the callback function that the wave out API will call when
// it has played some audio.
// Increments our free block counter when we're done with a block.
void CALLBACK wave_out_callback(HWAVEOUT hWaveOut, UINT uMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2);

// This sets up our audio device and audio queue.
// Call before playing any audio.
// Returns TRUE on success, FALSE on failure.
int audio_init(struct audio_state *audiostate, size_t num_blocks, long samplerate_hz);

// This uninitializes our audio device and audio queue.
// Call when we're done.
// Will wait for any audio in the queue to play to completion.
void audio_finish(struct audio_state *audiostate);

#endif
