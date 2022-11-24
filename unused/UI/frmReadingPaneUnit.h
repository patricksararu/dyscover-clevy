//---------------------------------------------------------------------------

#ifndef frmReadingPaneUnitH
#define frmReadingPaneUnitH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Imaging.pngimage.hpp>
//---------------------------------------------------------------------------
int const WM_PASS_GETTEXT_ = WM_USER + 8;
class TfrmReadingPane : public TForm
{
__published:	// IDE-managed Components
	TLabel *lblSpeech;
	TTimer *Timer1;
	TImage *Image1;
	void __fastcall Timer1Timer(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
private:	// User declarations
	void __fastcall WndProc(TMessage &Message);
public:		// User declarations
	__fastcall TfrmReadingPane(TComponent* Owner);
	void SetCaption(String aCaption);
	void SetCaption(wchar_t aCaption);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmReadingPane *frmReadingPane;
//---------------------------------------------------------------------------
#endif
