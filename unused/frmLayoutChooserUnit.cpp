// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "frmLayoutChooserUnit.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmLayoutChooser *frmLayoutChooser;

// ---------------------------------------------------------------------------
__fastcall TfrmLayoutChooser::TfrmLayoutChooser(TComponent* Owner)
	: TForm(Owner) {
}

// ---------------------------------------------------------------------------
void __fastcall TfrmLayoutChooser::OnChildClick(TObject *Sender) {
	dynamic_cast<TPanel *>(dynamic_cast<TControl *>(Sender)->Parent)->OnClick
		(Sender);
}

// ---------------------------------------------------------------------------
void __fastcall TfrmLayoutChooser::pnlKWeCoverLayoutClick(TObject *Sender) {
	pnlClassicLayout->Color = clBtnFace;
	pnlKWeCoverLayout->Color = clHighlight;
}
// ---------------------------------------------------------------------------

void __fastcall TfrmLayoutChooser::pnlClassicLayoutClick(TObject *Sender) {
	pnlClassicLayout->Color = clHighlight;
	pnlKWeCoverLayout->Color = clBtnFace;
}
// ---------------------------------------------------------------------------
//
