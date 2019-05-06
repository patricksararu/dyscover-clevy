#include <System.IOUtils.hpp>
#include <Dialogs.hpp>
// ---------------------------------------------------------------------------
#pragma hdrstop
// ---------------------------------------------------------------------------
#include "ctlDatabase.h"
// ---------------------------------------------------------------------------
#include <vector>
// ---------------------------------------------------------------------------
#pragma package(smart_init)
// ---------------------------------------------------------------------------

TctlDatabase *TctlDatabase::s_instance = nullptr;

TctlDatabase *TctlDatabase::Instance()
{
	if (s_instance == nullptr)
	{
		s_instance = new TctlDatabase();
	}
	return s_instance;
}

void TctlDatabase::DestroyInstance()
{
	delete s_instance;
	s_instance = nullptr;
}

TctlDatabase::TctlDatabase()
	: d_SQLite(new TFDConnection(nullptr)),
	  d_Query(new TFDQuery(d_SQLite))
{
	Configure();

	// System::Ioutils::TPath::GetHomePath will depend on the platform
	// Windows XP  				C:\Documents and Settings\<username>\Application Data
	// Windows Vista or later  C:\Users\<username>\AppData\Roaming
	// OS X  						/Users/<username>
	// iOS Device  				/private/var/mobile/Containers/Data/Application/<application ID>
	// iOS Simulator  			/Users/<username>/Library/Developer/CoreSimulator/Devices/<Device ID>/data/Containers/Data/Application/<application ID>
	// Android  					/data/data/<application ID>/files

	// this would be the preferred method for storing program data.
	d_programPath = System::Ioutils::TPath::GetHomePath();

#if defined(_Windows) && defined(_DEBUG)
	// however on windows it is easier to find the files in the program path. especially during debugging
	d_programPath = ExtractFilePath(ParamStr(0));
#endif

}

TctlDatabase::~TctlDatabase()
{
	Disconnect();
	delete d_Query;
	delete d_SQLite;
}

void TctlDatabase::Configure()
{
	d_SQLite->AfterConnect    = SQLiteAfterConnect;
	d_SQLite->AfterDisconnect = SQLiteAfterDisconnect;

	String databaseFileLocation = d_programPath + "data";
	if (!TDirectory::Exists(databaseFileLocation)) {
		TDirectory::CreateDirectory(databaseFileLocation);
	}

	if (d_SQLite->Connected)
	{
		d_SQLite->Close();
	}
	d_SQLite->Params->Clear();
	d_SQLite->Params->Add("DriverID=SQLite");
	d_SQLite->Params->Add("Database=" + d_programPath + "data" + System::Ioutils::TPath::DirectorySeparatorChar + "Keys.sdb");

	d_SQLite->Name           = "SQLiteConnection";
	d_SQLite->ConnectionName = "ClevySQL";

	d_Query->Connection = d_SQLite;

	Initialize(d_programPath + "data" + System::Ioutils::TPath::DirectorySeparatorChar + "Keys.sdb");
}

void TctlDatabase::Initialize(String aFile)
{
	if (!FileExists(aFile))
	{
		// the database must be created.
		try
		{
			Connect();
			CreateKeysTable();
			FillKeysClassicTable();
			FillKeysCoverTable();
		}
		catch (EDatabaseError &ex)
		{
			ShowMessage("Unable to initialize KeyLayout");
		}
	}
}

String TctlDatabase::GetSound(int keyCode, bool ctrl, bool shft, bool alt, bool UseCover)
{
	bool bControl = ctrl; // GetKeyState(VK_CONTROL) < 0;
	bool bShift   = shft; // GetKeyState(VK_SHIFT) < 0;
	bool bAlt     = alt;  // GetKeyState(VK_MENU) < 0;

	String query = "";

	if (UseCover)
	{
		query = "SELECT * FROM KEYS_COVER WHERE KEY_CODE = " + IntToStr(keyCode);
	}
	else
	{
		query = "SELECT * FROM KEYS_CLASSIC WHERE KEY_CODE = " + IntToStr(keyCode);
	}
	d_Query->SQL->Text = query;

	try
	{
		d_Query->Open();

		d_Query->First();

		if (!d_Query->Eof)
		{
			if (bShift)
			{
				return d_Query->FieldByName("SOUND_SHIFT")->AsString;
			}
			else if (bControl)
			{
				return d_Query->FieldByName("SOUND_CTRL")->AsString;
			}
			else if (bAlt)
			{
				return d_Query->FieldByName("SOUND_ALT")->AsString;
			}
			else
			{
				return d_Query->FieldByName("SOUND_NORMAL")->AsString;
			}
		}
		else
		{
			return ""; // String((wchar_t)(keyCode));
		}
	}
	catch (EDatabaseError &ex)
	{
		ShowMessage("Exeption raised with message : " + ex.Message);
	}
	return "";

}

String TctlDatabase::GetValue(int keyCode, bool ctrl, bool shft, bool alt, bool UseCover)
{
	if (keyCode >= 48 && keyCode <= 57)
	{
		return GetSound(keyCode, ctrl, shft, alt, UseCover);
	}
	else
	{
		return GetSound(keyCode, false, shft, alt, UseCover);
	}
}

void TctlDatabase::CreateKeysTable()
{
	// Each value stored in an SQLite database (or manipulated by the database engine) has one of the following storage classes:
	// -- nullptr. 	The value is a nullptr value.
	// -- INTEGER. The value is a signed integer, stored in 1, 2, 3, 4, 6, or 8 bytes depending on the magnitude of the value.
	// -- REAL. 	The value is a floating point value, stored as an 8-byte IEEE floating point number.
	// -- TEXT. 	The value is a text string, stored using the database encoding (UTF-8, UTF-16BE or UTF-16LE).
	// -- BLOB. 	The value is a blob of data, stored exactly as it was input.
	//
	// SQLite does not have a separate Boolean storage class. Instead, Boolean values are stored as integers 0 (false) and 1 (true).
	//
	// SQLite does not have a storage class set aside for storing dates and/or times. Instead, the built-in Date And Time Functions of SQLite are capable of storing dates and times as TEXT, REAL, or INTEGER values:
	// -- TEXT as ISO8601 strings ("YYYY-MM-DD HH:MM:SS.SSS").
	// -- REAL as Julian day numbers, the number of days since noon in Greenwich on November 24, 4714 B.C. according to the proleptic Gregorian calendar.
	// -- INTEGER as Unix Time, the number of seconds since 1970-01-01 00:00:00 UTC.

	// 69	E	1	1	1			1	1	E	EE	E	U

	try
	{
		d_Query->SQL->Text = "CREATE TABLE KEYS_CLASSIC(KEY_CODE INTEGER PRIMARY KEY ASC, SOUND_NORMAL , CONF1 INTEGER, CONF2, CONF3, CONF4, CONF5, CONF6, SOUND_SHIFT , SOUND_ALT, SOUND_CTRL)";
		d_Query->ExecSQL();
		d_SQLite->Commit();
		d_Query->SQL->Text = "CREATE TABLE KEYS_COVER(KEY_CODE INTEGER PRIMARY KEY ASC, SOUND_NORMAL , CONF1 INTEGER, CONF2, CONF3, CONF4, CONF5, CONF6, SOUND_SHIFT , SOUND_ALT, SOUND_CTRL)";
		d_Query->ExecSQL();
		d_SQLite->Commit();
	}
	catch (EDatabaseError &ex)
	{
		ShowMessage("Exeption raised with message : " + ex.Message);
	}

}

void TctlDatabase::FillKeysClassicTable()
{
	std::vector < String > keys; // key, normal,   shift, alt, contrl
	keys.push_back("INSERT INTO KEYS_CLASSIC VALUES( 49,'AA', 1, 1, 1, 1, 0,0,'','','1')");
	keys.push_back("INSERT INTO KEYS_CLASSIC VALUES( 50,'UU', 1, 1, 1, 1, 0,0,'','','2')");
	keys.push_back("INSERT INTO KEYS_CLASSIC VALUES( 51,'OO', 1, 1, 0, 1, 0,0,'','','3')");
	keys.push_back("INSERT INTO KEYS_CLASSIC VALUES( 52,'EE', 1, 1, 0, 1, 0,0,'','','4')");
	keys.push_back("INSERT INTO KEYS_CLASSIC VALUES( 53,'EU', 1, 1, 0, 1, 0,0,'','','5')");
	keys.push_back("INSERT INTO KEYS_CLASSIC VALUES( 54,'AU', 1, 1, 0, 1, 0,0,'','','6')");
	keys.push_back("INSERT INTO KEYS_CLASSIC VALUES( 55,'UI', 1, 1, 0, 1, 0,0,'','','7')");
	keys.push_back("INSERT INTO KEYS_CLASSIC VALUES( 56,'IE', 1, 1, 0, 1, 0,0,'','','8')");
	keys.push_back("INSERT INTO KEYS_CLASSIC VALUES( 57,'OE', 1, 1, 1, 1, 0,0,'','','9')");
	keys.push_back("INSERT INTO KEYS_CLASSIC VALUES( 48,'EI', 1, 1, 1, 1, 0,0,'','','0')");
	keys.push_back("INSERT INTO KEYS_CLASSIC VALUES( 219,'OU', 1, 1, 0, 0, 0,0,'','','')");
	keys.push_back("INSERT INTO KEYS_CLASSIC VALUES( 221,'IJ', 1, 1, 0, 1, 0,0,'','','IJ')");
	keys.push_back("INSERT INTO KEYS_CLASSIC VALUES( 186,'NG', 1, 1, 0, 0, 0,0,'','','')");
	keys.push_back("INSERT INTO KEYS_CLASSIC VALUES( 189,'EUR', 1, 1, 0, 1, 0,0,'','','')");
	keys.push_back("INSERT INTO KEYS_CLASSIC VALUES( 191,'OOR', 1, 1, 1, 0, 0,0,'','','')");
	keys.push_back("INSERT INTO KEYS_CLASSIC VALUES( 220,'CH', 1, 1, 0, 1, 0,0,'','','SJ')");
	keys.push_back("INSERT INTO KEYS_CLASSIC VALUES( 165,'EER', 1, 1, 0, 0, 1,1,'','','')");
	keys.push_back("INSERT INTO KEYS_CLASSIC VALUES( 222,'NK', 1, 1, 1, 1, 0,0,'','','')");
	keys.push_back("INSERT INTO KEYS_CLASSIC VALUES( 187,'SCH', 1, 1, 1, 1, 0,0,'','','')");
	keys.push_back("INSERT INTO KEYS_CLASSIC VALUES( 45,'AAI', 1, 1, 0, 1, 0,0,'','','')");
	keys.push_back("INSERT INTO KEYS_CLASSIC VALUES( 36,'OOI', 1, 1, 0, 1, 0,0,'','','')");
	keys.push_back("INSERT INTO KEYS_CLASSIC VALUES( 33,'OEI', 1, 1, 0, 1, 0,0,'','','')");
	keys.push_back("INSERT INTO KEYS_CLASSIC VALUES( 46,'EEUW', 1, 1, 0, 1, 0,0,'','','')");
	keys.push_back("INSERT INTO KEYS_CLASSIC VALUES( 35,'IEUW', 1, 1, 0, 1, 0,0,'','','')");
	keys.push_back("INSERT INTO KEYS_CLASSIC VALUES( 34,'UW', 1, 1, 0, 1, 0,0,'','','')");
	keys.push_back("INSERT INTO KEYS_CLASSIC VALUES( 65,'A', 1, 0, 1, 1, 1,1,'A','AA','')");
	keys.push_back("INSERT INTO KEYS_CLASSIC VALUES( 66,'B', 0, 0, 0, 0, 0,0,'B','','')");
	keys.push_back("INSERT INTO KEYS_CLASSIC VALUES( 67,'C', 1, 1, 1, 1, 0,0,'C','','K')");
	keys.push_back("INSERT INTO KEYS_CLASSIC VALUES( 68,'D', 1, 1, 1, 1, 0,0,'D','','')");
	keys.push_back("INSERT INTO KEYS_CLASSIC VALUES( 69,'E', 1, 1, 1, 1, 1,1,'E','EE','U')");
	keys.push_back("INSERT INTO KEYS_CLASSIC VALUES( 70,'F', 0, 0, 0, 0, 0,0,'F','','')");
	keys.push_back("INSERT INTO KEYS_CLASSIC VALUES( 71,'G', 0, 0, 0, 0, 0,0,'G','','')");
	keys.push_back("INSERT INTO KEYS_CLASSIC VALUES( 72,'H', 0, 0, 0, 0, 0,0,'H','','')");
	keys.push_back("INSERT INTO KEYS_CLASSIC VALUES( 73,'I', 1, 1, 1, 1, 1,1,'I','IE','I')");
	keys.push_back("INSERT INTO KEYS_CLASSIC VALUES( 74,'J', 0, 0, 0, 0, 0,0,'J','','')");
	keys.push_back("INSERT INTO KEYS_CLASSIC VALUES( 75,'K', 0, 0, 0, 0, 0,0,'K','','')");
	keys.push_back("INSERT INTO KEYS_CLASSIC VALUES( 76,'L', 0, 0, 0, 0, 0,0,'L','','')");
	keys.push_back("INSERT INTO KEYS_CLASSIC VALUES( 77,'M', 0, 0, 0, 0, 0,0,'M','','')");
	keys.push_back("INSERT INTO KEYS_CLASSIC VALUES( 78,'N', 0, 0, 0, 0, 0,0,'N','','')");
	keys.push_back("INSERT INTO KEYS_CLASSIC VALUES( 79,'O', 1, 1, 1, 0, 1,1,'O','OO','')");
	keys.push_back("INSERT INTO KEYS_CLASSIC VALUES( 80,'P', 0, 0, 0, 0, 0,0,'P','','')");
	keys.push_back("INSERT INTO KEYS_CLASSIC VALUES( 81,'Q', 0, 0, 0, 0, 0,0,'Q','','')");
	keys.push_back("INSERT INTO KEYS_CLASSIC VALUES( 82,'R', 0, 0, 0, 0, 0,0,'R','','')");
	keys.push_back("INSERT INTO KEYS_CLASSIC VALUES( 83,'S', 0, 0, 0, 0, 0,0,'S','','')");
	keys.push_back("INSERT INTO KEYS_CLASSIC VALUES( 84,'T', 0, 0, 0, 0, 0,0,'T','','')");
	keys.push_back("INSERT INTO KEYS_CLASSIC VALUES( 85,'U', 1, 1, 1, 0, 1,1,'U','UU','')");
	keys.push_back("INSERT INTO KEYS_CLASSIC VALUES( 86,'V', 0, 0, 0, 0, 0,0,'V','','')");
	keys.push_back("INSERT INTO KEYS_CLASSIC VALUES( 87,'W', 0, 0, 0, 0, 0,0,'W','','')");
	keys.push_back("INSERT INTO KEYS_CLASSIC VALUES( 88,'X', 0, 0, 0, 0, 0,0,'X','','')");
	keys.push_back("INSERT INTO KEYS_CLASSIC VALUES( 89,'Y', 1, 1, 1, 1, 0,0,'Y','','I')");
	keys.push_back("INSERT INTO KEYS_CLASSIC VALUES( 90,'Z', 0, 0, 0, 0, 0,0,'Z','','')");

	std::vector < String > ::iterator it = keys.begin();
	while (it != keys.end())
	{
		d_Query->SQL->Text = *it;

		try
		{
			d_Query->ExecSQL();
			d_SQLite->Commit();
		}
		catch (EDatabaseError &ex)
		{
			ShowMessage("Exeption raised with message : " + ex.Message);
		}
		++it;

	}

}

void TctlDatabase::FillKeysCoverTable()
{
	std::vector < String > keys; // key, normal,   shift, alt, contrl
	keys.push_back("INSERT INTO KEYS_COVER VALUES( 49,'AA', 1, 1, 1, 1, 0,0,'','','1')");
	keys.push_back("INSERT INTO KEYS_COVER VALUES( 50,'EE', 1, 1, 1, 1, 0,0,'','','2')");
	keys.push_back("INSERT INTO KEYS_COVER VALUES( 51,'OO', 1, 1, 0, 1, 0,0,'','','3')");
	keys.push_back("INSERT INTO KEYS_COVER VALUES( 52,'UU', 1, 1, 0, 1, 0,0,'','','4')");
	keys.push_back("INSERT INTO KEYS_COVER VALUES( 53,'AU', 1, 1, 0, 1, 0,0,'','','5')");
	keys.push_back("INSERT INTO KEYS_COVER VALUES( 54,'EI', 1, 1, 0, 1, 0,0,'','','6')");
	keys.push_back("INSERT INTO KEYS_COVER VALUES( 55,'EU', 1, 1, 0, 1, 0,0,'','','7')");
	keys.push_back("INSERT INTO KEYS_COVER VALUES( 56,'IE', 1, 1, 0, 1, 0,0,'','','8')");
	keys.push_back("INSERT INTO KEYS_COVER VALUES( 57,'IJ', 1, 1, 1, 1, 0,0,'','','9')");
	keys.push_back("INSERT INTO KEYS_COVER VALUES( 48,'OE', 1, 1, 1, 1, 0,0,'','','0')");
	keys.push_back("INSERT INTO KEYS_COVER VALUES( 219,'OU', 1, 1, 0, 0, 0,0,'','','')");
	keys.push_back("INSERT INTO KEYS_COVER VALUES( 221,'UI', 1, 1, 0, 0, 0,0,'','','')");
	keys.push_back("INSERT INTO KEYS_COVER VALUES( 186,'NG', 1, 1, 0, 0, 0,0,'','','')");
	keys.push_back("INSERT INTO KEYS_COVER VALUES( 189,'EUR', 1, 1, 0, 1, 0,0,'','','')");
	keys.push_back("INSERT INTO KEYS_COVER VALUES( 191,'OOR', 1, 1, 1, 0, 0,0,'','','')");
	keys.push_back("INSERT INTO KEYS_COVER VALUES( 220,'CH', 1, 1, 0, 1, 0,0,'','','SJ')");
	keys.push_back("INSERT INTO KEYS_COVER VALUES( 165,'EER', 1, 1, 0, 0, 1,1,'','','')");
	keys.push_back("INSERT INTO KEYS_COVER VALUES( 222,'NK', 1, 1, 1, 1, 0,0,'','','')");
	keys.push_back("INSERT INTO KEYS_COVER VALUES( 187,'SCH', 1, 1, 1, 1, 0,0,'','','')");
	keys.push_back("INSERT INTO KEYS_COVER VALUES( 45,'AAI', 1, 1, 0, 1, 0,0,'','','')");
	keys.push_back("INSERT INTO KEYS_COVER VALUES( 36,'OOI', 1, 1, 0, 1, 0,0,'','','')");
	keys.push_back("INSERT INTO KEYS_COVER VALUES( 33,'OEI', 1, 1, 0, 1, 0,0,'','','')");
	keys.push_back("INSERT INTO KEYS_COVER VALUES( 46,'EEUW', 1, 1, 0, 1, 0,0,'','','')");
	keys.push_back("INSERT INTO KEYS_COVER VALUES( 35,'IEUW', 1, 1, 0, 1, 0,0,'','','')");
	keys.push_back("INSERT INTO KEYS_COVER VALUES( 34,'UW', 1, 1, 0, 1, 0,0,'','','')");
	keys.push_back("INSERT INTO KEYS_COVER VALUES( 65,'A', 1, 0, 1, 1, 1,1,'A','AA','')");
	keys.push_back("INSERT INTO KEYS_COVER VALUES( 66,'B', 0, 0, 0, 0, 0,0,'B','','')");
	keys.push_back("INSERT INTO KEYS_COVER VALUES( 67,'C', 1, 1, 1, 1, 0,0,'C','','K')");
	keys.push_back("INSERT INTO KEYS_COVER VALUES( 68,'D', 1, 1, 1, 1, 0,0,'D','','D')");
	keys.push_back("INSERT INTO KEYS_COVER VALUES( 69,'E', 1, 1, 1, 1, 1,1,'E','EE','U')");
	keys.push_back("INSERT INTO KEYS_COVER VALUES( 70,'F', 0, 0, 0, 0, 0,0,'F','','')");
	keys.push_back("INSERT INTO KEYS_COVER VALUES( 71,'G', 0, 0, 0, 0, 0,0,'G','','')");
	keys.push_back("INSERT INTO KEYS_COVER VALUES( 72,'H', 0, 0, 0, 0, 0,0,'H','','')");
	keys.push_back("INSERT INTO KEYS_COVER VALUES( 73,'I', 1, 1, 1, 1, 1,1,'II','IE','I')");
	keys.push_back("INSERT INTO KEYS_COVER VALUES( 74,'J', 0, 0, 0, 0, 0,0,'J','','')");
	keys.push_back("INSERT INTO KEYS_COVER VALUES( 75,'K', 0, 0, 0, 0, 0,0,'K','','')");
	keys.push_back("INSERT INTO KEYS_COVER VALUES( 76,'L', 0, 0, 0, 0, 0,0,'L','','')");
	keys.push_back("INSERT INTO KEYS_COVER VALUES( 77,'M', 0, 0, 0, 0, 0,0,'M','','')");
	keys.push_back("INSERT INTO KEYS_COVER VALUES( 78,'N', 0, 0, 0, 0, 0,0,'N','','')");
	keys.push_back("INSERT INTO KEYS_COVER VALUES( 79,'O', 1, 1, 1, 0, 1,1,'O','OO','')");
	keys.push_back("INSERT INTO KEYS_COVER VALUES( 80,'P', 0, 0, 0, 0, 0,0,'P','','')");
	keys.push_back("INSERT INTO KEYS_COVER VALUES( 81,'Q', 0, 0, 0, 0, 0,0,'Q','','')");
	keys.push_back("INSERT INTO KEYS_COVER VALUES( 82,'R', 0, 0, 0, 0, 0,0,'R','','')");
	keys.push_back("INSERT INTO KEYS_COVER VALUES( 83,'S', 0, 0, 0, 0, 0,0,'S','','')");
	keys.push_back("INSERT INTO KEYS_COVER VALUES( 84,'T', 0, 0, 0, 0, 0,0,'T','','')");
	keys.push_back("INSERT INTO KEYS_COVER VALUES( 85,'U', 1, 1, 1, 0, 1,1,'U','UU','')");
	keys.push_back("INSERT INTO KEYS_COVER VALUES( 86,'V', 0, 0, 0, 0, 0,0,'V','','')");
	keys.push_back("INSERT INTO KEYS_COVER VALUES( 87,'W', 0, 0, 0, 0, 0,0,'W','','')");
	keys.push_back("INSERT INTO KEYS_COVER VALUES( 88,'X', 0, 0, 0, 0, 0,0,'X','','')");
	keys.push_back("INSERT INTO KEYS_COVER VALUES( 89,'Y', 1, 1, 1, 1, 0,0,'Y','','I')");
	keys.push_back("INSERT INTO KEYS_COVER VALUES( 90,'Z', 0, 0, 0, 0, 0,0,'Z','','')");
	std::vector < String > ::iterator it = keys.begin();
	while (it != keys.end())
	{
		d_Query->SQL->Text = *it;

		try
		{
			d_Query->ExecSQL();
			d_SQLite->Commit();
		}
		catch (EDatabaseError &ex)
		{
			ShowMessage("Exeption raised with message : " + ex.Message);
		}
		++it;

	}

}

void TctlDatabase::Connect()
{
	try
	{
		d_SQLite->Open();
	}
	catch (EDatabaseError &ex)
	{
		ShowMessage("Exeption raised with message : " + ex.Message);
	}

}

void TctlDatabase::Disconnect()
{
	if (d_SQLite->Connected)
	{
		d_SQLite->Close();
	}
}

// ---------------------------------------------------------------------------
void __fastcall TctlDatabase::SQLiteAfterConnect(TObject *Sender)
{
	// lblConnectionStatus->Text = "Status : Connected!";
}
// ---------------------------------------------------------------------------

void __fastcall TctlDatabase::SQLiteAfterDisconnect(TObject *Sender)
{
	// lblConnectionStatus->Text = "Status : Disconnected!";
}
