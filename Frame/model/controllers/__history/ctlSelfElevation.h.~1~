// ---------------------------------------------------------------------------
#ifndef ctlSelfElevationH
#define ctlSelfElevationH
// ---------------------------------------------------------------------------
#include <System.Types.hpp>
// ---------------------------------------------------------------------------

class TctlSelfElevation
{
public:
	TctlSelfElevation();

	__property bool  UserIsInAdminGroup    = {read = IsUserInAdminGroup};
	__property bool  RunAsAdmin            = {read = IsRunAsAdmin};
	__property bool  ProcessIsElevated     = {read = IsProcessElevated};
	__property DWORD ProcessIntegrityLevel = {read = GetProcessIntegrityLevel};

	void SelfElevateClick(HWND Handle);

protected:
private:
	bool IsUserInAdminGroup();
	bool IsRunAsAdmin();
	bool IsProcessElevated();
	DWORD GetProcessIntegrityLevel();

	void ReportError(LPCWSTR pszFunction, DWORD dwError = GetLastError());
};

#endif
