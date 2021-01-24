//
// Speech.h
//

#pragma once

#include <string>

#include <librstts.h>

#include "Audio.h"
#include "Queue.h"

class Speech
{
public:
	Speech();
	~Speech();

	bool Init(const char* basedir, const char* lang, const char* voice);
	void Term();

	float GetSpeed();
	bool SetSpeed(float value);

	float GetVolume();
	bool SetVolume(float value);

	void Speak(std::string text);
	void Stop();

private:
	Queue<std::string> m_queue;
	std::thread m_thread;
	RSTTSInst m_rstts;
	Audio m_audio;
	bool m_quit;  // Used to signalize thread to exit

	void ThreadProc();

	static void TTSAudioCallback(RSTTSInst, const void*, size_t, void*);
};
