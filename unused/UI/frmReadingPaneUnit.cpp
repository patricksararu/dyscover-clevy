// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "frmReadingPaneUnit.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmReadingPane *frmReadingPane;

// ---------------------------------------------------------------------------
__fastcall TfrmReadingPane::TfrmReadingPane(TComponent *Owner)
	: TForm(Owner)
{
}

// ---------------------------------------------------------------------------
void __fastcall TfrmReadingPane::Timer1Timer(TObject *Sender)
{
	Timer1->Enabled = false;
	ShowWindow(Handle, SW_HIDE);
}
// ---------------------------------------------------------------------------

void __fastcall TfrmReadingPane::WndProc(TMessage &Message)
{
	int msgCount = 0;
	if (Message.Msg == WM_PASS_GETTEXT_)
	{
		String w = (wchar_t *)Message.LParam;

		SetCaption(w);
	}
	// if (Message.Msg == WM_NCACTIVATE)
	// {
	// return;
	// }

	TForm::WndProc(Message);
}

void TfrmReadingPane::SetCaption(wchar_t aCaption)
{
	lblSpeech->Caption = aCaption;
	Timer1->Enabled    = true;
	ShowWindow(Handle, SW_SHOWNOACTIVATE);
	// Show();
}

void TfrmReadingPane::SetCaption(String aCaption)
{
	lblSpeech->Caption = aCaption;
	Timer1->Enabled    = true;
	ShowWindow(Handle, SW_SHOWNOACTIVATE);
	// Show();
}

void __fastcall TfrmReadingPane::FormCreate(TObject *Sender)
{
	SendMessage(Handle, WM_NCACTIVATE, WA_ACTIVE, 0);
	Top  = 25;
	Left = 800;
}

// ---------------------------------------------------------------------------
void __fastcall TfrmReadingPane::FormShow(TObject *Sender)
{
	// PostMessage(Handle, WM_NCACTIVATE, WA_ACTIVE, 0);
}
// ---------------------------------------------------------------------------
