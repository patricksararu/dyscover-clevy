//---------------------------------------------------------------------------

#include <thread>

#include <System.hpp>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <mmsystem.h>

#pragma hdrstop

#include "Debug.h"
#include "Audio.h"
#include "Speech.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

static const int kChannels = 1;
static const int kSampleRate = 22050;
static const int kSampleSize = 2;

static const char kLicense[] = "<?xml version=\"1.0\"?>\
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

Speech::Speech() : m_rstts(nullptr), m_quit(false)
{
}

Speech::~Speech()
{
	Term();
}

bool Speech::Init(const char* basedir, const char* lang, const char* voice)
{
	m_rstts = rsttsInit(basedir);
	if (m_rstts == nullptr) return false;

	int result = rsttsSetParameter(m_rstts, RSTTS_PARAM_LICENSE_BUFFER, RSTTS_TYPE_STRING, kLicense);
	if (RSTTS_ERROR(result))
	{
		Term();
		return false;
	}

	const int responsiveness = RSTTS_RESPONSIVENESS_FAST;
	result = rsttsSetParameter(m_rstts, RSTTS_PARAM_RESPONSIVENESS_SETTING, RSTTS_TYPE_INT, &responsiveness);
	if (RSTTS_ERROR(result))
	{
		Term();
		return false;
	}

	result = rsttsSetSampleRate(m_rstts, kSampleRate);
	if (RSTTS_ERROR(result))
	{
		Term();
		return false;
	}

	result = rsttsSetLanguage(m_rstts, lang);
	if (RSTTS_ERROR(result))
	{
		Term();
		return false;
	}

	result = rsttsSetVoiceByName(m_rstts, voice);
	if (RSTTS_ERROR(result))
	{
		Term();
		return false;
	}

	result = rsttsSetAudioCallback(m_rstts, TTSAudioCallback, this);
	if (RSTTS_ERROR(result))
	{
		Term();
		return false;
	}

	if (!m_audio.Open(kChannels, kSampleRate, kSampleSize))
	{
		Term();
		return false;
	}

	m_thread = std::thread(ThreadProc);

	return true;
}

void Speech::Term()
{
	if (m_thread.joinable())
	{
		// Request thread to exit.
		m_quit = true;
		m_queue.Enqueue("");

		// Wait for thread to exit.
		m_thread.join();
	}

	m_audio.Close();

	if (m_rstts != nullptr)
	{
		rsttsFree(m_rstts);
		m_rstts = nullptr;
	}
}

int Speech::GetSpeed()
{
	float speed = -1.0;
	int result = rsttsGetSpeed(m_rstts, &speed);
	return speed;
}

bool Speech::SetSpeed(int value)
{
	int result = rsttsSetSpeed(m_rstts, RSTTS_SPEED_DEFAULT + value);
	return RSTTS_SUCCESS(result);
}

int Speech::GetVolume()
{
	float volume = -1.0;
	int result = rsttsGetVolume(m_rstts, &volume);
	return volume;
}

bool Speech::SetVolume(int value)
{
	int result = rsttsSetVolume(m_rstts, value);
	return RSTTS_SUCCESS(result);
}

void Speech::Speak(String text)
{
	m_queue.Enqueue(text);
}

void Speech::Stop()
{
	// Stop TTS engine.
	if (m_rstts != nullptr)
	{
		rsttsStop(m_rstts);
	}

	// Stop any playing audio.
	m_audio.Stop();
}

void Speech::ThreadProc()
{
	while (!m_quit)
	{
		String text = m_queue.Dequeue();

		char* utf8Str = System::UTF8Encode(text).c_str();

		rsttsSynthesize(m_rstts, utf8Str, "text");
	}
}

void Speech::TTSAudioCallback(RSTTSInst inst, const void* audiodata, size_t audiodatalen, void* userptr)
{
	Speech* pThis = (Speech*)userptr;
	pThis->m_audio.Write(audiodata, audiodatalen);
}
