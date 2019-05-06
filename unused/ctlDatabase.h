// ---------------------------------------------------------------------------
#ifndef ctlDatabaseH
#define ctlDatabaseH
// ---------------------------------------------------------------------------
#include <FireDAC.Comp.Client.hpp>
#include <FireDAC.Comp.DataSet.hpp>
#include <FireDAC.VCLUI.Wait.hpp>
#include <FireDAC.Phys.hpp>
#include <FireDAC.Phys.Intf.hpp>
#include <FireDAC.Phys.SQLite.hpp>
#include <FireDAC.Phys.SQLiteDef.hpp>
#include <FireDAC.Stan.Async.hpp>
#include <FireDAC.Stan.Def.hpp>
#include <FireDAC.Stan.Error.hpp>
#include <FireDAC.Stan.ExprFuncs.hpp>
#include <FireDAC.Stan.Intf.hpp>
#include <FireDAC.Stan.Option.hpp>
#include <FireDAC.Stan.Pool.hpp>
#include <FireDAC.Stan.Param.hpp>
#include <FireDAC.UI.Intf.hpp>
#include <FireDAC.DApt.hpp>
#include <FireDAC.DApt.Intf.hpp>
#include <FireDAC.DatS.hpp>

// ---------------------------------------------------------------------------
class TctlDatabase
{
	TFDConnection *d_SQLite;
	TFDQuery *     d_Query;

	String d_programPath;

	static TctlDatabase *s_instance;

public:
	static TctlDatabase *Instance();
	static void DestroyInstance();

	void Connect();
	void Disconnect();

	// String GetSound(int KeyCode,bool UseCover = false);
	String GetSound(int KeyCode, bool ctrl, bool shft, bool alt, bool UseCover = false); // what should we sound like
	String GetValue(int keyCode, bool ctrl, bool shft, bool alt, bool UseCover = false); // what should we display

protected:
private:
	TctlDatabase();
	~TctlDatabase();

	void Configure();
	void Initialize(String aFile);

	void CreateKeysTable();
	void FillKeysClassicTable();
	void FillKeysCoverTable();

	void __fastcall SQLiteAfterConnect(TObject *Sender);
	void __fastcall SQLiteAfterDisconnect(TObject *Sender);
};

#endif
