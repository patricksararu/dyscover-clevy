//---------------------------------------------------------------------------

#ifndef frmLayoutChooserUnitH
#define frmLayoutChooserUnitH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Imaging.pngimage.hpp>
//---------------------------------------------------------------------------
class TfrmLayoutChooser : public TForm
{
__published:	// IDE-managed Components
	TImage *Image1;
	TPanel *pnlClassicLayout;
	TPanel *pnlKWeCoverLayout;
	TImage *Image2;
	TLabel *Label1;
	TLabel *Label2;
	void __fastcall OnChildClick(TObject *Sender);
	void __fastcall pnlKWeCoverLayoutClick(TObject *Sender);
	void __fastcall pnlClassicLayoutClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TfrmLayoutChooser(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmLayoutChooser *frmLayoutChooser;
//---------------------------------------------------------------------------
#endif
