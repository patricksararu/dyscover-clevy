// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
// ---------------------------------------------------------------------------
#include <Vcl.Styles.hpp>
#include <Vcl.Themes.hpp>
#include <Registry.hpp>
#include <SysUtils.hpp>
#include <Windows.hpp>
#include <TlHelp32.h>
#include <Dialogs.hpp>
//---------------------------------------------------------------------------
USEFORM("UI\frmMainUnit.cpp", frmMain);
USEFORM("UI\frmLayoutChooserUnit.cpp", frmLayoutChooser);
USEFORM("UI\frmReadingPaneUnit.cpp", frmReadingPane);
USEFORM("UI\frmTrayIconUnit.cpp", frmTrayIcon);
USEFORM("UI\frmSplashScreenUnit.cpp", frmSplashScreen);
//---------------------------------------------------------------------------
#include "Settings.h"
#include "UI/frmSplashScreenUnit.h"
//---------------------------------------------------------------------------
int ProcessCount(String const ExeName)
{
	bool ContinueLoop;
	HANDLE SnapshotHandle;
	PROCESSENTRY32 ProcessEntry32;

	SnapshotHandle         = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	ProcessEntry32.dwSize = sizeof(ProcessEntry32);

	ContinueLoop = Process32First(SnapshotHandle, &ProcessEntry32);

	int Result = 0;

	while (ContinueLoop)
	{
		if ((ExtractFileName(ProcessEntry32.szExeFile).CompareIC(ExeName) == 0) || (String(ProcessEntry32.szExeFile).CompareIC(ExeName) == 0))
			++Result;

		ContinueLoop = Process32Next(SnapshotHandle, &ProcessEntry32);
	}
	CloseHandle(SnapshotHandle);

	return Result;
}

// ---------------------------------------------------------------------------
int WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
	TfrmSplashScreen *frmSplash = nullptr;
	try
	{
		Application->Initialize();
		Application->MainFormOnTaskBar = true;

		frmSplash = new TfrmSplashScreen(nullptr);
		frmSplash->Show();
		Application->ProcessMessages();

		// load the available settings
		frmSplash->SetMessage("Loading Settings");
		lokiCpp::TSettings *conf = lokiCpp::TSettings::Instance();

		String ApplicationTitle = "";
		String ApplicationName  = "";
		String tmp              = "";
		conf->GetConfig("version", "Application", tmp);
		frmSplash->SetApplication(tmp);
		ApplicationName = tmp;
		ApplicationTitle += tmp;
		conf->GetConfig("version", "Version", tmp);
		frmSplash->SetVersion(tmp);
		ApplicationTitle += " " + tmp;
		conf->GetConfig("version", "DatabaseVersion", tmp);
		frmSplash->SetVersionDB(tmp);

		frmSplash->SetFramework("n/a");
		frmSplash->SetVendor("BNC Distribution");
		conf->GetConfig("version", "BuildDate", tmp);
		frmSplash->SetBuildDate(tmp);
		ApplicationTitle += " BuildDate: " + tmp;
		frmSplash->SetArchitect("Stroetenga Technologies");

		frmSplash->SetMessage("Finalizing initialization");

		Sleep(2500);

		delete frmSplash;
		frmSplash = nullptr;

		if (ProcessCount(ExtractFileName(Application->ExeName)) > 1)
		{
			MessageDlg(L"Application is already running!", mtError, TMsgDlgButtons() << mbOK, 0);
			Application->Terminate();
		}

		Application->Title = ApplicationName;
		TStyleManager::TrySetStyle("Carbon");
		Application->ShowMainForm = false;
		Application->CreateForm(__classid(TfrmMain), &frmMain);
		Application->Run();
	}
	catch (Exception &exception)
	{
		Application->ShowException(&exception);
	}
	catch (...)
	{
		try
		{
			throw Exception("");
		}
		catch (Exception &exception)
		{
			Application->ShowException(&exception);
		}
	}
	return 0;
}
// ---------------------------------------------------------------------------
