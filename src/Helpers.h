//---------------------------------------------------------------------------

#ifndef HelpersH
#define HelpersH

//---------------------------------------------------------------------------

struct Version
{
	int major;
	int minor;
	int revision;
	int build;
};

bool GetAppVersion(Version&);

bool IsAnotherInstanceRunning();

#endif
