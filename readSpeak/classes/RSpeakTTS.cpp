// ---------------------------------------------------------------------------
#pragma hdrstop
// ---------------------------------------------------------------------------
#include "RSpeakTTS.h"
// ---------------------------------------------------------------------------
#include <System.SysUtils.hpp>
#include <thread>
#include <mutex>
// #include "model/StringConversions.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
// ---------------------------------------------------------------------------
std::mutex g_ttsmutex;

namespace rstts
{
	/* rSpeak example console program for Microsoft Windows, speaks text
	 * from the command line.
	 * call with speak_console language[:voice] "text"
	 *
	 * Build as a console program with unicode support
	 * (if building with gcc: use -municode)
	 */

	// This is our callback function that the TTS will call when it has
	// audio for us.
	// userptr is the pointer that we gave when we set up the callback.
	void TRSpeakTTS::tts_audio_callback(RSTTSInst tts, const void *audiodata, size_t audiodatalen, void *userptr)
	{
		// std::lock_guard < std::mutex > guard(g_ttsmutex);

		// Cast back our user data to our own callback structure
		struct audio_state *audiostate = (struct audio_state *)userptr;

		if (userptr == nullptr || audiostate == nullptr)
			return;

		// Wait for one of the blocks to be free
		while (audiostate->blocks_free == 0)
			Sleep(10);

		// Get a block (and increase the next block counter).
		// We (re-)use them cyclically so we can always just get the next one.
		InterlockedDecrement(&audiostate->blocks_free);
		WAVEHDR *block         = &audiostate->blocks[audiostate->next_block];
		audiostate->next_block = (audiostate->next_block + 1) % audiostate->num_blocks;

		// Unprepare/reallocate the block if needed to handle the new audio.
		// (We use dwUser to keep track of the size we allocated in a block.)
		if (block->dwFlags &WHDR_PREPARED)
			waveOutUnprepareHeader(audiostate->dev, block, sizeof(WAVEHDR));
		if (block->dwUser < audiodatalen)
		{
			void *new_blockdata = realloc(block->lpData, audiodatalen);
			if (new_blockdata == nullptr)
				return; // discard if unable to allocate
			block->lpData = (LPSTR)new_blockdata;
			block->dwUser = audiodatalen; // new length allocated to this block
		}
		// Set up the block with the new audio
		memcpy(block->lpData, audiodata, audiodatalen);
		block->dwBufferLength = audiodatalen;
		waveOutPrepareHeader(audiostate->dev, block, sizeof(WAVEHDR));
		waveOutWrite(audiostate->dev, block, sizeof(WAVEHDR));
	}

	// Sets our TTS parameters
	int TRSpeakTTS::set_tts_params(RSTTSInst tts, wchar_t const *ttslic, wchar_t *lang, wchar_t *voice, int sample_rate)
	{
		// char *ttslic_utf8 = to_utf8(ttslic);
		char *lang_utf8  = System::UTF8Encode(String(lang)).c_str();
		char *voice_utf8 = System::UTF8Encode(String(voice)).c_str();

		// fprintf(stderr, "License: %s\n", ttslic_utf8);
		int err = rsttsSetParameter(tts, RSTTS_PARAM_LICENSE_BUFFER, RSTTS_TYPE_STRING, licence);
		// int err = rsttsSetParameter(tts, RSTTS_PARAM_LICENSE_FILE, RSTTS_TYPE_STRING, (const void *)ttslic_utf8);
		if (RSTTS_SUCCESS(err))
			err = rsttsSetLanguage(tts, lang_utf8);
		if (RSTTS_SUCCESS(err) && voice) // only if voice provided
				err = rsttsSetVoiceByName(tts, voice_utf8);

		if (RSTTS_SUCCESS(err))
			err = rsttsSetSampleRate(tts, sample_rate);

		// free(ttslic_utf8);
		// free(lang_utf8);
		// free(voice_utf8);
		return err;
	}

	TRSpeakTTS *TRSpeakTTS::s_instance = nullptr;

	TRSpeakTTS *TRSpeakTTS::Instance()
	{
		if (s_instance == nullptr)
		{
			s_instance = new TRSpeakTTS();
			s_instance->Init();
		}
		return s_instance;
	}

	void TRSpeakTTS::DestroyInstance()
	{
		delete s_instance;
		s_instance = nullptr;

	}

	TRSpeakTTS::TRSpeakTTS()
		: d_language("nl"),
		  d_voice("Ilse"),
		  d_ttsData("./data/tts/")
	{
		char *ttsdir_utf8 = System::UTF8Encode(d_ttsData).c_str();
		tts               = rsttsInit(ttsdir_utf8);
	}

	TRSpeakTTS::~TRSpeakTTS()
	{
		Stop();
		rsttsFree(tts);
	}

	void TRSpeakTTS::SetLanguage(String value)
	{
		d_language = value;
	}

	void TRSpeakTTS::SetVoice(String value)
	{
		d_voice = value;
	}

	void TRSpeakTTS::SetVolume(int value)
	{
		double volume = (RSTTS_VOLUME_MAX * value) / 100.;
		rsttsSetVolume(tts, 250);
	}

	void TRSpeakTTS::SetSpeed(int value)
	{
		// rate runs from -50 to 50 ... where 0 is default.
		double speed = RSTTS_SPEED_DEFAULT + value;
		rsttsSetSpeed(tts, speed);
	}

	void TRSpeakTTS::Stop()
	{
		// switch (rsttsGetState(tts))
		// {
		// case RSTTSInst_unset:
		// case RSTTSInst_ready:
		// break;
		// case RSTTSInst_paused:
		// case RSTTSInst_playing:
		// case RSTTSInst_stopping:
		try
		{
			rsttsStop(tts);
			rsttsWaitState(tts, RSTTSInst_ready, 250);
			// }
		}
		catch (...)
		{
			int ludo = 0;
		}
		if (rsttsGetState(tts) != RSTTSInst_ready)
			int ludo = 0;
	}

	// ---------------------------------------------------------------------------
	void TRSpeakTTS::Speak(String value)
	{
		if (value.Trim().Length() <= 0)
			return;

		std:: thread t(Process, value);
		t.detach();
	}

	void TRSpeakTTS::Process(String value)
	{
		std::lock_guard < std::mutex > guard(g_ttsmutex);
		if (value.Trim().Length() <= 0)
			return;
		try
		{
			Stop();
			// Create TTS instance
			// char *    ttsdir_utf8 = to_utf8(d_ttsData.w_str());
			// RSTTSInst tts         = rsttsInit(ttsdir_utf8);
			if (tts == nullptr)
			{
				wchar_t *errmsg = System::UTF8Decode(rsttsGetErrorMessage(nullptr)).c_bstr();
				if (errmsg)
				{
					throw(new Exception(L"TTS init failed: " + String(errmsg) + "s\n"));
					free(errmsg);
				}
				return; // exit(EXIT_FAILURE);
			}

			// Initialize TTS.
			// We can use any sample rate we wish as far as the TTS engine is
			// concerned, but we should make sure that the audio device and the
			// TTS engine use the same rate.
			const int sample_rate = 22050;
			int err = set_tts_params(tts, nullptr /* licence */ , d_language.w_str(), d_voice.w_str(), sample_rate);

			// Set up audio (with the same sample rate)
			struct audio_state audiostate;
			const int audioqueue_num_blocks = 4;
			if (!audio_init(&audiostate, audioqueue_num_blocks, sample_rate))
			{
				throw(new Exception(L"Failed to initialize audio\n"));
				return; // EXIT_FAILURE;
			}

			// Setup TTS callback (providing our audio state as callback user data)
			if (RSTTS_SUCCESS(err))
			{
				err = rsttsSetAudioCallback(tts, tts_audio_callback, &audiostate);
			}
			err = 0;

			// Call the TTS engine with the text to read. We could do this multiple
			// times if we want to read multiple texts.
			if (RSTTS_SUCCESS(err))
			{
				char *text_utf8 = System::UTF8Encode(value).c_str();
				err             = rsttsSynthesize(tts, text_utf8, "text");
			}

			// Report error if err indicates error
			if (RSTTS_ERROR(err))
			{
				wchar_t *errmsg = System::UTF8Decode(rsttsGetErrorMessage(nullptr)).c_bstr();
				if (errmsg)
				{
					throw(new Exception(L"TTS failed: " + String(errmsg) + "s\n"));
					free(errmsg);
				}
			}

			// Free TTS instance and close audio device when we're done
			audio_finish(&audiostate);
		}
		catch (...)
		{
			return;
		}

	}

}
