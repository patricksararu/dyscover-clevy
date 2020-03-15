//
// Audio.cpp
//

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#include "Audio.h"

#ifdef  __BORLANDC__
#pragma package(smart_init)
#endif

Audio::Audio() : m_pStream(nullptr)
{
	Pa_Initialize();
}

Audio::~Audio()
{
	Close();

	Pa_Terminate();
}

bool Audio::Open(int channels, int samplerate, PaSampleFormat sampleformat)
{
	PaError error = Pa_OpenDefaultStream(&m_pStream, 0, channels, sampleformat, samplerate, paFramesPerBufferUnspecified, nullptr, nullptr);
	if (error != paNoError)
	{
		return false;
	}

	error = Pa_StartStream(m_pStream);
	return error == paNoError;
}

void Audio::Close()
{
	Stop();

	Pa_CloseStream(m_pStream);
}

bool Audio::Write(const void* audiodata, unsigned long audiodatalen)
{
	PaError error = Pa_WriteStream(m_pStream, audiodata, audiodatalen);

	return error == paNoError;
}

void Audio::Stop()
{
	Pa_AbortStream(m_pStream);
	Pa_Sleep(100);
	Pa_StartStream(m_pStream);
}
