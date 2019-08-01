//---------------------------------------------------------------------------

#ifndef SpeechH
#define SpeechH

#include <librstts.h>

#include "Audio.h"
#include "Queue.h"

//---------------------------------------------------------------------------

class Speech
{
public:
	Speech();
	~Speech();

	bool Init(const char* basedir, const char* lang, const char* voice);
	void Term();

	int GetSpeed();
	bool SetSpeed(int value);

	int GetVolume();
	bool SetVolume(int value);

	int GetAudioVolume();
	bool SetAudioVolume(int value);

	void Speak(String text);
	void Stop();

private:
	Queue<String> m_queue;
	std::thread m_thread;
	RSTTSInst m_rstts;
	Audio m_audio;
	bool m_quit;  // Used to signalize thread to exit

	void ThreadProc();

	static void TTSAudioCallback(RSTTSInst, const void*, size_t, void*);
};

#endif
