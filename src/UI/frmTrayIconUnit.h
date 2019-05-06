// ---------------------------------------------------------------------------
#ifndef frmTrayIconUnitH
#define frmTrayIconUnitH
// ---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ExtCtrls.hpp>

// ---------------------------------------------------------------------------
class TfrmTrayIcon :
	public TForm
{
__published: // IDE-managed Components
	TButton *   Button1;
	TStatusBar *StatusBar1;
	TTimer *    Timer1;
	TTrackBar * tbVolume;
	TCheckBox * chkSound;
	TCheckBox * chkSounds;
	TCheckBox * chkTTS;
	TLabel *    lblVolume;

	void __fastcall Button1Click(TObject *Sender);
	void __fastcall FormMouseLeave(TObject *Sender);
	void __fastcall FormMouseEnter(TObject *Sender);
	void __fastcall Timer1Timer(TObject *Sender);
	void __fastcall tbVolumeChange(TObject *Sender);
	void __fastcall chkSoundClick(TObject *Sender);
	void __fastcall chkSoundsClick(TObject *Sender);
	void __fastcall chkTTSClick(TObject *Sender);
	void __fastcall FormHide(TObject *Sender);

private: // User declarations
	void __fastcall WndProc(TMessage &Message);
	// void __fastcall WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

public: // User declarations
	__fastcall TfrmTrayIcon(TComponent *Owner);
};

// ---------------------------------------------------------------------------
extern PACKAGE TfrmTrayIcon *frmTrayIcon;
// ---------------------------------------------------------------------------
#endif
