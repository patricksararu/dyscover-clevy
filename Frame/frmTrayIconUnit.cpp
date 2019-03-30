// ---------------------------------------------------------------------------
#include <vcl.h>
#include <Winuser.h>
// ---------------------------------------------------------------------------
#pragma hdrstop
// ---------------------------------------------------------------------------
#include "frmTrayIconUnit.h"
#include "frmMainUnit.h"
#include "lokiCpp/common/Language.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
// ---------------------------------------------------------------------------
TfrmTrayIcon *frmTrayIcon;

typedef lokiCpp::TLanguage LANG;

// ---------------------------------------------------------------------------
__fastcall TfrmTrayIcon::TfrmTrayIcon(TComponent *Owner)
	: TForm(Owner)
{
	chkSound->Caption  = LANG::Translate("TfrmMain", chkSound->Name);
	chkSounds->Caption = LANG::Translate("TfrmMain", chkSounds->Name);
	chkTTS->Caption    = LANG::Translate("TfrmMain", chkTTS->Name);
	lblVolume->Caption = LANG::Translate("TfrmMain", lblVolume->Name);

}

// void __fastcall TfrmTrayIcon::WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
void __fastcall TfrmTrayIcon::WndProc(TMessage &Message)
{
	if (Message.Msg == WM_MOUSELEAVE)
	{
		POINT *myPoint = new POINT();

		if (GetCursorPos(myPoint))
		{
			Timer1->Enabled = !((myPoint->x > Left && myPoint->x < (Left + Width)) &&
				(myPoint->y > Top && myPoint->y < (Top + Height)));
		}
		else
		{
			Timer1->Enabled = true;
		}

	}
	else if (Message.Msg == WM_MOUSEHOVER)
	{
		Timer1->Enabled = false;
	}

	TForm::WndProc(Message);
}
// WM_MOUSEMOVE

// ---------------------------------------------------------------------------
void __fastcall TfrmTrayIcon::Button1Click(TObject *Sender)
{
	Hide();
}

// ---------------------------------------------------------------------------
void __fastcall TfrmTrayIcon::FormMouseLeave(TObject *Sender)
{
	// Timer1->Enabled = true;
}

// ---------------------------------------------------------------------------
void __fastcall TfrmTrayIcon::FormMouseEnter(TObject *Sender)
{
	Timer1->Enabled = false;
}
// ---------------------------------------------------------------------------

void __fastcall TfrmTrayIcon::Timer1Timer(TObject *Sender)
{
	Hide();
}
// ---------------------------------------------------------------------------

void __fastcall TfrmTrayIcon::tbVolumeChange(TObject *Sender)
{
	dynamic_cast < TfrmMain * > (Owner)->tbVolume->Position = -1 * tbVolume->Position;
	Timer1->Enabled = false;
}
// ---------------------------------------------------------------------------

void __fastcall TfrmTrayIcon::chkSoundClick(TObject *Sender)
{
	dynamic_cast < TfrmMain * > (Owner)->chkSound->Checked = chkSound->Checked;
}

// ---------------------------------------------------------------------------
void __fastcall TfrmTrayIcon::chkSoundsClick(TObject *Sender)
{
	dynamic_cast < TfrmMain * > (Owner)->chkSounds->Checked = chkSounds->Checked;
}

// ---------------------------------------------------------------------------
void __fastcall TfrmTrayIcon::chkTTSClick(TObject *Sender)
{
	dynamic_cast < TfrmMain * > (Owner)->chkTTS->Checked = chkTTS->Checked;
}

// ---------------------------------------------------------------------------
void __fastcall TfrmTrayIcon::FormHide(TObject *Sender)
{
	Timer1->Enabled = false;
}
// ---------------------------------------------------------------------------
