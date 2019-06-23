//---------------------------------------------------------------------------

#ifndef VersionH
#define VersionH

//---------------------------------------------------------------------------

struct Version
{
	int major;
	int minor;
	int revision;
	int build;
};

bool GetAppVersion(Version&);

#endif
