//
// Speech.cpp
//

#include <thread>

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#include "Audio.h"
#include "Debug.h"
#include "Speech.h"

#ifdef  __BORLANDC__
#pragma package(smart_init)
#endif

static const int kChannels = 1;
static const int kSampleRate = 22050;
static const int kSampleSize = 2;

#if defined __LANGUAGE_NL__
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
#elif defined __LANGUAGE_NL_BE__
static const char kLicense[] = "<?xml version=\"1.0\"?>\
<license version=\"1.0\" licid=\"1e740ecca77d93e6746f433d5805140c\" subject=\"rSpeak SDK Ilse Veerle, BNC, exp. 2020-06-02\">\
	<issued>2020-05-14</issued>\
	<licensee>\
		<company name=\"BNC Distribution\" address=\"Wasaweg 3a, 9723JD Groningen, The Netherlands\"/>\
		<contact name=\"Bertran van den Hoff\" email=\"bertran.vandenhoff@bnc-distribution.nl\"/>\
	</licensee>\
	<product name=\"SDK rSpeak\"/>\
	<general channels=\"0\" speed=\"0\" textlimit=\"0\" expires=\"2020-06-02\"/>\
	<voice name=\"Ilse\" vendor=\"rSpeak\"/>\
	<voice name=\"Veerle\" vendor=\"rSpeak\"/>\
	<signature id=\"20ccdbe7165d8d221a95853a1dc8d0fb0c5eaa83fca9549a44f3d69eed2395cca25c58b2e43b0692bfcc9886b15c13afa6840411f95428e443b61806fdf6eaa7\"/>\
</license>";
#else
#error Unsupported language.
#endif

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

	m_thread = std::thread(&Speech::ThreadProc, this);

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

float Speech::GetSpeed()
{
	float speed = -1.0f;
	int result = rsttsGetSpeed(m_rstts, &speed);
	return RSTTS_SUCCESS(result) ? speed : -1.0f;
}

bool Speech::SetSpeed(float value)
{
	int result = rsttsSetSpeed(m_rstts, static_cast<float>(RSTTS_SPEED_DEFAULT) + value);
	return RSTTS_SUCCESS(result);
}

float Speech::GetVolume()
{
	float volume = -1.0f;
	int result = rsttsGetVolume(m_rstts, &volume);
	return RSTTS_SUCCESS(result) ? volume : -1.0f;
}

bool Speech::SetVolume(float value)
{
	int result = rsttsSetVolume(m_rstts, value);
	return RSTTS_SUCCESS(result);
}

int Speech::GetAudioVolume()
{
	return m_audio.GetVolume();
}

bool Speech::SetAudioVolume(int value)
{
	return m_audio.SetVolume(value);
}

void Speech::Speak(std::string text)
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
		std::string text = m_queue.Dequeue();
		rsttsSynthesize(m_rstts, text.c_str(), "text");
	}
}

void Speech::TTSAudioCallback(RSTTSInst inst, const void* audiodata, size_t audiodatalen, void* userptr)
{
	(void)inst;

	Speech* pThis = (Speech*)userptr;
	pThis->m_audio.Write(audiodata, audiodatalen);
}
