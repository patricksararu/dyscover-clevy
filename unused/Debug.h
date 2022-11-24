//---------------------------------------------------------------------------

#ifndef DebugH
#define DebugH

//---------------------------------------------------------------------------

void DebugLog(LPCTSTR lpszFormat, ...);

class DebugTimer
{
public:
	DebugTimer(LPCTSTR lpszContext);
	~DebugTimer();

private:
	LPCTSTR m_lpszContext;
	LARGE_INTEGER m_liStartTime;
};

#endif
