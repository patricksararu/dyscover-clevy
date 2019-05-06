// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
// ---------------------------------------------------------------------------
#include "LokiCpp/common/Settings.h"
#include "LokiCpp/model/views/frmSplashScreenUnit.h"
// ---------------------------------------------------------------------------
#include <Vcl.Styles.hpp>
#include <Vcl.Themes.hpp>
#include <Registry.hpp>

USEFORM("LokiCpp\model\views\frmSplashScreenUnit.cpp", frmSplashScreen);
USEFORM("Frame\frmMainUnit.cpp", frmMain);

// ---------------------------------------------------------------------------
int WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
	TfrmSplashScreen *frmSplash = NULL;
	try
	{
		Application->Initialize();
		Application->MainFormOnTaskBar = true;

		frmSplash = new TfrmSplashScreen(NULL);
		frmSplash->Show();
		Application->ProcessMessages();

		// load the available settings
		frmSplash->SetMessage("Loading Settings");
		lokiCpp::TSettings *conf = lokiCpp::TSettings::Instance();

		String ApplicationTitle = "";
		String tmp              = "";
		conf->GetConfig("version", "Application", tmp);
		frmSplash->SetApplication(tmp);
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
		frmSplash->SetArchitect("Stroetenga Design");

		frmSplash->SetMessage("Finalizing initialization");

		Sleep(2500);

		delete frmSplash;
		frmSplash = NULL;

		Application->Title = ApplicationTitle;
		TStyleManager::TrySetStyle("Carbon");
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