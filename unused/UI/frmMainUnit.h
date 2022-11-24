// ---------------------------------------------------------------------------

#ifndef frmMainUnitH
#define frmMainUnitH
// ---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Menus.hpp>
#include <System.ImageList.hpp>
#include <Vcl.ImgList.hpp>
// ---------------------------------------------------------------------------
#include "lib/cfgmgr32.h"
#include "Settings.h"
// ---------------------------------------------------------------------------
#include "frmReadingPaneUnit.h"
#include "frmTrayIconUnit.h"
// ---------------------------------------------------------------------------
#include <Vcl.ComCtrls.hpp>
#include <Vcl.AppEvnts.hpp>
#include <System.Win.TaskbarCore.hpp>
#include <Vcl.Taskbar.hpp>
// ---------------------------------------------------------------------------

int const WM_SHOW_KEY     = WM_USER + 4;
int const WM_CTRL_KEY     = WM_USER + 5;
int const WM_ALT_KEY      = WM_USER + 6;
int const WM_SHIFT_KEY    = WM_USER + 7;
int const WM_PASS_GETTEXT = WM_USER + 8;

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);

struct TKBDLLHOOKSTRUCT
{
	DWORD     vkCode;
	DWORD     scanCode;
	DWORD     flags;
	DWORD     time;
	ULONG_PTR dwExtraInfo;
};

struct TCurrentConfig
{
	bool Sound;
	bool Sounds;
	bool TTS;
   bool Paused;
	bool ActivateAfterWords;
	bool SelectionAndSpeaker;
	bool StartWithWindows;
	bool ReadAlong;

	bool KeboardConnected;

	int    Volume;
	int    ReadingSpeed;
	double ReadingSpeedFactor;

	int CoverType;

};

// String GetText(HWND handle);

class Speech;

// ---------------------------------------------------------------------------
// #include "lib/ve_engine_v2.0.0_win32/inc/ve_platform.h"
// #include "lib/ve_engine_v2.0.0_win32/inc/ve_ttsapi.h"
// ---------------------------------------------------------------------------
class TfrmMain :
	public TForm
{
	String d_dataDir;

public:
	volatile bool bSkipKeyStroke;

private:

	volatile bool d_ctrl;
	volatile bool d_shift;

	Speech *d_TTS;

	lokiCpp::TSettings *d_settings;

	// TctlSelfElevation *d_selfElevate;
	TfrmTrayIcon *d_frmTrayIcon;

	bool d_terminate;

	NOTIFYICONDATA nid;
	HWND           hwnd;

__published: // IDE-managed Components

	TTrayIcon *         TrayIcon;
	TPopupMenu *        PopupMenu;
	TMenuItem *         mnuSound;
	TMenuItem *         mnuSounds;
	TMenuItem *         mnuTTS;
	TMenuItem *         mnuSettings;
	TMenuItem *         mnuManual;
	TMenuItem *         mnuClose;
	TImageList *        ImageList1;
	TMenuItem *         N1;
	TGroupBox *         gbOtherSettings;
	TGroupBox *         gbSoundSettings;
	TTrackBar *         tbVolume;
	TGroupBox *         gbTextToSpeech;
	TTrackBar *         tbSpeed;
	TCheckBox *         chkSound;
	TCheckBox *         chkSounds;
	TCheckBox *         chkTTS;
	TCheckBox *         chkWord;
	TCheckBox *         chkSelectionSpeakerKey;
	TLabel *            lblActivateAfter;
	TCheckBox *         chkAutomaticBoot;
	TButton *           cmdSave;
	TButton *           cmdApply;
	TButton *           cmdCancel;
	TApplicationEvents *ApplicationEvents1;
	TRadioButton *      rbClassic;
	TRadioButton *      rbCover;
	TCheckBox *         chkReadAlong;
	TLabel *            lblVolume;
	TLabel *            lblSpeed;
	TLabel *            lblVersion;
	TMenuItem *N2;
	TMenuItem *mnuPaused;
	TCheckBox *chkPaused;

	void __fastcall PopupMenuItemClick(TObject *Sender);
	void __fastcall InstellingenClick(TObject *Sender);
	void __fastcall HandleidingClick(TObject *Sender);
	void __fastcall AfsluitenClick(TObject *Sender);
	void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
	void __fastcall cmdSaveClick(TObject *Sender);
	void __fastcall ApplicationEvents1Idle(TObject *Sender, bool &Done);
	void __fastcall cmdApplyClick(TObject *Sender);
	void __fastcall cmdCancelClick(TObject *Sender);
	void __fastcall chkTTSClick(TObject *Sender);
	void __fastcall TrayIconClick(TObject *Sender);
	void __fastcall TrayIconDblClick(TObject *Sender);
	void __fastcall chkSoundClick(TObject *Sender);
	void __fastcall chkSoundsClick(TObject *Sender);
	void __fastcall chkWordClick(TObject *Sender);
	void __fastcall tbVolumeChange(TObject *Sender);
	void __fastcall PopupMenuPopup(TObject *Sender);

private: // User declarations
	void Init();

	void __fastcall WndProc(TMessage &Message);
	void __fastcall HandleConnectDisconnect();
	bool ParseSubNodes(PDEVINST parent, DEVINST hnode);

	void LoadSettings();
	void SaveSettings();

	void ClearSpeech();

	TfrmReadingPane *d_readingPane;

	void UpdateIcon();

public:
	TCurrentConfig d_config;

   void StopProcessing();

	bool ProcessChar(wchar_t value);
	void ProcessText(String value, bool ignoreKeboardStatus = false); // Read selection key
	void ProcessWord(String value, bool ignoreKeboardStatus = false); // read along words
	bool CheckKey(int key, bool ctrl, bool shft, bool alt, bool IgnoreKeyStatus = false);

	void ReadTextSelection();

private:
	void UpdateConfiguration();

//	String GetText(HWND hndl);

public: // User declarations

	__fastcall TfrmMain(TComponent *Owner);
	__fastcall ~TfrmMain();
};

// ---------------------------------------------------------------------------
extern PACKAGE TfrmMain *frmMain;
// ---------------------------------------------------------------------------
#endif
