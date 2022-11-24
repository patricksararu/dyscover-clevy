#ifndef __VEAPI_H__
#define __VEAPI_H__
/* -------------------------------------------------------------------------+
 |                      Nuance Communications, Inc (TM)                     |
 + -------------------------------------------------------------------------*/

/* --------------------------- COPYRIGHT INFORMATION -----------------------+
 | This program contains proprietary information that is a trade secret of  |
 | Nuance Communications, Inc. and also is protected as an unpublished work |
 | under applicable Copyright laws. Recipient is to retain this program in  |
 | confidence and is not permitted to use or make copies thereof other than |
 | as permitted in a prior written agreement with Nuance Communications,    |
 | Inc. or its affiliates.                                                  |
 | Copyright (c) 2003-2008 Nuance Communications, Inc.                      |
 | All Rights Reserved. Company confidential.                               |
 + -------------------------------------------------------------------------*/

/* -------------------------------------------------------------------------+
 |   HEADER (INCLUDE) SECTION                                               |
 + -------------------------------------------------------------------------*/
#include <stdio.h>
/* API header files */

#if defined( __cplusplus )
extern "C"
{
#endif


/* -------------------------------------------------------------------------+
 |   MACROS                                                                 |
 + -------------------------------------------------------------------------*/

#ifndef VEAPI_EXPORT
#define VEAPI_EXPORT
#endif

#ifndef VEAPI_EXTERN
#define VEAPI_EXTERN
#endif

/* Maximum length for a phoneme string */
#define     VE_MAX_PHONEMELEN 8

/* Maximum length for engine/language/language-version string */
#define VE_MAX_STRING_LENGTH   128
#define VE_MAX_VERSIONSTRING_LENGTH   1024

/* -------------------------------------------------------------------------+
 |   TYPE DEFINITIONS                                                       |
 + -------------------------------------------------------------------------*/

/* @type VE_ integer type definitions */
typedef unsigned char      NUAN_U8;
typedef signed   char      NUAN_S8;
typedef unsigned short     NUAN_U16;
typedef signed   short     NUAN_S16;
typedef unsigned int       NUAN_U32;
typedef signed   int       NUAN_S32;
typedef          float     NUAN_F32;
typedef double             NUAN_D64;

/* @type NUAN_ERROR | 32-bit error number that is returned to the
** client by an API function, NUAN_OK is success. */
typedef NUAN_U32        NUAN_ERROR;

/* Successful error code for a NUAN_ERROR */
#ifndef NUAN_OK
#define     NUAN_OK                (NUAN_ERROR)0x00000000 /* @def "Success." */
#endif

/* Check a NUAN_ERROR for failure */
#define     NUAN_FAIL( Err )  (((NUAN_ERROR)(Err) != NUAN_OK ) ? (1) : (0))

/* @enum NUAN_BOOL | Boolean expression. */
typedef enum _NUAN_BOOL
{
  NUAN_FALSE  = 0, /* @emem false (=0) */
  NUAN_TRUE   = 1  /* @emem true  (=!0)*/
} NUAN_BOOL;

/* @type VE_VERSION | 16-bit version ID where the first 8 bits
** are the major version number and the second 8 bits the minor
** version number. */
typedef NUAN_U16 VE_VERSION;

typedef struct {
  NUAN_U8 minor;
  NUAN_U8 major;
} VE_VERSION_AS_ELEMENTS;

/* Current product version number for fmtVersion fields. */
#define VE_CURRENT_VERSION ((NUAN_U16) 0x0520)

/* @struct VE_HSAFE | VE handle, used to prevent a module from
** crashing when invalid handles are used.  The handle is not a
** pointer but a struct. This makes it possible to check the handle
** for validity without having to de-reference it. */
typedef struct _VE_HSAFE
{
  void      * pHandleData;  /* @field Pointer to the actual handle
                            ** data (real pointer handle). */
  NUAN_U32    u32Check;     /* @field Container for a magic number.
                            ** Enables validity checks on this handle. */
} VE_HSAFE;

/* @type VE_HSPEECH | Handle to a TTS class.
 * @xref <t VE_HSAFE>, <f ve_ttsInitialize>() */
typedef VE_HSAFE  VE_HSPEECH;

/* @type VE_HINSTANCE | Handle to a TTS instance.
 * @xref <t VE_HSAFE>, <f ve_ttsOpen>() */
typedef VE_HSAFE  VE_HINSTANCE;

/* @type VE_HRESOURCE | Handle to a TTS resource.
 * @xref <t VE_HSAFE>, <f ve_ttsResourceLoad>() */
typedef VE_HSAFE  VE_HRESOURCE;

/* @struct VE_[...]_INTERFACE | Forward declaration of the platform
** dependent function interfaces declared in ve_platform.h */
struct VE_HEAP_INTERFACE_S;
typedef struct VE_HEAP_INTERFACE_S VE_HEAP_INTERFACE;

struct VE_CRITSEC_INTERFACE_S;
typedef struct VE_CRITSEC_INTERFACE_S VE_CRITSEC_INTERFACE;

struct VE_DATA_STREAM_INTERFACE_S;
typedef struct VE_DATA_STREAM_INTERFACE_S VE_DATA_STREAM_INTERFACE;

struct VE_DATA_MAPPING_INTERFACE_S;
typedef struct VE_DATA_MAPPING_INTERFACE_S VE_DATA_MAPPING_INTERFACE;

struct VE_LOG_INTERFACE_S;
typedef struct VE_LOG_INTERFACE_S VE_LOG_INTERFACE;

/* @struct VE_INSTALL | List of resources available to the engine
** for processing. The length of the list may vary. If the engine ends
** up using, or not using, a resource depends on:
** (1) the TTS process function called, and
** (2) the parameters set prior to calling the process function. */
typedef struct {
  /* Format version of this struct */
  VE_VERSION                             fmtVersion;
  /* Pointer to the broker information, a NULL terminated string */
  const char *                           pBinBrokerInfo;
  /* Heap interface, used for all heap operations */
  const VE_HEAP_INTERFACE              * pIHeap;
  /* Heap for class operations, instances get passed their own heap
  ** during ve_ttsOpen( ) */
  void                                 * hHeap;
  /* Critical section interface, used for all critical sections. May
  ** be NULL to disable the use of critical sections. */
  const VE_CRITSEC_INTERFACE           * pICritSec;
  /* Critical section class handle, used to create critical sections
  ** across all instances. */
  void                                 * hCSClass;
  /* Data interfaces, used for all Vocalizer data file access. pIDataMapping
  ** may be NULL to redirect all data access to the pIDataStream interface. */
  const VE_DATA_STREAM_INTERFACE       * pIDataStream;
  const VE_DATA_MAPPING_INTERFACE      * pIDataMapping;
  /* Data class handle, used to open individual data streams and mappings */
  void                                 * hDataClass;
  /* Log interface, used for all logging, may be NULL to disable logging */
  const VE_LOG_INTERFACE               * pILog;
  /* Log handle for class operations, instances get passed their own
  ** log handle during ve_ttsOpen( ) */
  void                                 * hLog;
} VE_INSTALL;

/* @enum VE_PARAMID | Parameters */
typedef enum {
    /*  Voice configuration  */
  VE_PARAM_LANGUAGE               = 1,
  VE_PARAM_VOICE                  = 2,
  VE_PARAM_VOICE_OPERATING_POINT  = 3,
  VE_PARAM_FREQUENCY              = 4,
  VE_PARAM_EXTRAESCLANG           = 5,
  VE_PARAM_EXTRAESCTN             = 6,
    /*  Input text attributes */
  VE_PARAM_TYPE_OF_CHAR           = 7,
    /*  Speech output controls */
  VE_PARAM_VOLUME                 = 8,
  VE_PARAM_SPEECHRATE             = 9,
  VE_PARAM_PITCH                  = 10,
  VE_PARAM_WAITFACTOR             = 11,
  VE_PARAM_READMODE               = 12,
  VE_PARAM_TEXTMODE               = 13, /* to remove? */
  VE_PARAM_MAX_INPUT_LENGTH       = 14,
  VE_PARAM_LIDSCOPE               = 15,
  VE_PARAM_LIDVOICESWITCH         = 16,
  VE_PARAM_LIDMODE                = 17,
  VE_PARAM_LIDLANGUAGES           = 18,
    /*  Marker output controls */
  VE_PARAM_MARKER_MODE            = 19,
    /*  Operating mode controls */
  VE_PARAM_INITMODE               = 20  /* to remove */
} VE_PARAMID;

/* @union VE_PARAM_VALUE | Parameter value
 * @xref <f ve_ttsSetParamList>(), <f ve_ttsGetParamList>() */
typedef union {
  NUAN_U16  usValue;
  char      szStringValue[VE_MAX_STRING_LENGTH];
} VE_PARAM_VALUE;

/* @struct VE_PARAM | Parameter setting
 * @xref <f ve_ttsSetParamList>(), <f ve_ttsGetParamList>() */
typedef struct {
  VE_PARAMID        eID;
  VE_PARAM_VALUE    uValue;
} VE_PARAM;

/* @enum VE_INITMODE | Init modes */
typedef enum {
  /* Load and open all modules once (modules remain loaded until
  ** ve_ttsClose is called) */
  VE_INITMODE_LOAD_ONCE_OPEN_ALL         = 0xC,
  /* Load and open all modules for each speak request */
  VE_INITMODE_LOAD_OPEN_ALL_EACH_TIME    = 0x3
} VE_INITMODE;

/* @enum VE_TYPE_OF_CHAR | Text character encodings */
typedef enum {
  VE_TYPE_OF_CHAR_UTF16   = 1, /* Unicode UTF-16, the default */
  VE_TYPE_OF_CHAR_UTF8    = 2  /* Unicode UTF-8 */
} VE_TYPE_OF_CHAR;

/* @enum VE_TEXTMODE | Text modes */
typedef enum {
  VE_TEXTMODE_STANDARD    = 1, /* No preprocessor is active */
  VE_TEXTMODE_SMS         = 2  /* SMS preprocessor is active */
} VE_TEXTMODE;

/* @enum VE_READMODE | Read modes */
typedef enum {
  VE_READMODE_SENT     = 1,         /* Sentence-by-sentence (default) */
  VE_READMODE_CHAR     = 2,         /* Character-by-character */
  VE_READMODE_WORD     = 3,         /* Word-by-word */
  VE_READMODE_LINE     = 4          /* Line-by-line */
} VE_READMODE;

/* @enum VE_FREQUENCY | Audio frequencies */
typedef enum {
  VE_FREQ_8KHZ            =  8, /* @emem  8 kHz */
  VE_FREQ_11KHZ           = 11, /* @emem 11 kHz */
  VE_FREQ_16KHZ           = 16, /* @emem 16 kHz */
  VE_FREQ_22KHZ           = 22  /* @emem 22 kHz */
} VE_FREQUENCY ;

/* @enum VE_MARKERMODE | Marker modes */
typedef enum {
  VE_MRK_OFF              =  0, /* @emem run-time marker notifications
                                   ** are disabled */
  VE_MRK_ON               =  1  /* @emem run-time marker notifications
                                   ** are enabled */
} VE_MARKERMODE;

/* @enum VE_TEXTFORMAT | Text formats */
typedef enum {
  VE_NORM_TEXT              = 0,
  VE_SSML_TEXT              = 1 /* Available on demand via dedicated build only */
} VE_TEXTFORMAT;

/* @enum VE_LIDSCOPE | Values for parameter VE_PARAM_LIDSCOPE */
typedef enum {
  VE_LIDSCOPE_NONE         = 0,
  VE_LIDSCOPE_USERDEFINED  = 1,
  VE_LIDSCOPE_MESSAGE      = 2
} VE_LIDSCOPE;

/* @enum VE_LIDVOICESWITCH | Values for parameter VE_PARAM_LIDVOICESWITCH */
typedef enum {
  VE_LIDVOICESWITCH_OFF  = 0,
  VE_LIDVOICESWITCH_ON   = 1
} VE_LIDVOICESWITCH;

/* @enum VE_LIDMODE | Values for parameter VE_LIDMODE */
typedef enum {
  VE_LIDMODE_MEMORY_BIASED  = 0,
  VE_LIDMODE_FORCED_CHOICE  = 1
} VE_LIDMODE;


/* @struct VE_INTEXT | Input text
 * Structure used for passing char based input (text, phonetic) to the
 * process functions. */
typedef struct {
  VE_TEXTFORMAT         eTextFormat;  /* can be 0 (VE_NORM_TEXT)
                                      ** or 3 (VE_SSML_TEXT) */
  size_t                cntTextLength;/* szInText length in bytes */
  void                * szInText;     /* Input string. No need to zero
                                      ** terminated */
} VE_INTEXT;

/* @enum VE_MSG | Messages */
typedef enum {
  VE_MSG_BEGINPROCESS   = 0x00000001,
  VE_MSG_ENDPROCESS     = 0x00000002,
  VE_MSG_PROCESS        = 0x00000004,
  VE_MSG_OUTBUFREQ      = 0x00000008,
  VE_MSG_OUTBUFDONE     = 0x00000010,
  VE_MSG_STOP           = 0x00000020,
  VE_MSG_PAUSE          = 0x00000040,
  VE_MSG_RESUME         = 0x00000080,
  VE_MSG_TAIBEGIN       = 0x00000100,
  VE_MSG_TAIEND         = 0x00000200,
  VE_MSG_TAIBUFREQ      = 0x00000400,
  VE_MSG_TAIBUFDONE     = 0x00000800
} VE_MSG;

/* @enum VE_PCMSTAT | Indicates the status of the text unit in a
** VE_CALLBACKMSG */
typedef enum {
  VE_PCMSTAT_TXTUNIT_NEW     = 1,
  VE_PCMSTAT_TXTUNIT_MID     = 2,
  VE_PCMSTAT_DONE            = 0xFFFF
} VE_PCMSTAT;

/* @enum VE_AUDIOFORMAT | Audio formats */
typedef enum {
  VE_16LINEAR    = 0,  /* @emem 16 bit (platform native
                          ** byte order) linear PCM */
  VE_MU_LAW,           /* @emem mu-law */
  VE_A_LAW             /* @emem A-law */
} VE_AUDIOFORMAT;

/* @enum VE_MARKTYPE | Marker types */
typedef enum {
  VE_MRK_TEXTUNIT           = 0x0001,
  VE_MRK_WORD               = 0x0002,
  VE_MRK_PHONEME            = 0x0004,
  VE_MRK_BOOKMARK           = 0x0008,
  VE_MRK_PROMPT             = 0x0010
} VE_MARKTYPE;

/* @struct VE_MARKINFO | Marker information
 * @xref <t VE_OUTDATA> */
typedef struct {
  NUAN_U32          ulMrkInfo;
  VE_MARKTYPE       eMrkType;
  size_t            cntSrcPos;
  size_t            cntSrcTextLen;
  size_t            cntDestPos;
  size_t            cntDestLen;
  NUAN_U16          usPhoneme;
  NUAN_U32          ulMrkId;
  NUAN_U32          ulParam;
  char            * szPromptID;
} VE_MARKINFO;

/* @enum VE_TAITYPE | TA Info types */
typedef enum {
  VE_TAI_TEXTUNIT           = 0x0001,
  VE_TAI_BOOKMARK           = 0x0002
} VE_TAITYPE;



/* @struct VE_LIPSYNC | Lip synchronisation information
 * @xref <f ve_ttsGetLipSyncInfo>() */
typedef struct {
  NUAN_S16  sJawOpen;
  NUAN_S16  sTeethUpVisible;
  NUAN_S16  sTeethLoVisible;
  NUAN_S16  sMouthHeight;
  NUAN_S16  sMouthWidth;
  NUAN_S16  sMouthUpturn;
  NUAN_S16  sTonguePos;
  NUAN_S16  sLipTension;
  char      szLHPhoneme[VE_MAX_PHONEMELEN];
} VE_LIPSYNC;


/* @struct VE_OUTDATA | Output data */
typedef struct {
  VE_AUDIOFORMAT        eAudioFormat;
  size_t                cntPcmBufLen;  /* Size of samples buffer in bytes */
  void                * pOutPcmBuf;
  size_t                cntMrkListLen;
  VE_MARKINFO         * pMrkList;
} VE_OUTDATA;

/* @struct VE_OUTTAINFO | Output TA info data */
typedef struct {
  size_t                cntRewrittenTextLen;     /* Size of rewritten text in bytes */
  char                * pRewrittenTextBuf;
  size_t                cntTaInfoListLen;
  void                * pTaInfoList;
} VE_OUTTAINFO;

/* @struct VE_CALLBACKMSG | Callback message
 * @xref <f VE_CBOUTNOTIFY>() */
typedef struct {
  VE_MSG            eMessage;
  NUAN_S32          lValue;
  void            * pParam;
} VE_CALLBACKMSG;


/* @func VE_CBOUTNOTIFY | Output notification callback
 * @xref <t VE_OUTDEVINFO> */
typedef NUAN_ERROR VE_CBOUTNOTIFY (
  VE_HINSTANCE             hTtsInst,
  void                   * pUserData,
  VE_CALLBACKMSG         * pcbMessage
);

/* @struct VE_OUTDEVINFO | Output device
 * @xref <f VE_CBOUTNOTIFY>() */
typedef struct {
  void                   * pUserData;
  VE_CBOUTNOTIFY         * pfOutNotify;
} VE_OUTDEVINFO;

/* @struct VE_LANGUAGE | Basic information on a language.
 * @xref <f ve_ttsGetLanguageList>() */
typedef struct _VE_LANGUAGE
{
  char      szLanguage[VE_MAX_STRING_LENGTH];
  char      szLanguageTLW[4];
  char      szVersion[VE_MAX_STRING_LENGTH];
} VE_LANGUAGE;

typedef struct _VE_NTSINFO
{
  char      szVersion[VE_MAX_STRING_LENGTH];
} VE_NTSINFO;

typedef struct _VE_CLMINFO
{
  char      szFileVersion[VE_MAX_STRING_LENGTH];
  char      szSrcVersion[VE_MAX_VERSIONSTRING_LENGTH];
  char      szDstVersion[VE_MAX_VERSIONSTRING_LENGTH];
} VE_CLMINFO;


/* @struct VE_VOICEINFO | Voice information.
 * @xref <f ve_ttsGetVoiceList>() */
typedef struct {
  char      szVersion[VE_MAX_STRING_LENGTH];
  char      szLanguage[VE_MAX_STRING_LENGTH];
  char      szVoiceName[VE_MAX_STRING_LENGTH];
  char      szVoiceAge[VE_MAX_STRING_LENGTH];
  char      szVoiceType[VE_MAX_STRING_LENGTH];
} VE_VOICEINFO;

/* @struct VE_SPEECHDBINFO | Speech DB information
 * @xref <f ve_ttsGetSpeechDBList>() */
typedef struct {
  char      szVersion[VE_MAX_STRING_LENGTH];
  char      szLanguage[VE_MAX_STRING_LENGTH];
  char      szVoiceName[VE_MAX_STRING_LENGTH];
  char      szVoiceOperatingPoint[VE_MAX_STRING_LENGTH];
  NUAN_U16  u16Freq;
} VE_SPEECHDBINFO;

/* @struct VE_PRODUCT_VERSION | Product version information
 * @xref <f ve_ttsGetProductVersion>() */
typedef struct {
  NUAN_U8 major;
  NUAN_U8 minor;
  NUAN_U8 maint;
} VE_PRODUCT_VERSION;

/* @struct VE_ADDITIONAL_PRODUCTINFO | Additional product information
 * @xref <f ve_ttsGetAdditionalProductInfo>() */
typedef struct {
  NUAN_U16  buildYear;
  NUAN_U8   buildMonth;
  NUAN_U8   buildDay;
  char      buildInfoStr[256];
} VE_ADDITIONAL_PRODUCTINFO;

typedef struct
{
  VE_TAITYPE         jmpPointType;
  size_t             positionInText;
  void              *stateInfo;
  char               languageIdent[16];
} VE_TA_NODE;


/*
 * function prototype typedefs
*/

typedef NUAN_ERROR VE_TTSINITIALIZE_F (
  const VE_INSTALL   * pResources,
        VE_HSPEECH   * phTtsCl
);


typedef  NUAN_ERROR  VE_TTSUNINITIALIZE_F (
  const VE_HSPEECH   hTtsCl
);

typedef  NUAN_ERROR  VE_TTSOPEN_F (
        VE_HSPEECH   hTtsCl,
        void       * hHeap,
        void       * hLog,
        VE_HINSTANCE       * phTtsInst
);

typedef  NUAN_ERROR  VE_TTSCLOSE_F (
  VE_HINSTANCE   hTtsInst
);

typedef  NUAN_ERROR  VE_TTSSETOUTDEVICE_F (
  VE_HINSTANCE       hTtsInst,
  VE_OUTDEVINFO    * pOutDevInfo
);

typedef  NUAN_ERROR  VE_TTSPROCESSTEXT2SPEECH_F (
        VE_HINSTANCE     hTtsInst,
  const VE_INTEXT      * pInText
);

typedef  NUAN_ERROR  VE_TTSPROCESSTEXT2SPEECHCOOPERATIVELY_F (
        VE_HINSTANCE     hTtsInst,
  const VE_INTEXT      * pInText,
        VE_MSG         * pMsg,
        VE_OUTDATA     * pOutData
);

typedef NUAN_ERROR VE_TTSANALYZETEXT_F (
        VE_HINSTANCE     hTtsInst,
  const VE_INTEXT      * pInText
);

typedef NUAN_ERROR VE_TTSPROCESSTEXT2SPEECHSTARTINGAT_F (
        VE_HINSTANCE        hTtsInst,
        VE_INTEXT         * pInText,
  const VE_TA_NODE        * pTaInfo,
  const size_t              cntTaIndex
);

typedef  NUAN_ERROR  VE_TTSSTOP_F (
  VE_HINSTANCE   hTtsInst
);

typedef  NUAN_ERROR  VE_TTSPAUSE_F (
  VE_HINSTANCE   hTtsInst
);

typedef  NUAN_ERROR  VE_TTSRESUME_F (
  VE_HINSTANCE   hTtsInst
);

typedef  NUAN_ERROR  VE_TTSSETPARAMLIST_F (
  VE_HINSTANCE        hTtsInst,
  VE_PARAM          * pTtsParam,
  NUAN_U16            usNbrOfParam
);

typedef  NUAN_ERROR  VE_TTSGETPARAMLIST_F (
  VE_HINSTANCE        hTtsInst,
  VE_PARAM          * pTtsParam,
  NUAN_U16            usNbrOfParam
);

typedef  NUAN_ERROR  VE_TTSRESOURCELOAD_F (
        VE_HINSTANCE        hTtsInst,
  const char              * szMimeContentType,
        size_t              cntInDataLength,
  const void              * pInData,
        VE_HRESOURCE      * phResource
);

typedef  NUAN_ERROR  VE_TTSRESOURCEUNLOAD_F (
  VE_HINSTANCE       hTtsInst,
  VE_HRESOURCE       hResource
);

typedef  NUAN_ERROR  VE_TTSGETLANGUAGELIST_F (
  const VE_HSPEECH          hTtsCl,
        VE_LANGUAGE       * pLanguages,
        NUAN_U16          * pusNbrOfElements
);

typedef  NUAN_ERROR  VE_TTSGETVOICELIST_F (
  const VE_HSPEECH          hTtsCl,
  const char              * szLanguage,
        VE_VOICEINFO      * pVoiceList,
        NUAN_U16          * pusNbrOfElements
);

typedef  NUAN_ERROR  VE_TTSGETNTSINFO_F (
  const VE_HSPEECH          hTtsCl,
  const char              * szLanguage,
        VE_NTSINFO        * pNtsInfo
);

typedef  NUAN_ERROR  VE_TTSGETSPEECHDBLIST_F (
  const VE_HSPEECH            hTtsCl,
  const char                * szLanguage,
  const char                * szVoice,
        VE_SPEECHDBINFO     * pSpeechDBList,
        NUAN_U16            * pusNbrOfElements
);

typedef  NUAN_ERROR  VE_TTSGETLIPSYNCINFO_F (
  VE_HINSTANCE        hTtsInst,
  NUAN_U16            usPhoneme,
  VE_LIPSYNC        * pTtsLipSync
);

typedef  NUAN_ERROR  VE_TTSGETPRODUCTVERSION_F (
  VE_PRODUCT_VERSION  *pTtsProductVersion
);

/* -------------------------------------------------------------------------+
 |   GLOBAL FUNCTION PROTOTYPES                                             |
 + -------------------------------------------------------------------------*/

VEAPI_EXTERN NUAN_ERROR VEAPI_EXPORT ve_ttsInitialize (
  const VE_INSTALL   * pResources,
        VE_HSPEECH   * phTtsCl
);

VEAPI_EXTERN NUAN_ERROR VEAPI_EXPORT ve_ttsUnInitialize (
  const VE_HSPEECH   hTtsCl
);

VEAPI_EXTERN NUAN_ERROR VEAPI_EXPORT ve_ttsOpen (
        VE_HSPEECH     hTtsCl,
        void         * hHeap,
        void         * hLog,
        VE_HINSTANCE * phTtsInst
);

VEAPI_EXTERN NUAN_ERROR VEAPI_EXPORT ve_ttsClose (
  VE_HINSTANCE   hTtsInst
);

VEAPI_EXTERN NUAN_ERROR VEAPI_EXPORT ve_ttsSetOutDevice (
  VE_HINSTANCE       hTtsInst,
  VE_OUTDEVINFO    * pOutDevInfo
);

VEAPI_EXTERN NUAN_ERROR VEAPI_EXPORT ve_ttsProcessText2Speech (
        VE_HINSTANCE     hTtsInst,
  const VE_INTEXT      * pInText
);

VEAPI_EXTERN NUAN_ERROR VEAPI_EXPORT ve_ttsProcessText2SpeechCooperatively (
        VE_HINSTANCE     hTtsInst,
  const VE_INTEXT      * pInText,
        VE_MSG         * pMsg,
        VE_OUTDATA     * pOutData
);

VEAPI_EXTERN NUAN_ERROR VEAPI_EXPORT ve_ttsAnalyzeText (
        VE_HINSTANCE     hTtsInst,
  const VE_INTEXT      * pInText
);

VEAPI_EXTERN NUAN_ERROR VEAPI_EXPORT ve_ttsProcessText2SpeechStartingAt (
        VE_HINSTANCE        hTtsInst,
        VE_INTEXT         * pInText,
  const VE_TA_NODE        * pTaInfo,
  const size_t              cntTaIndex
);

VEAPI_EXTERN NUAN_ERROR VEAPI_EXPORT ve_ttsStop (
  VE_HINSTANCE   hTtsInst
);

VEAPI_EXTERN NUAN_ERROR VEAPI_EXPORT ve_ttsPause (
  VE_HINSTANCE   hTtsInst
);

VEAPI_EXTERN NUAN_ERROR VEAPI_EXPORT ve_ttsResume (
  VE_HINSTANCE   hTtsInst
);

VEAPI_EXTERN NUAN_ERROR VEAPI_EXPORT ve_ttsSetParamList (
  VE_HINSTANCE        hTtsInst,
  VE_PARAM          * pTtsParam,
  NUAN_U16            usNbrOfParam
);

VEAPI_EXTERN NUAN_ERROR VEAPI_EXPORT ve_ttsGetParamList (
  VE_HINSTANCE        hTtsInst,
  VE_PARAM          * pTtsParam,
  NUAN_U16            usNbrOfParam
);

VEAPI_EXTERN NUAN_ERROR VEAPI_EXPORT ve_ttsResourceLoad (
        VE_HINSTANCE        hTtsInst,
  const char              * szMimeContentType,
        size_t              cntInDataLength,
  const void              * pInData,
        VE_HRESOURCE      * phResource
);

VEAPI_EXTERN NUAN_ERROR VEAPI_EXPORT ve_ttsResourceUnload (
  VE_HINSTANCE       hTtsInst,
  VE_HRESOURCE       hResource
);

VEAPI_EXTERN NUAN_ERROR VEAPI_EXPORT ve_ttsGetLanguageList (
  const VE_HSPEECH          hTtsCl,
        VE_LANGUAGE       * pLanguages,
        NUAN_U16          * pusNbrOfElements
);

VEAPI_EXTERN NUAN_ERROR VEAPI_EXPORT ve_ttsGetVoiceList (
  const VE_HSPEECH          hTtsCl,
  const char              * szLanguage,
        VE_VOICEINFO      * pVoiceList,
        NUAN_U16          * pusNbrOfElements
);

VEAPI_EXTERN NUAN_ERROR VEAPI_EXPORT ve_ttsGetSpeechDBList (
  const VE_HSPEECH            hTtsCl,
  const char                * szLanguage,
  const char                * szVoice,
        VE_SPEECHDBINFO     * pSpeechDBList,
        NUAN_U16            * pusNbrOfElements
);

VEAPI_EXTERN NUAN_ERROR VEAPI_EXPORT ve_ttsGetNtsInfo (
  const VE_HSPEECH       hSpeech,
  const char              * szLanguage,
        VE_NTSINFO        * pNtsInfo
);
VEAPI_EXTERN NUAN_ERROR VEAPI_EXPORT ve_ttsGetClmInfo (
  const VE_HSPEECH       hSpeech,
  const char              * szLanguage,
        VE_CLMINFO        * pClmInfo
);


VEAPI_EXTERN NUAN_ERROR VEAPI_EXPORT ve_ttsGetLipSyncInfo (
  VE_HINSTANCE        hTtsInst,
  NUAN_U16            usPhoneme,
  VE_LIPSYNC        * pTtsLipSync
);

VEAPI_EXTERN NUAN_ERROR VEAPI_EXPORT ve_ttsGetProductVersion (
  VE_PRODUCT_VERSION  * pTtsProductVersion
);

VEAPI_EXTERN NUAN_ERROR VEAPI_EXPORT ve_ttsGetAdditionalProductInfo (
  VE_ADDITIONAL_PRODUCTINFO  * pTtsAdditionalProductInfo
);


/*---------------------------------------------------------------------
* @deflist GENERAL ERROR CODES
*--------------------------------------------------------------------*/
/*GENERAL ERROR CODES*/
/*General Error Variable*/
#define NUAN_E_NOK                          (NUAN_ERROR)0x80000000   /*@def Failure.*/
#define NUAN_E_NOTIMPLEMENTED               (NUAN_ERROR)0x80000001   /*@def This feature is not supported.*/
#define NUAN_E_NOTINITIALIZED               (NUAN_ERROR)0x80000002   /*@def API has not been appropriately initialized.*/
#define NUAN_E_COULDNOTOPENFILE             (NUAN_ERROR)0x80000003   /*@def Could not open the file.*/
#define NUAN_E_FILEREADERROR                (NUAN_ERROR)0x80000004   /*@def Error while reading file.*/
#define NUAN_E_FILEWRITEERROR               (NUAN_ERROR)0x80000005   /*@def Error while writing file.*/
#define NUAN_E_NULLPOINTER                  (NUAN_ERROR)0x80000006   /*@def Nullpointer as argument.*/
#define NUAN_E_INVALIDARG                   (NUAN_ERROR)0x80000007   /*@def Argument is not valid.*/
#define NUAN_E_INVALIDHANDLE                (NUAN_ERROR)0x80000008   /*@def Handle is not valid.*/
#define NUAN_E_BUFFERTOOSMALL               (NUAN_ERROR)0x80000009   /*@def Buffer is too small.*/
#define NUAN_E_OUTOFMEMORY                  (NUAN_ERROR)0x8000000A   /*@def Not enough memory.*/
#define NUAN_E_ENGINENOTFOUND               (NUAN_ERROR)0x8000000B   /*@def Specified engine could not be found.*/
#define NUAN_E_LANGUAGENOTFOUND             (NUAN_ERROR)0x8000000C   /*@def Specified language could not be found.*/
#define NUAN_E_MODULENOTFOUND               (NUAN_ERROR)0x8000000D   /*@def Unable to locate a module.*/
#define NUAN_E_CONVERSIONFAILED             (NUAN_ERROR)0x8000000E   /*@def A string conversion (to or from UNICODE) has failed.*/
#define NUAN_E_OUTOFRANGE                   (NUAN_ERROR)0x8000000F   /*@def The specified value is out of range.*/
#define NUAN_E_INVALIDPOINTER               (NUAN_ERROR)0x80000010   /*@def An invalid pointer has been passed to the function.*/
#define NUAN_E_WRONG_STATE                  (NUAN_ERROR)0x80000011   /*@def Inappropriate command for current state.*/
#define NUAN_E_INVALIDPARAM                 (NUAN_ERROR)0x80000012   /*@def The parameter value was invalid.*/
#define NUAN_E_ALREADYINITIALIZED           (NUAN_ERROR)0x80000013   /*@def The API is already initialized.*/
#define NUAN_E_NOTFOUND                     (NUAN_ERROR)0x80000014   /*@def The object was not found.*/
#define NUAN_E_FEATEXTRACT                  (NUAN_ERROR)0x80000015   /*@def The feature extraction failed.*/
#define NUAN_E_MAXCHANNELS                  (NUAN_ERROR)0x80000016   /*@def Maximum allowed number of instances are already in use.*/
#define NUAN_E_ENDOFINPUT                   (NUAN_ERROR)0x80000017   /*@def No more input to process.*/
#define NUAN_E_ALREADYDEFINED               (NUAN_ERROR)0x80000018   /*@def Object already defined.*/
#define NUAN_E_NOTCOMPATIBLE                (NUAN_ERROR)0x80000019   /*@def Incompatibility between objects.*/
#define NUAN_E_FILENOTLOADED                (NUAN_ERROR)0x8000001A   /*@def Trying to use a file that was not loaded.*/
#define NUAN_E_FOLDERREADERROR              (NUAN_ERROR)0x8000001B   /*@def Error while reading folder.*/

#define NUAN_E_NON_DOCUMENTED_ERROR         (NUAN_ERROR)0x80000100   /*@def Non documented error*/
#define NUAN_E_BUSY                         (NUAN_ERROR)0x80000101   /*@def Object  busy.*/
#define NUAN_E_READONLY                     (NUAN_ERROR)0x80000102   /*@def Object  is  read-only->probably  in  use.*/
#define NUAN_E_FILECLOSE                    (NUAN_ERROR)0x80000103   /*@def Error  closing  file.*/
#define NUAN_E_FILESEEK                     (NUAN_ERROR)0x80000104   /*@def Error  seeking  in  file.*/
#define NUAN_E_INVALIDCHAR                  (NUAN_ERROR)0x80000105   /*@def An invalid UTF-8 character was used.*/
#define NUAN_E_WRONG_BUFFER_SIZE            (NUAN_ERROR)0x80000106   /*@def Specified buffer size is not correct.*/
#define NUAN_E_EMPTY_LHSTRING               (NUAN_ERROR)0x80000107   /*@def Received an empty L&H+ string.*/
#define NUAN_E_MALLOC                       (NUAN_ERROR)0x80000108   /*@def Memory allocation failed.*/
#define NUAN_E_DATA_IN_USE                  (NUAN_ERROR)0x80000109   /*@def A Data usage or close was attempted on a dataimport that is still in use on some engine.*/
#define NUAN_E_INVALID_DATA                 (NUAN_ERROR)0x8000010A   /*@def Handle to the imported data is not valid - probably because it has been closed.*/
#define NUAN_E_INVALID_DATATYPE             (NUAN_ERROR)0x8000010B   /*@def A buffer with an invalid data type is supplied.*/
#define NUAN_E_NULL_HANDLE                  (NUAN_ERROR)0x8000010C   /*@def A NULL handle was passed to the function.*/
#define NUAN_E_NULL_POINTER                 (NUAN_ERROR)0x8000010D   /*@def A NULL pointer has been passed to an spi command.*/
#define NUAN_E_SYSTEM_ERROR                 (NUAN_ERROR)0x8000010E   /*@def An error has occurred in the system while an Operating System call was made.*/
#define NUAN_E_MAPPING                      (NUAN_ERROR)0x8000010F   /*@def Error while mapping a read-only window on specified section of a data object.*/
#define NUAN_E_HANDLEWASOPEN                (NUAN_ERROR)0x80000110   /*@def Already open handle presented to an open function.*/
#define NUAN_E_INVALID_FLAG_COMBINATION     (NUAN_ERROR)0x80000111   /*@def An invalid flag combination was used as one of the paramaters.*/
#define NUAN_E_UNRELEASEDMODULES            (NUAN_ERROR)0x80000112   /*@def Some modules (data or interfaces) were not released yet.*/
#define NUAN_E_INTFNOTFOUND                 (NUAN_ERROR)0x80000113   /*@def Interface could not be found.*/

#define NUAN_E_OUTOFRESOURCE                (NUAN_ERROR)0x80000200   /*@def Out of resources  */

#define NUAN_E_VERSION                      (NUAN_ERROR)0x80000300   /*@def Struct has unsupported version no.*/


/*---------------------------------------------------------------------
* @deflist DICTIONARY ERROR
*--------------------------------------------------------------------*/
/*DICTIONARY ERROR CODES*/
/*General Error Variable*/
#define NUAN_E_DICT_WRONGTXTDCTFORMAT       (NUAN_ERROR)0x80000400   /*@def Illegal text dictionary format.*/
#define NUAN_E_DICT_CORRUPTBUFFER           (NUAN_ERROR)0x8000040E   /*@def The provided buffer is corrupt.*/
#define NUAN_E_DICT_UNKNOWNSTREAMFORMAT     (NUAN_ERROR)0x80000416   /*@def Unknown format specified */

/*---------------------------------------------------------------------
* @deflist GENERAL WARNING CODES
*--------------------------------------------------------------------*/
/*GENERAL WARNING CODES*/
/*General WARNING Variable*/
#define NUAN_W_FALSE                        0x00000001   /*@def Warning: False success.*/
#define NUAN_W_ALREADYPRESENT               0x00000002   /*@def Warning: Object already present.*/
#define NUAN_W_ENDOFINPUT                   0x00000003   /*@def Warning: No more input to process.*/
#define NUAN_W_CHARSKIPPED                  0x00000004   /*@def Warning: Characters skipped during a conversion.*/
#define NUAN_W_NOINPUTTEXT                  0x00000005   /*@def Warning: No input text.*/
#define NUAN_W_NON_DOCUMENTED_WARNING       0x00001FFF   /*@def Warning: Non Documented Warning*/
#define NUAN_W_EOF                          0x00000006   /*@def Warning: End of File reached.*/


/*---------------------------------------------------------------------
* @deflist TTS ERROR CODES
*--------------------------------------------------------------------*/
/*TTS ERROR CODES*/
/*General TTS Variable*/
#define NUAN_E_TTS_NOINPUTTEXT              (NUAN_ERROR)0x80000800   /*@def No input text has been found.*/
#define NUAN_E_TTS_INSTBUSY                 (NUAN_ERROR)0x80000801   /*@def The specified instance is processing an input text.*/
#define NUAN_E_TTS_AUDIOOUTOPEN             (NUAN_ERROR)0x80000802   /*@def Could not open the audio output device.*/
#define NUAN_E_TTS_NOLANGUAGE               (NUAN_ERROR)0x80000803   /*@def No language has been selected yet.*/
#define NUAN_E_TTS_AUDIOOUTWRITE            (NUAN_ERROR)0x80000804   /*@def Could not write to the audio output device.*/
#define NUAN_E_TTS_VOICENOTFOUND            (NUAN_ERROR)0x80000805   /*@def Specified voice could not be found.*/
#define NUAN_E_TTS_PPNOTFOUND               (NUAN_ERROR)0x80000806   /*@def Specified preprocessor could not be found.*/
#define NUAN_E_TTS_USERSTOP                 (NUAN_ERROR)0x80000807   /*@def Text processing stopped at user request.*/
#define NUAN_E_TTS_ILLFORMEDINPUTDOC        (NUAN_ERROR)0x80000808   /*@def The input document is not well formed (contains errors at the syntactic level).*/
#define NUAN_E_TTS_INVALIDINPUTDOC          (NUAN_ERROR)0x80000809   /*@def The input document is not valid according to the document type definition (DTD) or schema.*/
#define NUAN_E_TTS_DPSLINK                  (NUAN_ERROR)0x8000080A   /*@def Internal link error.*/
#define NUAN_E_TTS_NOMORETEXT               (NUAN_ERROR)0x8000080B   /*@def No more text to send to the text-to-speech system.*/
#define NUAN_E_TTS_DPSOVERFLOW              (NUAN_ERROR)0x8000080C   /*@def Internal overflow error, input text contains garbage or is too long.*/

/* "Vocalizer for Automotive" specific error codes */
#define NUAN_E_TTS_MISSING_OUTDEVICE        (NUAN_ERROR)0x80000880   /*@def Missing output device. I.e. no callback function. */

#if defined( __cplusplus )
}
#endif

/* -------------------------------------------------------------------------+
 |   END                                                                    |
 + -------------------------------------------------------------------------*/

#endif /* #ifndef __VEAPI_H__ */

