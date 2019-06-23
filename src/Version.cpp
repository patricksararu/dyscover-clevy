//---------------------------------------------------------------------------

#pragma hdrstop

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "Version.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

#define MAXFILENAME 1024

bool GetAppVersion(Version& version)
{
	TCHAR szFilename[MAXFILENAME];
	DWORD dwResult = GetModuleFileName(NULL, szFilename, MAXFILENAME);
	if (dwResult == 0)
	{
		return false;
	}

	DWORD dwSize = GetFileVersionInfoSize(szFilename, nullptr);
	if (dwSize <= 0)
	{
		return false;
	}

	BYTE* pbData = new BYTE[dwSize];
	if (pbData == nullptr)
	{
		return false;
	}

	if (!GetFileVersionInfo(szFilename, 0L, dwSize, pbData))
	{
		delete pbData;
		return false;
	}

	VS_FIXEDFILEINFO* pFileInfo;
	UINT uFileInfoLen;
	if (!VerQueryValue(pbData, TEXT("\\"), (LPVOID*)&pFileInfo, &uFileInfoLen))
	{
		delete pbData;
		return false;
	}

	if (pFileInfo->dwSignature != 0xFEEF04BD)
	{
		delete pbData;
		return false;
	}

	version.major    = HIWORD(pFileInfo->dwProductVersionMS);
	version.minor    = LOWORD(pFileInfo->dwProductVersionMS);
	version.revision = HIWORD(pFileInfo->dwProductVersionLS);
	version.build    = LOWORD(pFileInfo->dwProductVersionLS);

	return true;
}

