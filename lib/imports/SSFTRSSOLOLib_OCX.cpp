// ************************************************************************ //
// WARNING                                                                    
// -------                                                                    
// The types declared in this file were generated from data read from a       
// Type Library. If this type library is explicitly or indirectly (via        
// another type library referring to this type library) re-imported, or the   
// 'Refresh' command of the Type Library Editor activated while editing the   
// Type Library, the contents of this file will be regenerated and all        
// manual modifications will be lost.                                         
// ************************************************************************ //

// $Rev: 52393 $
// File generated on 2-6-2016 16:17:31 from Type Library described below.

// ************************************************************************  //
// Type Lib: C:\Program Files (x86)\Clevy Dyscover\speech\components\common\ssftrssolo.ocx (1)
// LIBID: {6FEF776A-4CC5-11D4-9521-0000F8092E73}
// LCID: 0
// Helpfile: 
// HelpString: ScanSoft RealSpeak Solo ActiveX Control module
// DepndLst: 
//   (1) v2.0 stdole, (C:\Windows\SysWOW64\stdole2.tlb)
// SYS_KIND: SYS_WIN32
// ************************************************************************ //

#include <vcl.h>
#pragma hdrstop

#include <olectrls.hpp>
#include <oleserver.hpp>
#if defined(USING_ATL)
#include <atl\atlvcl.h>
#endif

#pragma option -w-8122
#include "SSFTRSSOLOLib_OCX.h"

#if !defined(__PRAGMA_PACKAGE_SMART_INIT)
#define      __PRAGMA_PACKAGE_SMART_INIT
#pragma package(smart_init)
#endif

namespace Ssftrssololib_tlb
{



// *********************************************************************//
// OCX PROXY CLASS IMPLEMENTATION
// (The following variables/methods implement the class TSsftRSSoloA which
// allows "ScanSoft RealSpeak Solo ActiveX Control" to be hosted in CBuilder IDE/apps).
// *********************************************************************//
int   TSsftRSSoloA::EventDispIDs[13] = {
    0x00000001, 0x00000002, 0x00000003, 0x00000004, 0x00000005, 0x00000006,
    0x00000007, 0x00000008, 0x00000009, 0x0000000A, 0x0000000B, 0x0000000C,
    0x0000000D};

Vcl::Olectrls::TControlData2 TSsftRSSoloA::CControlData =
{
  // GUID of CoClass and Event Interface of Control
  {0x6FEF776D, 0x4CC5, 0x11D4,{ 0x95, 0x21, 0x00,0x00, 0xF8, 0x09,0x2E, 0x73} }, // CoClass
  {0x6FEF776C, 0x4CC5, 0x11D4,{ 0x95, 0x21, 0x00,0x00, 0xF8, 0x09,0x2E, 0x73} }, // Events

  // Count of Events and array of their DISPIDs
  13, &EventDispIDs,

  // Pointer to Runtime License string
  NULL,  // HRESULT(0x80004005)

  // Flags for OnChanged PropertyNotification
  0x00000000,
  401,// (IDE Version)

  // Count of Font Prop and array of their DISPIDs
  0, Winapi::Activex::PDispIDList(NULL),

  // Count of Pict Prop and array of their DISPIDs
  0, Winapi::Activex::PDispIDList(NULL),
  0, // Reserved
  0, // Instance count (used internally)
  0, // List of Enum descriptions (internal)
};

GUID     TSsftRSSoloA::DEF_CTL_INTF = {0x6FEF776B, 0x4CC5, 0x11D4,{ 0x95, 0x21, 0x00,0x00, 0xF8, 0x09,0x2E, 0x73} };
TNoParam TSsftRSSoloA::OptParam;

static inline void ValidCtrCheck(TSsftRSSoloA *)
{
   delete new TSsftRSSoloA((System::Classes::TComponent*)(0));
};

void __fastcall TSsftRSSoloA::InitControlData()
{
  ControlData = (Vcl::Olectrls::TControlData*)&CControlData;
  CControlData.FirstEventOfs = System::PByte(&FOnWordMark) - System::PByte(this);
};

void __fastcall TSsftRSSoloA::CreateControl()
{
  if (!m_OCXIntf)
  {
    _ASSERTE(DefaultDispatch);
    DefaultDispatch->QueryInterface(DEF_CTL_INTF, (LPVOID*)&m_OCXIntf);
  }
};

_DSsftRSSoloDisp __fastcall TSsftRSSoloA::GetDefaultInterface()
{
  CreateControl();
  return m_OCXIntf;
};

void __fastcall TSsftRSSoloA::AboutBox(void)
{
  GetDefaultInterface()->AboutBox();
}

long __fastcall TSsftRSSoloA::AddLicenseKey(BSTR pszLincenseKey)
{
  return  GetDefaultInterface()->AddLicenseKey(pszLincenseKey);
}

long __fastcall TSsftRSSoloA::Backward(Ssftrssololib_tlb::enumJmpMode sJmpMode, long lJmpValue)
{
  return  GetDefaultInterface()->Backward(sJmpMode, lJmpValue);
}

long __fastcall TSsftRSSoloA::Forward(Ssftrssololib_tlb::enumJmpMode sJmpMode, long lJmpValue)
{
  return  GetDefaultInterface()->Forward(sJmpMode, lJmpValue);
}

long __fastcall TSsftRSSoloA::GeneratePcmBuff(BSTR pszInText, 
                                              Ssftrssololib_tlb::enumTextFormat sTextFormat, 
                                              short sPCMFormat)
{
  return  GetDefaultInterface()->GeneratePcmBuff(pszInText, sTextFormat, sPCMFormat);
}

long __fastcall TSsftRSSoloA::GeneratePcmFile(BSTR pszInText, short sTextFormat, BSTR pszOutFile, 
                                              short sPCMFormat, 
                                              Ssftrssololib_tlb::enumOutType sOutType)
{
  return  GetDefaultInterface()->GeneratePcmFile(pszInText, sTextFormat, pszOutFile, sPCMFormat, 
                                                 sOutType);
}

BSTR __fastcall TSsftRSSoloA::GetDictionaryPathName(short sDictionaryIndex)
{
  return  GetDefaultInterface()->GetDictionaryPathName(sDictionaryIndex);
}

BSTR __fastcall TSsftRSSoloA::GetEngineName(long nEngineIndex)
{
  return  GetDefaultInterface()->GetEngineName(nEngineIndex);
}

BSTR __fastcall TSsftRSSoloA::GetEngineVersion(long nEngineIndex)
{
  return  GetDefaultInterface()->GetEngineVersion(nEngineIndex);
}

BSTR __fastcall TSsftRSSoloA::GetLanguageName(long nLanguageIndex)
{
  return  GetDefaultInterface()->GetLanguageName(nLanguageIndex);
}

BSTR __fastcall TSsftRSSoloA::GetLanguageOfText(BSTR pszInText, 
                                                Ssftrssololib_tlb::enumTextFormat sTextFormat)
{
  return  GetDefaultInterface()->GetLanguageOfText(pszInText, sTextFormat);
}

long __fastcall TSsftRSSoloA::GetNextPcm(VARIANT Buffer, long* plBufferLen, 
                                         VARIANT_BOOL* pbLastBuffer, VARIANT bNoTimer/*[opt]*/)
{
  return  GetDefaultInterface()->GetNextPcm(Buffer, plBufferLen, pbLastBuffer, bNoTimer/*[opt]*/);
}

long __fastcall TSsftRSSoloA::GetNextPcmAndMarkers(VARIANT Buffer, long* plBufferLen, VARIANT Mrk, 
                                                   long* plMarkerSize, VARIANT_BOOL* pbLastBuffer, 
                                                   VARIANT bNoTimer/*[opt]*/)
{
  return  GetDefaultInterface()->GetNextPcmAndMarkers(Buffer, plBufferLen, Mrk, plMarkerSize, 
                                                      pbLastBuffer, bNoTimer/*[opt]*/);
}

long __fastcall TSsftRSSoloA::GetPersistentProperties(void)
{
  return  GetDefaultInterface()->GetPersistentProperties();
}

BSTR __fastcall TSsftRSSoloA::GetPPDescription(short sPPIndex)
{
  return  GetDefaultInterface()->GetPPDescription(sPPIndex);
}

BSTR __fastcall TSsftRSSoloA::GetPPManufacturer(short sPPIndex)
{
  return  GetDefaultInterface()->GetPPManufacturer(sPPIndex);
}

BSTR __fastcall TSsftRSSoloA::GetPPName(short sPPIndex)
{
  return  GetDefaultInterface()->GetPPName(sPPIndex);
}

BSTR __fastcall TSsftRSSoloA::GetPPVersion(short sPPIndex)
{
  return  GetDefaultInterface()->GetPPVersion(sPPIndex);
}

BSTR __fastcall TSsftRSSoloA::GetRuleTablePathName(short sRuleTableIndex)
{
  return  GetDefaultInterface()->GetRuleTablePathName(sRuleTableIndex);
}

BSTR __fastcall TSsftRSSoloA::GetVoiceAge(short sVoiceIndex)
{
  return  GetDefaultInterface()->GetVoiceAge(sVoiceIndex);
}

BSTR __fastcall TSsftRSSoloA::GetVoiceName(short sVoiceIndex)
{
  return  GetDefaultInterface()->GetVoiceName(sVoiceIndex);
}

BSTR __fastcall TSsftRSSoloA::GetVoiceVersion(short sVoiceIndex)
{
  return  GetDefaultInterface()->GetVoiceVersion(sVoiceIndex);
}

BSTR __fastcall TSsftRSSoloA::GetVoiceType(short sVoiceIndex)
{
  return  GetDefaultInterface()->GetVoiceType(sVoiceIndex);
}

long __fastcall TSsftRSSoloA::InitSpeech(BSTR szPath)
{
  return  GetDefaultInterface()->InitSpeech(szPath);
}

long __fastcall TSsftRSSoloA::LoadDictionary(BSTR szDictionaryPathName)
{
  return  GetDefaultInterface()->LoadDictionary(szDictionaryPathName);
}

long __fastcall TSsftRSSoloA::LoadRuleTable(BSTR pszFileName)
{
  return  GetDefaultInterface()->LoadRuleTable(pszFileName);
}

long __fastcall TSsftRSSoloA::Pause(void)
{
  return  GetDefaultInterface()->Pause();
}

long __fastcall TSsftRSSoloA::PersistProperties(void)
{
  return  GetDefaultInterface()->PersistProperties();
}

long __fastcall TSsftRSSoloA::Resume(void)
{
  return  GetDefaultInterface()->Resume();
}

long __fastcall TSsftRSSoloA::Speak(BSTR pszInText, Ssftrssololib_tlb::enumTextFormat sTextFormat)
{
  return  GetDefaultInterface()->Speak(pszInText, sTextFormat);
}

long __fastcall TSsftRSSoloA::Stop(void)
{
  return  GetDefaultInterface()->Stop();
}

long __fastcall TSsftRSSoloA::UnloadAllDictionaries(void)
{
  return  GetDefaultInterface()->UnloadAllDictionaries();
}

long __fastcall TSsftRSSoloA::UnloadAllRuleTables(void)
{
  return  GetDefaultInterface()->UnloadAllRuleTables();
}

long __fastcall TSsftRSSoloA::UnloadDictionary(BSTR szDictionaryPathName)
{
  return  GetDefaultInterface()->UnloadDictionary(szDictionaryPathName);
}

long __fastcall TSsftRSSoloA::UnloadRuleTable(BSTR pszFileName)
{
  return  GetDefaultInterface()->UnloadRuleTable(pszFileName);
}


};     // namespace Ssftrssololib_tlb


// *********************************************************************//
// The Register function is invoked by the IDE when this module is 
// installed in a Package. It provides the list of Components (including
// OCXes) implemented by this module. The following implementation
// informs the IDE of the OCX proxy classes implemented here.
// *********************************************************************//
namespace Ssftrssololib_ocx
{

void __fastcall PACKAGE Register()
{
  // [1]
  System::Classes::TComponentClass cls_ocx[] = {
                              __classid(Ssftrssololib_tlb::TSsftRSSoloA)
                           };
  System::Classes::RegisterComponents("ActiveX", cls_ocx,
                     sizeof(cls_ocx)/sizeof(cls_ocx[0])-1);
}

};     // namespace Ssftrssololib_ocx
