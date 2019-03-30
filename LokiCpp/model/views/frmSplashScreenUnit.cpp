// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "frmSplashScreenUnit.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmSplashScreen *frmSplashScreen;

// ---------------------------------------------------------------------------
__fastcall TfrmSplashScreen::TfrmSplashScreen(TComponent *Owner)
	: TForm(Owner)
{
#ifdef _WIN64
	img64bit->Visible = true;
#else
	img32bit->Visible = true;
#endif
}
// ---------------------------------------------------------------------------

void TfrmSplashScreen::SetMessage(String message)
{
	lblMessage->Caption = message;
	// force immediate processing
	Application->ProcessMessages();
}

void TfrmSplashScreen::SetApplication(String value)
{
	lblApplication->Caption = value;
	Label1->Caption         = value + " " + lblVersion->Caption;
}

void TfrmSplashScreen::SetVersion(String value)
{
	lblVersion->Caption = value;
	Label1->Caption     = lblApplication->Caption + " " + value;
}

void TfrmSplashScreen::SetVersionDB(String value)
{
	lblVersionDB->Caption = value;
}

void TfrmSplashScreen::SetFramework(String value)
{
	lblFramework->Caption = value;
}

void TfrmSplashScreen::SetVendor(String value)
{
	lblVendor->Caption = value;
}

void TfrmSplashScreen::SetBuildDate(String value)
{
	lblBuildDate->Caption = value;
}

void TfrmSplashScreen::SetArchitect(String value)
{
	lblArchitect->Caption = value;
}
