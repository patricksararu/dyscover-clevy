// ---------------------------------------------------------------------------
#ifndef RSpeakTTSH
#define RSpeakTTSH
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
#include <Windows.h>
#include <mmsystem.h>
#include <system.hpp>
// ---------------------------------------------------------------------------
#include "readSpeak\WaveOut.h"
// ---------------------------------------------------------------------------
#include "lib\readSpeak\librstts.h"
#include "lib\readSpeak\librstts_event.h"
#include <stdio.h>
#include <stdlib.h>
// ---------------------------------------------------------------------------

namespace rstts
{
	const char licence[] = "<?xml version=\"1.0\"?>\
<license version=\"1.0\" licid=\"4f2aab79db4149aa559d9044adac697e\" subject=\"rSpeak SDK Ilse for Clevy Dyscover software\">\
  <issued>2018-08-28</issued>\
  <licensee>\
	 <company name=\"BNC Distribution\" address=\"Wasaweg 3a, 9723JD Groningen, The Netherlands\"/>\
	 <contact name=\"Bertran van den Hoff\" email=\"bertran.vandenhoff@bnc-distribution.nl\"/>\
  </licensee>\
  <product name=\"SDK rSpeak\"/>\
  <general channels=\"0\" speed=\"0\" textlimit=\"0\" expires=\"-1\"/>\
  <voice name=\"Ilse\" vendor=\"rSpeak\"/>\
  <signature id=\"1ea8690890fffbbff71f2c37148dfd50e351e666940dadf494ed4ba122e901f7ad71df6d8b974d1261b638f12acf786777590cfb13509850ec700223f1de95cb\"/>\
</license>";

	class TRSpeakTTS
	{

		static TRSpeakTTS *s_instance;

		String d_language; // language to use eg. NL EN etc
		String d_voice;    // the selected voice eg. Ilse
		String d_ttsData;  // the root where the TTS data folder is located

		RSTTSInst tts;
		// audio_state audiostate;

	public:
		static TRSpeakTTS *Instance();
		static void DestroyInstance();

		void SetLanguage(String value);
		void SetVoice(String value);

		void SetVolume(int value);
		void SetSpeed(int value);

		void Speak(String value);
		void Stop();

	protected:

	private:
		TRSpeakTTS();
		~TRSpeakTTS();

		void Process(String value);

		void Init() {}; // initializes the TTS engine

		// This is our callback function that the TTS will call when it has
		// audio for us.
		// userptr is the pointer that we gave when we set up the callback.
		static void tts_audio_callback(RSTTSInst tts, const void *audiodata, size_t audiodatalen, void *userptr);

		// Sets our TTS parameters
		int set_tts_params(RSTTSInst tts, wchar_t const *ttslic, wchar_t *lang, wchar_t *voice, int sample_rate);
	};

}

#endif
