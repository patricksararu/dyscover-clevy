// ---------------------------------------------------------------------------

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <tchar.h>
#include <stdio.h>

#pragma hdrstop

#include "Debug.h"

// ---------------------------------------------------------------------------

#pragma package(smart_init)

#define BUFFER_SIZE 1024

void DebugLog(LPCTSTR lpszFormat, ...)
{
	TCHAR szBuffer[BUFFER_SIZE];
	ZeroMemory(szBuffer, BUFFER_SIZE);
	va_list args;
	va_start(args, lpszFormat);
	_vsntprintf(szBuffer, BUFFER_SIZE, lpszFormat, args);
	va_end(args);
	OutputDebugString(szBuffer);
}

DebugTimer::DebugTimer(LPCTSTR lpszContext)
{
	m_lpszContext = lpszContext;
	QueryPerformanceCounter(&m_liStartTime);
}

DebugTimer::~DebugTimer()
{
	LARGE_INTEGER endTime;
	LARGE_INTEGER frequency;
	QueryPerformanceCounter(&endTime);
	QueryPerformanceFrequency(&frequency);
	LONGLONG elapsedTime = endTime.QuadPart - m_liStartTime.QuadPart;
	LONGLONG elapsedTimeMs = 1000 * elapsedTime / frequency.QuadPart;
	DebugLog(TEXT("%s spent %lld ms"), m_lpszContext, elapsedTimeMs);
}
