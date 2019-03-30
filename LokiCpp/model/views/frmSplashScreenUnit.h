// ---------------------------------------------------------------------------

#ifndef frmSplashScreenUnitH
#define frmSplashScreenUnitH
// ---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Graphics.hpp>
#include <Vcl.Imaging.pngimage.hpp>

// ---------------------------------------------------------------------------
class TfrmSplashScreen :
	public TForm
{
__published:    // IDE-managed Components
	TImage *Image2;
	TImage *Image1;
	TLabel *lblMessage;
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label3;
	TLabel *Label4;
	TLabel *Label5;
	TLabel *Label6;
	TLabel *Label7;
	TLabel *lblVersion;
	TLabel *lblVersionDB;
	TLabel *lblBuildDate;
	TLabel *lblArchitect;
	TLabel *lblVendor;
	TLabel *lblFramework;
	TLabel *Label8;
	TLabel *lblApplication;
	TImage *Image4;
	TImage *img32bit;
	TImage *img64bit;
		private : // User declarations
	public :     // User declarations
		__fastcall TfrmSplashScreen(TComponent *Owner);

	void SetMessage(String message);

	void SetApplication(String value);
	void SetVersion(String value);
	void SetVersionDB(String value);
	void SetFramework(String value);
	void SetVendor(String value);
	void SetBuildDate(String value);
	void SetArchitect(String value);

};

// ---------------------------------------------------------------------------
extern PACKAGE TfrmSplashScreen *frmSplashScreen;
// ---------------------------------------------------------------------------
#endif
