// ---------------------------------------------------------------------------

#include <vcl.h>
#include <Vcl.Clipbrd.hpp>
#include <mmsystem.h>
// #include <windows.h>
#pragma hdrstop

#include "Data.h"
#include "Debug.h"
#include "Speech.h"
#include "frmMainUnit.h"
// ---------------------------------------------------------------------------
#include "Language.h"
// ---------------------------------------------------------------------------
#include <Dbt.h>
#include <vector>
#include <algorithm>

#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmMain *frmMain;

typedef lokiCpp::TLanguage LANG;

HHOOK keyboardHook;

#include <thread>
#include <mutex>
std::mutex g_mutex;

// ---------------------------------------------------------------------------
__fastcall TfrmMain::TfrmMain(TComponent * Owner)
	: TForm(Owner),
	  d_settings(lokiCpp::TSettings::Instance()),
	// d_selfElevate(new TctlSelfElevation()),
	d_frmTrayIcon(nullptr),
	d_terminate(false),
	d_layoutSelection(nullptr),
	d_readingPane(new TfrmReadingPane(nullptr)),
	bSkipKeyStroke(false),
	d_ctrl(false),
	d_shift(false),
	d_TTS(nullptr)
{

	d_dataDir = IncludeTrailingBackslash(ExtractFilePath(Application->ExeName)) + "data\\";

	// unregister
	// regsvr32 /u “path & filename of dll or ocx“
}

// MediaPlayer1->FileName = PATH + "0.wav";

// hwnd = FindWindow(0, L"Form4");
// nid.cbSize = sizeof(NOTIFYICONDATA);
// nid.hWnd = hwnd;
// nid.uID = 100;
// nid.uVersion = NOTIFYICON_VERSION;
// //nid.uCallbackMessage = WM_TRAYICONCLICK;
//
// // TResourceHandle hnd = new TResourceHandle("Icon_1", RT_ICON); // RT_RCDATA is the Resource Type.
// nid.hIcon = (HICON)LoadImage(GetModuleHandle(nullptr), L"favicon.ico", IMAGE_ICON, SM_CXICON, SM_CYICON, LR_SHARED | LR_LOADFROMFILE);
// // nid.hIcon = (HICON)LoadIcon(GetModuleHandle(nullptr), MAKEINTRESOURCE(ICO1));
// nid.hIcon = (HICON)LoadImage(GetModuleHandle(nullptr), L"ico.ico", IMAGE_ICON, SM_CXICON, SM_CYICON, LR_SHARED | LR_LOADFROMFILE);
//
// //wcscpy_s(nid.szTip, 256, L"Tray Icon");
// nid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
// Shell_NotifyIconW(NIM_ADD, &nid);
// Shell_NotifyIconW(NIM_ADD, &nid);
// }

// String GetText(HWND handle)
// {
// int maxLength = 255;
//
// wchar_t *buffer;
// buffer = (wchar_t *)malloc(maxLength + 1);
// PostMessage(handle, WM_GETTEXT, maxLength, LPARAM(buffer));
//
// String w = buffer; // Marshal.PtrToStringUni(buffer);
//
// PostMessage(frmMain->Handle, WM_PASS_GETTEXT, maxLength, LPARAM(buffer));
//
// // free(buffer);
//
// // Marshal.FreeHGlobal(buffer);
// return w;
// }

void TfrmMain::ReadTextSelection()
{
	TClipboard *cb = Clipboard();
	if (cb->HasFormat(CF_TEXT))
	{
		try
		{
			ProcessText(cb->AsText);
		}
		catch (...)
		{
		}
	}
}

// ---------------------------------------------------------------------------
LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	DebugTimer dt(TEXT("LowLevelKeyboardProc()"));

	static std::vector < wchar_t > chars;
	volatile bool keyboardDisconnected = !frmMain->d_config.KeboardConnected;
	volatile bool NoSound              = !frmMain->d_config.Sound;
	volatile bool NoSounds             = !frmMain->d_config.Sounds;
	volatile bool Pause                = frmMain->d_config.Paused;

	if (nCode == HC_ACTION)
	{
		TKBDLLHOOKSTRUCT *obj = reinterpret_cast < TKBDLLHOOKSTRUCT * > (lParam);

		DebugLog(TEXT("LowLevelKeyboardProc(nCode = %d, wParam = %d, lParam = %d) vkCode = %d"), nCode, wParam, lParam, obj->vkCode);

		volatile static bool win  = false;
		volatile static bool ctrl = false;
		volatile static bool shft = false;
		volatile static bool alt  = false;

		volatile static bool allowControlToPass = false;

		switch (wParam)
		{
		case WM_SYSKEYDOWN:
		case WM_KEYDOWN:
			if (keyboardDisconnected || NoSound || NoSounds || Pause)
				break;

			if (!win && obj->vkCode == VK_LWIN)
			{
				win = true;
				// keybd_event(VK_LCONTROL, 0, 0, 0);     // Alt Press
				// keybd_event(0x43, 0, 0, 0); // Tab Press
				// keybd_event(0x43, 0, KEYEVENTF_KEYUP, 0); // Tab Release
				// keybd_event(VK_LCONTROL, 0, KEYEVENTF_KEYUP, 0); // Alt Release

				tagINPUT ti;
				ti.type = INPUT_KEYBOARD;
				// ti.ki.wScan       = 0; // hardware scan code for key
				// ti.ki.time        = 0;
				// ti.ki.dwExtraInfo = 0;
				ti.ki.dwFlags = 0; // 0 for keypress
				ti.ki.wVk     = VK_LCONTROL;
				ti.ki.wScan   = MapVirtualKey(ti.ki.wVk, 1);
				SendInput(1, &ti, sizeof(INPUT));

				ti.type = INPUT_KEYBOARD;
				// ti.ki.wScan       = 0; // hardware scan code for key
				// ti.ki.time        = 0;
				// ti.ki.dwExtraInfo = 0;
				ti.ki.dwFlags = 0; // 0 for keypress
				ti.ki.wVk     = 0x43;
				ti.ki.wScan   = MapVirtualKey(ti.ki.wVk, 1);
				SendInput(1, &ti, sizeof(INPUT));

				Sleep(25);

				ti.type = INPUT_KEYBOARD;
				// ti.ki.wScan       = 0; // hardware scan code for key
				// ti.ki.time        = 0;
				// ti.ki.dwExtraInfo = 0;
				ti.ki.dwFlags = KEYEVENTF_KEYUP; // 0 for keypress
				ti.ki.wVk     = 0x43;
				// ti.ki.wScan = MapVirtualKey(ti.ki.wVk, 1);
				SendInput(1, &ti, sizeof(INPUT));

				ti.type = INPUT_KEYBOARD;
				// ti.ki.wScan       = 0; // hardware scan code for key
				// ti.ki.time        = 0;
				// ti.ki.dwExtraInfo = 0;
				ti.ki.dwFlags = KEYEVENTF_KEYUP; // 0 for keypress
				ti.ki.wVk     = VK_LCONTROL;
				// ti.ki.wScan   = MapVirtualKey(ti.ki.wVk, 1);
				SendInput(1, &ti, sizeof(INPUT));
				Sleep(25);

				win     = false;
				ti.type = INPUT_KEYBOARD;
				// ti.ki.wScan       = 0; // hardware scan code for key
				// ti.ki.time        = 0;
				// ti.ki.dwExtraInfo = 0;
				ti.ki.dwFlags = KEYEVENTF_KEYUP; // 0 for keypress
				ti.ki.wVk     = VK_LCONTROL;
				// ti.ki.wScan   = MapVirtualKey(ti.ki.wVk, 1);
				SendInput(1, &ti, sizeof(INPUT));
				Sleep(25);

				// unsigned char* keystate;
				// GetKeyboardState(keystate);

				// for (int idx = 0; idx != 256; ++idx)
				// {
				// keystate[idx] = 0;
				// }

				// keystate[VK_LSHIFT] = 0;
				// keystate[VK_LALT]  = 0;

				// SetKeyboardState(keystate);
				Application->ProcessMessages();

				win = true;

				// try
				// {
				// HWND activeWin = GetForegroundWindow();
				// // int activeWinPtr    = GetForegroundWindow()->ToInt32();
				// DWORD activeThreadId;
				// LPDWORD processId;
				// activeThreadId      = GetWindowThreadProcessId(activeWin, processId);
				// int currentThreadId = GetCurrentThreadId();
				// if (activeThreadId != currentThreadId)
				// AttachThreadInput(activeThreadId, currentThreadId, true);
				// HWND activeCtrlId = GetFocus();
				//
				// String a = GetText(activeCtrlId);

				// frmMain->ProcessText("Ludo snapt het niet meer..");
				// }
				// catch (Exception &exp)
				// {
				// String ludo = exp.Message;
				// int    jorn = 0;
				// }

				return 1;
			}

			if (win == true)
			{
				return 0;
			}
			else if (obj->vkCode == VK_LCONTROL && !allowControlToPass)
			{
				ctrl = true;
				PostMessage(frmMain->Handle, WM_CTRL_KEY, wParam, obj->vkCode);
				return 1;
			}
			else if (obj->vkCode == VK_SHIFT || obj->vkCode == VK_LSHIFT || obj->vkCode == VK_RSHIFT)
			{
				shft = true;
				PostMessage(frmMain->Handle, WM_SHIFT_KEY, wParam, obj->vkCode);
				// return 1;
			}
			else if (obj->vkCode == 0x45)
			{
				// alt = true;
				// // SendMessage(frmMain->Handle, WM_ALT_KEY, wParam, lParam);
				// return 1;
			}
			else if (obj->vkCode == VK_MENU /* official alt key code */ || obj->vkCode == 0xA4 || obj->vkCode == 0xA5)
			{
				// alt = false;
				PostMessage(frmMain->Handle, WM_ALT_KEY, wParam, lParam);
				// return 1;
			}

			if (ctrl &&
				(obj->vkCode == VK_PRIOR ||  // PAGE UP
					obj->vkCode == VK_NEXT || // PAGE DOWN
					obj->vkCode == VK_HOME ||
					obj->vkCode == VK_END ||
					obj->vkCode == VK_INSERT ||
					obj->vkCode == VK_DELETE))
			{
				return 0;
			}

			if (obj->vkCode == VK_SPACE)
			{
				SendMessage(frmMain->Handle, WM_SHOW_KEY, WM_KEYUP, obj->vkCode);
				// return 1;
			}
			if (frmMain->CheckKey(obj->vkCode, ctrl, shft, alt))
			{
				PostMessage(frmMain->Handle, WM_SHOW_KEY, WM_KEYUP, obj->vkCode);
				return 1;
			}
			else
			{
				if (!frmMain->CheckKey(obj->vkCode, ctrl, shft, alt, true) && ctrl)
				{
					win = true;
					// keybd_event(VK_LCONTROL, 0, 0, 0);     // Alt Press
					// keybd_event(0x43, 0, 0, 0); // Tab Press
					// keybd_event(0x43, 0, KEYEVENTF_KEYUP, 0); // Tab Release
					// keybd_event(VK_LCONTROL, 0, KEYEVENTF_KEYUP, 0); // Alt Release

					tagINPUT ti;
					ti.type = INPUT_KEYBOARD;
					// ti.ki.wScan       = 0; // hardware scan code for key
					// ti.ki.time        = 0;
					// ti.ki.dwExtraInfo = 0;
					ti.ki.dwFlags = 0; // 0 for keypress
					ti.ki.wVk     = VK_LCONTROL;
					ti.ki.wScan   = MapVirtualKey(ti.ki.wVk, 1);
					SendInput(1, &ti, sizeof(INPUT));

					ti.type = INPUT_KEYBOARD;
					// ti.ki.wScan       = 0; // hardware scan code for key
					// ti.ki.time        = 0;
					// ti.ki.dwExtraInfo = 0;
					ti.ki.dwFlags = 0; // 0 for keypress
					ti.ki.wVk     = nCode;
					ti.ki.wScan   = MapVirtualKey(ti.ki.wVk, 1);
					SendInput(1, &ti, sizeof(INPUT));

					Sleep(25);

					ti.type = INPUT_KEYBOARD;
					// ti.ki.wScan       = 0; // hardware scan code for key
					// ti.ki.time        = 0;
					// ti.ki.dwExtraInfo = 0;
					ti.ki.dwFlags = KEYEVENTF_KEYUP; // 0 for keypress
					ti.ki.wVk     = nCode;
					// ti.ki.wScan = MapVirtualKey(ti.ki.wVk, 1);
					SendInput(1, &ti, sizeof(INPUT));

					ti.type = INPUT_KEYBOARD;
					// ti.ki.wScan       = 0; // hardware scan code for key
					// ti.ki.time        = 0;
					// ti.ki.dwExtraInfo = 0;
					ti.ki.dwFlags = KEYEVENTF_KEYUP; // 0 for keypress
					ti.ki.wVk     = VK_LCONTROL;
					// ti.ki.wScan   = MapVirtualKey(ti.ki.wVk, 1);
					SendInput(1, &ti, sizeof(INPUT));
					Sleep(25);

					win = false;
					return 0;
				}
			}

			break;
		case WM_KEYUP:
			if (keyboardDisconnected || NoSound || Pause)
				break;

			if (obj->vkCode == VK_LWIN)
			{
				win = false;
				frmMain->ReadTextSelection();
				PostMessage(frmMain->Handle, VK_LCONTROL, WM_KEYUP, obj->vkCode);

				return 1;
			}
			if (win)
				return 1;

			if (obj->vkCode == VK_LCONTROL)
			{
				ctrl = false;
				PostMessage(frmMain->Handle, WM_CTRL_KEY, wParam, obj->vkCode);
				// return 1;
			}
			else if (obj->vkCode == VK_SHIFT || obj->vkCode == VK_LSHIFT || obj->vkCode == VK_RSHIFT)
			{
				shft = false;
				PostMessage(frmMain->Handle, WM_SHIFT_KEY, wParam, obj->vkCode);
				// return 1;
			}
			// if (obj->vkCode == VK_MENU)
			// {
			// // alt = false;
			// PostMessage(frmMain->Handle, WM_ALT_KEY, wParam, lParam);
			// // return 1;
			// }

			if (frmMain->CheckKey(obj->vkCode, ctrl, shft, alt))
			{
				return 0; // PostMessage(frmMain->Handle, WM_SHOW_KEY, wParam, obj->vkCode);
			}

			break;
			// case WM_SYSKEYDOWN:
			// // return 1;
			// break;
		case WM_SYSKEYUP:
			// return 1;
			// frmMain->Memo1->Lines->Add("syskey : " + IntToStr((__int64)obj->vkCode));
			// SendMessage(frmMain->Handle, WM_SHOW_KEY, WM_KEYUP, lParam);

			break;
		}
	}

	return CallNextHookEx(keyboardHook, nCode, wParam, lParam);
}

bool TfrmMain::CheckKey(int key, bool ctrl, bool shft, bool alt, bool IgnoreKeyStatus)
{
	if (bSkipKeyStroke)
		return IgnoreKeyStatus;

	String value = GetSoundForKey((int)key, d_ctrl, d_shift, alt, rbCover->Checked);
	if (value.IsEmpty()) // || value.CompareIC(key) == 0)
			return false;
	else
		return true;
}

__fastcall TfrmMain::~TfrmMain()
{
	if (d_layoutSelection != nullptr)
	{
		delete d_layoutSelection;
	}
	if (d_readingPane != nullptr)
	{
		delete d_readingPane;
		d_readingPane = nullptr;
	}
	// delete d_selfElevate;
}

void TfrmMain::Init()
{
	Caption = LANG::Translate(ClassName(), this->Name);
	String tmp;
	d_settings->GetConfig("version", "Version", tmp);
	lblVersion->Caption = tmp;

	chkSound->Caption  = LANG::Translate(ClassName(), chkSound->Name);
	chkSounds->Caption = LANG::Translate(ClassName(), chkSounds->Name);
	chkPaused->Caption = LANG::Translate(ClassName(), chkPaused->Name);
	chkTTS->Caption    = LANG::Translate(ClassName(), chkTTS->Name);

	lblActivateAfter->Caption       = LANG::Translate(ClassName(), lblActivateAfter->Name);
	chkWord->Caption                = LANG::Translate(ClassName(), chkWord->Name);
	chkSelectionSpeakerKey->Caption = LANG::Translate(ClassName(), chkSelectionSpeakerKey->Name);
	chkReadAlong->Caption           = LANG::Translate(ClassName(), chkReadAlong->Name);

	chkAutomaticBoot->Caption = LANG::Translate(ClassName(), chkAutomaticBoot->Name);

	mnuSound->Caption    = LANG::Translate(ClassName(), mnuSound->Name);
	mnuSounds->Caption   = LANG::Translate(ClassName(), mnuSounds->Name);
	mnuPaused->Caption   = LANG::Translate(ClassName(), mnuPaused->Name);
	mnuTTS->Caption      = LANG::Translate(ClassName(), mnuTTS->Name);
	mnuSettings->Caption = LANG::Translate(ClassName(), mnuSettings->Name);
	mnuManual->Caption   = LANG::Translate(ClassName(), mnuManual->Name);
	mnuClose->Caption    = LANG::Translate(ClassName(), mnuClose->Name);

	cmdSave->Caption   = LANG::Translate(ClassName(), cmdSave->Name);
	cmdApply->Caption  = LANG::Translate(ClassName(), cmdApply->Name);
	cmdCancel->Caption = LANG::Translate(ClassName(), cmdCancel->Name);

	lblVolume->Caption = LANG::Translate(ClassName(), lblVolume->Name);
	lblSpeed->Caption  = LANG::Translate(ClassName(), lblSpeed->Name);

	rbClassic->Caption = LANG::Translate(ClassName(), rbClassic->Name);
	rbCover->Caption   = LANG::Translate(ClassName(), rbCover->Name);

	gbSoundSettings->Caption = LANG::Translate("Configuration", gbSoundSettings->Name);
	gbTextToSpeech->Caption  = LANG::Translate("Configuration", gbTextToSpeech->Name);
	gbOtherSettings->Caption = LANG::Translate("Configuration", gbOtherSettings->Name);

	HandleConnectDisconnect();
	keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, HInstance, 0);

	d_TTS = new Speech();
	d_TTS->Init("data/tts/", "nl", "Ilse");

	/*
	try
	{
		d_TTS = rstts::TRSpeakTTS::Instance(); // new Ssftrssololib_tlb::TSsftRSSoloA(this);
		// when will this fail ? we only need to register if it is not registered...
		// BSTR s = SysAllocString(L"");
		// SsftRSSoloA1->InitSpeech(s);
		// SysFreeString(s);
		// SsftRSSoloA1->LangName = "Dutch";
		d_TTS->SetLanguage("nl");
		d_TTS->SetVoice("ilse");
		// SsftRSSoloA1->LangName = "Dutch"; // "";
		// SsftRSSoloA1->Volume = 40;

	}
	// catch (EOleSysError &ex)
	// {
	// // if (d_selfElevate->UserIsInAdminGroup)
	// // {
	// // if (d_selfElevate->RunAsAdmin || d_selfElevate->ProcessIsElevated)
	// // {
	// // ShellExecute(0, L"open", L"regsvr32.exe",
	// // L"speech\\components\\common\\ssftrssolo.ocx", L"/s", 0); // SW_NORMAL);
	// // try
	// // {
	// // SsftRSSoloA1 = new Ssftrssololib_tlb::TSsftRSSoloA(this);
	// // SsftRSSoloA1->InitSpeech(L"");
	// // SsftRSSoloA1->LangName = "Dutch";
	// // // SsftRSSoloA1->LangName = "Dutch"; // "";
	// // SsftRSSoloA1->Volume = 40;
	// // }
	// // catch (Exception &ex)
	// // {
	// // int ludo = 0;
	// // }
	// // }
	// // else
	// // {
	// // d_selfElevate->SelfElevateClick(Handle);
	// // d_terminate = true;
	// // Close();
	// // }
	// // }
	// }
	catch (Exception &ex)
	{
		ShowMessage(ex.Message);
	}
	*/

	LoadSettings();
}

// ---------------------------------------------------------------------------
void __fastcall TfrmMain::PopupMenuItemClick(TObject *Sender)
{

	TMenuItem *ClickedItem = dynamic_cast < TMenuItem * > (Sender);

	if (ClickedItem != nullptr)
	{
		switch (ClickedItem->Tag)
		{
		case 0:
			chkSound->Checked = ClickedItem->Checked;
			if (d_frmTrayIcon != nullptr)
				d_frmTrayIcon->chkSound->Checked = chkSound->Checked;
			// ShowMessage("0");
			break;
		case 1:
			chkSounds->Checked = ClickedItem->Checked;
			if (d_frmTrayIcon != nullptr)
				d_frmTrayIcon->chkSounds->Checked = chkSounds->Checked;
			// ShowMessage("1");
			break;
		case 2:
			// ShowMessage("2");
			chkTTS->Checked = ClickedItem->Checked;
			if (d_frmTrayIcon != nullptr)
				d_frmTrayIcon->chkTTS->Checked = chkTTS->Checked;
			break;
		case 3:
			// ShowMessage("2");
			chkPaused->Checked = ClickedItem->Checked;
			// if (d_frmTrayIcon != nullptr)
			// d_frmTrayIcon->chkPaused->Checked = chkPaused->Checked;
			break;
		}
	}
	SaveSettings();
	UpdateIcon();
}

void __fastcall TfrmMain::WndProc(TMessage &Message)
{

	int msgCount = 0;
	if (Message.Msg == WM_PASS_GETTEXT)
	{
		if (!chkSelectionSpeakerKey->Checked)
			return;
		// free(Message.LParam);
		String w = (wchar_t *)Message.LParam;

		ProcessWord(w);

		// int ludo = 0;

		return;
	}
	if (Message.Msg == WM_CTRL_KEY)
	{
		switch (Message.WParam)
		{
		case WM_KEYDOWN:
			d_ctrl = true;
			break;
		case WM_KEYUP:
			d_ctrl = false;
			break;
		}
	}
	else if (Message.Msg == WM_SHIFT_KEY)
	{
		switch (Message.WParam)
		{
		case WM_KEYDOWN:
			d_shift = true;
			break;
		case WM_KEYUP:
			d_shift = false;
			break;
		}
	}
	else if (Message.Msg == WM_ALT_KEY)
	{
		DebugTimer dt(TEXT("TfrmMain::WndProc()  WM_ALT_KEY"));
		StopProcessing();
	}
	else if (Message.Msg == WM_SHOW_KEY)
	{
		DebugTimer dt(TEXT("TfrmMain::WndProc()  WM_SHOW_KEY"));

		static std::vector < wchar_t > chars;
		// TKBDLLHOOKSTRUCT *obj = reinterpret_cast < TKBDLLHOOKSTRUCT * > (Message.LParam);

		int vkCode = (int)Message.LParam;

		switch (Message.WParam)
		{
		case WM_KEYDOWN:
			return;

			break;
		case WM_KEYUP:
			// frmMain->Memo1->Lines->Add(IntToStr((__int64)obj->vkCode));

			if (d_readingPane != nullptr)
			{
				wchar_t c = vkCode;
				if (c == 162)
					return;

				// std::lock_guard < std::mutex > guard(g_mutex);
				if (c == ' ' || c == '\r' || c == '\n')
				{

					if (chars.empty())
						return;
					String s = "";
					std::vector < wchar_t > ::const_iterator it = chars.begin();
					while (it != chars.end())
					{
						s += *it;
						++it;
					}
					ProcessWord(s);
					chars.clear();
				}
				else if (c == '\b')
				{
					if (!chars.empty())
						chars.pop_back();
				}
				else
				{
					// else
					// chars.push_back(c);

					if (CheckKey(c, d_ctrl, d_shift, false))
					{
						String value = GetValueForKey((int)c, d_ctrl, d_shift, false).LowerCase();
						for (int idx = 1; idx <= value.Length(); ++idx)
						{
							chars.push_back(value[idx]);
						}
					}

					if (!bSkipKeyStroke)
					{
						if (!ProcessChar(c))
						{
							return;
						}
					}

				}
			}

			return;

			// sndPlaySound(String(PATH + (wchar_t)(obj->vkCode) + ".wav").w_str(),
			// SND_LOOP || SND_ASYNC || SND_NODEFAULT);

			break;
		case WM_SYSKEYDOWN:

			break;
		case WM_SYSKEYUP:
			// frmMain->Memo1->Lines->Add("syskey : " + IntToStr((__int64)obj->vkCode));
			break;
		}

	}
	else if (Message.Msg == WM_DEVICECHANGE)
	{
		switch (Message.WParam)
		{
		case DBT_DEVICEARRIVAL:
			msgCount++ ;
			// StringCchPrintf(strBuff, 256,
			// TEXT("Message %d: DBT_DEVICEARRIVAL\n"), msgCount);
			break;
		case DBT_DEVICEREMOVECOMPLETE:
			msgCount++ ;
			// StringCchPrintf(strBuff, 256,
			// TEXT("Message %d: DBT_DEVICEREMOVECOMPLETE\n"), msgCount);
			break;
		case DBT_DEVNODES_CHANGED:
			msgCount++ ;
			// StringCchPrintf(strBuff, 256,
			// TEXT("Message %d: DBT_DEVNODES_CHANGED\n"), msgCount);
			break;
		default:
			msgCount++ ;
			// StringCchPrintf(strBuff, 256,
			// TEXT("Message %d: WM_DEVICECHANGE message received, value %d unhandled.\n")
			// , msgCount, wParam);
			break;
		}
		HandleConnectDisconnect();
	}

	TForm::WndProc(Message);
}

// ---------------------------------------------------------------------------
void __fastcall TfrmMain::HandleConnectDisconnect()
{
	// Hoe langer de zin of het woord des te meer er wordt uitgesproken....
	// String EindeTekst = L"Einde demo. Bezoek www punt clevvie . nl voor informatie over de volledige versieEinde demo. Bezoek www punt clevvie . nl voor informatie over de volledige versie";
	// SsftRSSoloA1->Speak(EindeTekst.c_str(), enumTextFormat::NormalText);

	HMACHINE hcomp;
	DEVINST hroot;
	DEVINST hfirst;

	CM_Locate_DevNode(&hroot, nullptr, 0);
	CM_Get_Child(&hfirst, hroot, 0);
	if (ParseSubNodes(&hroot, hfirst))
	{
		if (d_config.KeboardConnected == false)
		{
			d_config.KeboardConnected = true;

			ProcessText(LANG::Translate("Actions", "KeyboardConnected"));
		}
	}
	else
	{
		if (d_config.KeboardConnected == true)
		{
			d_config.KeboardConnected = false;

			ProcessText(LANG::Translate("Actions", "KeyboardDisconnected"), true);
		}
	}
#ifdef _DEBUG
	d_config.KeboardConnected = true;
#endif
	UpdateIcon();

}

bool TfrmMain::ParseSubNodes(PDEVINST parent, DEVINST hnode)
{
	bool Result = false;

	DEVINST hsibling, hchild;

	char buf[1024];
	ULONG size;

	String hardwareid = "";

	size = 1024;

	while (!Result && hnode != NULL)
	{
		if (CM_Get_Sibling(&hsibling, hnode, 0) != CR_SUCCESS)
		{
			hsibling = NULL;
		}

		if (CM_Get_DevNode_Registry_Property(hnode, CM_DRP_HARDWAREID, nullptr,
				buf, &size, 0) == CR_SUCCESS)
		{
			// XP doet mixed case, Vista uppercase...
			hardwareid = StrUpper(PChar(buf));
			if ((Pos("USB\\VID_04B4&PID_0101", hardwareid) != 0) ||
				(Pos("USB\\VID_0CD3&PID_320F", hardwareid) != 0) ||
				(Pos("BTHENUM\\DEV_01000141", hardwareid) != 0)) // the bluetooth token is longer but starts with this
			{
				Result = true;
			}

			if (!Result && CM_Get_Child(&hchild, hnode, 0) == CR_SUCCESS)
			{
				Result = ParseSubNodes(&hnode, hchild);
			}
		}
		hnode = hsibling;
	}
	return Result;
}

// ---------------------------------------------------------------------------
void __fastcall TfrmMain::InstellingenClick(TObject *Sender)
{
	// Setting click
	this->Show();
}

// ---------------------------------------------------------------------------
void __fastcall TfrmMain::HandleidingClick(TObject *Sender)
{
	// Handleiding click
	ShellExecute(0, L"open", L"http://www.clevy.nl/dyscover2-handleiding", 0, 0, SW_SHOWNORMAL);
}

// ---------------------------------------------------------------------------
void __fastcall TfrmMain::AfsluitenClick(TObject *Sender)
{
	// Afsluiten click
	TrayIcon->Visible = false;
	d_terminate       = true;
	Close();
}

// ---------------------------------------------------------------------------
void __fastcall TfrmMain::FormCloseQuery(TObject *Sender, bool &CanClose)
{
	CanClose = d_terminate || false;
	Hide();
}

bool TfrmMain::ProcessChar(wchar_t aChar)
{
	DebugTimer dt(TEXT("> TfrmMain::ProcessChar()"));

	// if (value.Trim().Length() == 0)
	// {
	// return;
	// }
	String value    = aChar;
	bool   retValue = true;

	if (d_TTS == nullptr || !d_config.Sound || !d_config.KeboardConnected || d_config.Paused)
	{
		return retValue;
	}

	if (d_config.Sounds)
	{
		// link the value to a sound
		value         = GetSoundForKey((int)aChar, d_ctrl, d_shift, false, rbCover->Checked).LowerCase();
		String value2 = GetValueForKey((int)aChar, d_ctrl, d_shift, false, rbCover->Checked).LowerCase();

		if (!value.IsEmpty() && !value2.IsEmpty())
		{
			bSkipKeyStroke = true;
			// TKeyboardState ks;
			INPUT ti; // = new tagINPUT();
			// reset keyboardstate om gewone toets te kunnen emuleren
			// GetKeyboardState(ks);
			// ks[VK_CONTROL]  = 0;
			// ks[VK_LCONTROL] = 0;
			// ks[VK_RCONTROL] = 0;
			// ks[VK_MENU]     = 0;
			// ks[VK_LMENU]    = 0;
			// ks[VK_RMENU]    = 0;
			// SetKeyboardState(ks);

			for (int idx = 1; idx != value2.Length() + 1; ++idx)
			{
				// press the key
				ti.type = INPUT_KEYBOARD;
				// ti.ki.wScan       = 0; // hardware scan code for key
				// ti.ki.time        = 0;
				// ti.ki.dwExtraInfo = 0;
				ti.ki.dwFlags = 0; // 0 for keypress
				ti.ki.wVk     = (int)value2.UpperCase()[idx];
				// ti.ki.wScan = MapVirtualKey(ti.ki.wVk, 1);
				SendInput(1, &ti, sizeof(INPUT));

				Sleep(1); // just commented

				// ti.ki.dwFlags = KEYEVENTF_KEYUP;  // KEYEVENTF_KEYUP for key release
				// SendInput(1, &ti, sizeof(INPUT)); // make sure that two same keys will result in two keystrokes

				// release the key
				// ti.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
				// SendInput(1, &ti, sizeof(tagINPUT));
			}
			bSkipKeyStroke = false;
			retValue = false;

		}

		// tagINPUT ti;
		// ti.type = INPUT_KEYBOARD;
		// ti.ki.dwFlags = 0;
		// ti.ki.wVk = (int)' A ';
		// ti.ki.wScan = MapVirtualKey(ti.ki.wVk,1);
		// SendInput(1,&ti, sizeof(ti));

	}
	ClearSpeech();

	String filename = value + ".wav";
	if (FileExists(d_dataDir + filename))
	{
		sndPlaySound((d_dataDir + filename).c_str(), SND_ASYNC | SND_NODEFAULT | SND_NOSTOP);
	}
	else
	{
		// ClearSpeech();
		// BSTR s = SysAllocString(value.w_str());
		d_TTS->SetVolume(100 - tbVolume->Position);
		d_TTS->SetSpeed(-1 *tbSpeed->Position *d_config.ReadingSpeedFactor);
		// SsftRSSoloA1->Speak(s, enumTextFormat::NormalText);
		d_TTS->Speak(value);
		// SysFreeString(s);
	}

	return retValue; // determines whether the inputted key is ignored (home, ins, del, etc...)
}

void TfrmMain::ProcessText(String value, bool ignoreKeboardStatus)
{
	DebugTimer dt(TEXT("> TfrmMain::ProcessText()"));

	if (value.Trim().Length() == 0)
	{
		return;
	}

	if (d_TTS == nullptr || !d_config.Sound || !d_config.TTS || !(d_config.KeboardConnected || ignoreKeboardStatus) || d_config.Paused)
	{
		return;
	}

	ClearSpeech();
	// BSTR s = SysAllocString(value.w_str());
	// SsftRSSoloA1->Volume = (100 - tbVolume->Position);
	// SsftRSSoloA1->Rate   = (70 - tbSpeed->Position);
	d_TTS->SetVolume(100 - tbVolume->Position);
	d_TTS->SetSpeed(-1 *tbSpeed->Position *d_config.ReadingSpeedFactor);
	// SsftRSSoloA1->Speak(s, enumTextFormat::NormalText);
	d_TTS->Speak(value);
	// SysFreeString(s);
}

void TfrmMain::StopProcessing()
{
	if (d_TTS == nullptr)
	{
		return;
	}

	d_TTS->Stop();
}

void TfrmMain::ProcessWord(String value, bool ignoreKeboardStatus)
{
	DebugTimer dt(TEXT("> TfrmMain::ProcessWord()"));

	if (!d_config.ActivateAfterWords)
		return;

	if (value.Trim().Length() == 0)
	{
		return;
	}

	if (d_TTS == nullptr || !d_config.Sound || !d_config.TTS || !(d_config.KeboardConnected || ignoreKeboardStatus) || d_config.Paused)
	{
		return;
	}

	if (d_config.ReadAlong)
	{
		// wchar_t *buffer;
		// buffer = (wchar_t *)malloc(value.Length() + 1);
		// PostMessage(d_readingPane->Handle, WM_PASS_GETTEXT, value.Length(), LPARAM(buffer));

		d_readingPane->SetCaption(value);
	}

	ClearSpeech();
	// BSTR s = SysAllocString(value.w_str());
	// SsftRSSoloA1->Volume = (100 - tbVolume->Position);
	// SsftRSSoloA1->Rate   = (70 - tbSpeed->Position);
	d_TTS->SetVolume(100 - tbVolume->Position);
	d_TTS->SetSpeed(-1 *tbSpeed->Position *d_config.ReadingSpeedFactor);
	// SsftRSSoloA1->Speak(s, enumTextFormat::NormalText);
	d_TTS->Speak(value);
	// SysFreeString(s);

}

void TfrmMain::ClearSpeech()
{
	sndPlaySound(NULL, SND_ASYNC);

	if (d_TTS != nullptr)
		d_TTS->Stop();
	// if (SsftRSSoloA1->InstanceStatus != (int)enumStatus::Idle)
	// {
	// SsftRSSoloA1->Stop();
	// }
	// while (SsftRSSoloA1->InstanceStatus != (int)enumStatus::Idle)
	// {
	// Sleep(25);
	// }
}

// ---------------------------------------------------------------------------
void __fastcall TfrmMain::cmdSaveClick(TObject *Sender)
{
	SaveSettings();
	// ModalResult = mrOk;
	Hide();
}

// ---------------------------------------------------------------------------
void __fastcall TfrmMain::ApplicationEvents1Idle(TObject *Sender, bool &Done)
{
	ApplicationEvents1->OnIdle = nullptr;
	Init();
}

// ---------------------------------------------------------------------------
void __fastcall TfrmMain::cmdApplyClick(TObject *Sender)
{
	SaveSettings();
}
// ---------------------------------------------------------------------------

void TfrmMain::LoadSettings()
{
	bool   bValue = false;
	double dValue = 0.0;

	d_settings->GetConfig("clevy", "Sound", bValue);
	chkSound->Checked = bValue;
	d_settings->GetConfig("clevy", "Sounds", bValue);
	chkSounds->Checked = bValue;
	d_settings->GetConfig("clevy", "Paused", bValue);
	chkPaused->Checked = bValue;
	d_settings->GetConfig("clevy", "TTS", bValue);
	chkTTS->Checked = bValue;
	d_settings->GetConfig("clevy", "ActivateAfterWords", bValue);
	chkWord->Checked = bValue;
	d_settings->GetConfig("clevy", "SelectionAndSpeaker", bValue);
	chkSelectionSpeakerKey->Checked = bValue;
	d_settings->GetConfig("clevy", "StartWithWindows", bValue);
	chkAutomaticBoot->Checked = bValue;
	d_settings->GetConfig("clevy", "ReadAlong", bValue);
	chkReadAlong->Checked = bValue;
	d_settings->GetConfig("clevy", "Volume", dValue);
	tbVolume->Position = dValue;
	d_settings->GetConfig("clevy", "ReadingSpeed", dValue);
	tbSpeed->Position = dValue;

	d_settings->GetConfig("clevy", "ReadingSpeedFactor", dValue);
	d_config.ReadingSpeedFactor = dValue;

	d_settings->GetConfig("clevy", "CoverType", dValue);
	switch ((int)dValue)
	{
	case 1:
		rbCover->Checked = true;
	case 0:
	default:
		rbClassic->Checked = true;
	}

	UpdateConfiguration();
}

void TfrmMain::SaveSettings()
{
	d_settings->SetConfig(L"clevy", "Sound", chkSound->Checked);

	d_settings->SetConfig(L"clevy", L"Sounds", chkSounds->Checked);
	d_settings->SetConfig(L"clevy", L"Paused", chkPaused->Checked);
	d_settings->SetConfig(L"clevy", L"TTS", chkTTS->Checked);
	d_settings->SetConfig(L"clevy", L"ActivateAfterWords", chkWord->Checked);
	d_settings->SetConfig(L"clevy", L"SelectionAndSpeaker", chkSelectionSpeakerKey->Checked);
	d_settings->SetConfig(L"clevy", L"StartWithWindows", chkAutomaticBoot->Checked);
	d_settings->SetConfig(L"clevy", L"ReadAlong", chkReadAlong->Checked);

	d_settings->SetConfig(L"clevy", L"Volume", (double)tbVolume->Position);
	d_settings->SetConfig(L"clevy", L"ReadingSpeed", (double)tbSpeed->Position);

	d_settings->SetConfig(L"clevy", L"CoverType", rbCover->Checked ? 1. : 0.);

	UpdateConfiguration();

	d_settings->SaveConfig();

	tWAVEFORMATEX WFE;
	HWAVEOUT hWO;

	WFE.wFormatTag      = WAVE_FORMAT_PCM;
	WFE.nChannels       = 2;
	WFE.nSamplesPerSec  = 44100;
	WFE.nAvgBytesPerSec = 176400;
	WFE.nBlockAlign     = 4;
	WFE.wBitsPerSample  = 16;
	WFE.cbSize          = 0;
	waveOutOpen(&hWO, WAVE_MAPPER, &WFE, 0, 0, CALLBACK_NULL);
	// if (FResetVolume == 0)
	// { // als FResetVolume <> 0 is dan hebben we het volume zelf aangepast!
	// waveOutGetVolume(hWO, @ KeyHandler.SystemVolume);
	// }

	WORD vol = (100 - tbVolume->Position) * 0xffff / 100; // (0xffff*tbVolume->Position) + 0xffff*(0xffff*tbVolume->Position);

	if (hWO != nullptr)
	{
		waveOutSetVolume(hWO, vol);
		waveOutClose(hWO);
	}
	// FResetVolume = Now + EncodeTime(0, 0, 1, 500);

	UpdateIcon();
}

void TfrmMain::UpdateConfiguration()
{
	d_config.Sound               = chkSound->Checked;
	d_config.Sounds              = chkSounds->Checked;
	d_config.Paused              = chkPaused->Checked;
	d_config.TTS                 = chkTTS->Checked;
	d_config.ActivateAfterWords  = chkWord->Checked;
	d_config.SelectionAndSpeaker = chkSelectionSpeakerKey->Checked;
	d_config.StartWithWindows    = chkAutomaticBoot->Checked;
	d_config.ReadAlong           = chkReadAlong->Checked;

	d_config.Volume       = (double)tbVolume->Position;
	d_config.ReadingSpeed = (double)tbSpeed->Position;

	d_config.CoverType = rbCover->Checked ? 1. : 0.;

}

void __fastcall TfrmMain::cmdCancelClick(TObject *Sender)
{
	Hide();
}

// ---------------------------------------------------------------------------
void __fastcall TfrmMain::chkTTSClick(TObject *Sender)
{
	gbTextToSpeech->Enabled         = chkTTS->Checked;
	chkWord->Enabled                = gbTextToSpeech->Enabled;
	chkSelectionSpeakerKey->Enabled = gbTextToSpeech->Enabled;
	chkReadAlong->Enabled           = gbTextToSpeech->Enabled;
	UpdateIcon();
}

// ---------------------------------------------------------------------------
void __fastcall TfrmMain::TrayIconClick(TObject *Sender)
{
	if (d_frmTrayIcon == nullptr)
	{
		d_frmTrayIcon       = new TfrmTrayIcon(this);
		d_frmTrayIcon->Left = Screen->PrimaryMonitor->WorkareaRect.Width() - d_frmTrayIcon->Width;
		d_frmTrayIcon->Top  = Screen->PrimaryMonitor->WorkareaRect.Height() - d_frmTrayIcon->Height;
	}
	d_frmTrayIcon->tbVolume->Position = tbVolume->Position * -1;
	d_frmTrayIcon->chkSound->Checked  = chkSound->Checked;
	d_frmTrayIcon->chkSounds->Checked = chkSounds->Checked;
	d_frmTrayIcon->chkTTS->Checked    = chkTTS->Checked;
	d_frmTrayIcon->Show();
}
// ---------------------------------------------------------------------------

void __fastcall TfrmMain::TrayIconDblClick(TObject *Sender)
{
	Show();
	Application->BringToFront();
}
// ---------------------------------------------------------------------------

void TfrmMain::UpdateIcon()
{
	if (!d_config.KeboardConnected || d_config.Paused)
		TrayIcon->IconIndex = 6;
	else if (chkSound->Checked && chkSounds->Checked && chkTTS->Checked)
		TrayIcon->IconIndex = 0;
	else if (chkSound->Checked && chkSounds->Checked && !chkTTS->Checked)
		TrayIcon->IconIndex = 1;
	else if (chkSound->Checked && !chkSounds->Checked && chkTTS->Checked)
		TrayIcon->IconIndex = 2;
	else if (chkSound->Checked && !chkSounds->Checked && !chkTTS->Checked)
		TrayIcon->IconIndex = 3;
	else if (!chkSound->Checked && chkSounds->Checked && !chkTTS->Checked)
		TrayIcon->IconIndex = 4;
	else
		TrayIcon->IconIndex = 5;
}

void __fastcall TfrmMain::chkSoundClick(TObject *Sender)
{
	UpdateIcon();
}
// ---------------------------------------------------------------------------

void __fastcall TfrmMain::chkSoundsClick(TObject *Sender)
{
	UpdateIcon();
}
// ---------------------------------------------------------------------------

void __fastcall TfrmMain::chkWordClick(TObject *Sender)
{
	if (!chkWord->Enabled)
		chkReadAlong->Enabled = false;
}

// ---------------------------------------------------------------------------
void __fastcall TfrmMain::tbVolumeChange(TObject *Sender)
{
	tWAVEFORMATEX WFE;
	HWAVEOUT hWO;

	WFE.wFormatTag      = WAVE_FORMAT_PCM;
	WFE.nChannels       = 2;
	WFE.nSamplesPerSec  = 44100;
	WFE.nAvgBytesPerSec = 176400;
	WFE.nBlockAlign     = 4;
	WFE.wBitsPerSample  = 16;
	WFE.cbSize          = 0;
	waveOutOpen(&hWO, WAVE_MAPPER, &WFE, 0, 0, CALLBACK_NULL);
	// if (FResetVolume == 0)
	// { // als FResetVolume <> 0 is dan hebben we het volume zelf aangepast!
	// waveOutGetVolume(hWO, @ KeyHandler.SystemVolume);
	// }

	WORD vol = (100 - tbVolume->Position) * 0xffff / 100; // (0xffff*tbVolume->Position) + 0xffff*(0xffff*tbVolume->Position);

	if (hWO != nullptr)
	{
		waveOutSetVolume(hWO, vol);
		waveOutClose(hWO);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TfrmMain::PopupMenuPopup(TObject *Sender)
{
	SaveSettings();

	mnuSound->Checked  = d_config.Sound;
	mnuSounds->Checked = d_config.Sounds;
	mnuTTS->Checked    = d_config.TTS;
	mnuPaused->Checked = d_config.Paused;
}
// ---------------------------------------------------------------------------
