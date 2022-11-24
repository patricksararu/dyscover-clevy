/* ******************************************************************
**  Nuance Communications, Inc.
** ******************************************************************/

/* ******************************************************************
**
**  COPYRIGHT INFORMATION
**
**  This program contains proprietary information that is a trade secret
**  of Nuance Communications, Inc. and also is protected as an unpublished
**  work under applicable Copyright laws. Recipient is to retain this
**  program in confidence and is not permitted to use or make copies
**  thereof other than as permitted in a prior written agreement with
**  Nuance Communications, Inc. or its affiliates.
**
**  (c) Copyright 2008 Nuance Communications, Inc.
**  All rights reserved. Company confidential.
**
** ******************************************************************/

/* ******************************************************************
**  HEADER (INCLUDE) SECTION
** ******************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef UNDER_CE
#include <windows.h>
#endif /* UNDER_CE */

#include "vprintf.h"
#include "ve_ttsapi.h"
#include "vplatform.h"
#include "vplatform_tchar.h"
#include "vheap.h"
#include "vfile.h"

/* ******************************************************************
**  DEFINITIONS
** ******************************************************************/

#define PCM_BUF_SIZE   0x1000 /* in bytes */
#define MRK_BUF_SIZE      100 /* in VE_MARKINFO structs */

#define ARGV_CHAR                   PLATFORM_TCHAR
#define ARGV_ATOI(x)                ((NUAN_U16)_ttoi(x))
#define ARGV_COPY(dest,src)         _tcscpy(dest,src)
#define ARGV_COPY_API(dest,src)     NUAN_APP_tcscpy(dest,src)

/* ******************************************************************
**  GLOBALS
** ******************************************************************/

static PLATFORM_TCHAR szTextFile[VE_MAX_STRING_LENGTH] = { 0 };
static PLATFORM_TCHAR szOutFile[VE_MAX_STRING_LENGTH] = { 0 };
static VE_PARAM TtsParam[16];
static NUAN_U16 cTtsParam = 0;
static NUAN_BOOL bExtraInfo = NUAN_FALSE;
static NUAN_BOOL bNoOutput = NUAN_FALSE;
static NUAN_S16 cDirScanning = -1;
static VE_TEXTFORMAT eTextFormat = VE_NORM_TEXT;
static NUAN_S16 * pPcmData = NULL;
static VE_MARKINFO * pMrkData = NULL;

#define MAX_DATA_INSTALL_PATHS 64
static NUAN_U16 g_u16NbrOfDataInstallPaths = 0;
static PLATFORM_TCHAR g_aszDataInstallPaths[MAX_DATA_INSTALL_PATHS][VE_MAX_STRING_LENGTH];
static PLATFORM_TCHAR * g_apDataInstallPaths[MAX_DATA_INSTALL_PATHS];
static PLATFORM_TCHAR g_szBrokerInfoFile[VE_MAX_STRING_LENGTH] = { 0 };

#define MAX_TUNING_RES 128
#define MAX_MIME_CONTENT_TYPE 128
static char g_tuningResourceMimeTypes[MAX_TUNING_RES][MAX_MIME_CONTENT_TYPE];
static PLATFORM_TCHAR g_tuningResourceFiles[MAX_TUNING_RES][VE_MAX_STRING_LENGTH];
static unsigned char *g_pTuningResourceData[MAX_TUNING_RES];
static VE_HRESOURCE g_hTuningResource[MAX_TUNING_RES];
static NUAN_U32 g_cTuningResources = 0U;

#ifdef _PRIVATE_HEAP_
#define PRIV_HEAP_SIZE (8*1024*1024)
static long g_aHeapMemory[PRIV_HEAP_SIZE/sizeof(long)];
#endif

static void * hHeap = NULL;
static void *     hDataClass = NULL;

/* ******************************************************************
**  LOCAL FUNCTIONS
** ******************************************************************/
/* ------------------------------------------------------------------
**  Parse program arguments
** -----------------------------------------------------------------*/

static int ParseInputArgsFromArgv(
  int         argc,
  ARGV_CHAR * argv[],
  int         startArg
);

#ifdef UNDER_CE

static int ParseInputArgsFromString(
  LPTSTR szCmdLine
);

#endif /* UNDER_CE */

/* ------------------------------------------------------------------
**  Usage
** -----------------------------------------------------------------*/
#ifdef PLATFORM_UNICODE
static void Usage(
  const wchar_t *name);
#else
static void Usage(
  const char *name);
#endif

/* ------------------------------------------------------------------
**  Output callback function
**  This function receives messages from the TTS instance and writes
**  the generated PCM + marker data to file.
** -----------------------------------------------------------------*/
static NUAN_ERROR CbOutNotifyToFile(
  VE_HINSTANCE           hTtsInst,
  void                 * pUserData,
  VE_CALLBACKMSG       * pcbMessage);

/* ------------------------------------------------------------------
**  Read Text
** -----------------------------------------------------------------*/
static NUAN_U32 ReadTextFromFile(
  const PLATFORM_TCHAR * szText,
  void                **ppText,
  size_t               *pnTextLen
);


/* -------------------------------------------------------------------
**  Utility functions handling char & PLATFORM_TCHAR differences
** -----------------------------------------------------------------*/
/* copy PLATFORM_TCHAR string into char string */
static char * NUAN_APP_tcscpy(
  char * dst,
  const PLATFORM_TCHAR * src
);
/* Print combination of char string and PLATFORM_TCHAR string */
static void NUAN_APP_Print(
  const char *szNString,
  const PLATFORM_TCHAR *szPString
);

/*------------------------------------------------------------------*/
static char * NUAN_APP_tcscpy(char * dst, const PLATFORM_TCHAR * src)
{
    char * cp = dst;

    /*lint --e(571) */
    while( (*cp = (char)*src) != 0 ) { cp++; src++; }/* Copy src to end of dst */

    return( dst );
} /* NUAN_APP_tcscpy */

/*------------------------------------------------------------------*/
static void NUAN_APP_Print(const char *szNString, const PLATFORM_TCHAR *szPString)
{
    char szBuf[VE_MAX_STRING_LENGTH];
    char *cp;
    size_t len;

    /* copy char */
    strcpy(szBuf, szNString);

    /* copy PLATFORM_TCHAR to char */
    len = strlen(szBuf);
    cp = &szBuf[len];
    /*lint --e(571) */
    while( ((*cp = (char) *szPString) != 0) && (len < VE_MAX_STRING_LENGTH-1) ) { cp++; szPString++; len++;}
    *cp = 0;

    vplatform_printf("%s", szBuf);
}

/*------------------------------------------------------------------*/
static int ParseInputArgsFromArgv(
  int         argc,
  ARGV_CHAR * argv[],
  int         startArg)
{
  int i;
  NUAN_BOOL bLanguage = NUAN_FALSE, bVoice = NUAN_FALSE;
  const ARGV_CHAR *ptr;

  /* Initialize params with default values */

  memset(TtsParam, 0, sizeof(TtsParam));
  cTtsParam = 0;

  TtsParam[cTtsParam].eID = VE_PARAM_MARKER_MODE;
  TtsParam[cTtsParam++].uValue.usValue = (NUAN_U16) VE_MRK_ON;

  TtsParam[cTtsParam].eID = VE_PARAM_WAITFACTOR;
  TtsParam[cTtsParam++].uValue.usValue = 1;

  /* init mode : no modules loaded */
  TtsParam[cTtsParam].eID = VE_PARAM_INITMODE;
  TtsParam[cTtsParam++].uValue.usValue = VE_INITMODE_LOAD_OPEN_ALL_EACH_TIME  ;


  /* init install path */
  g_aszDataInstallPaths[0][0] = 0;
  g_u16NbrOfDataInstallPaths = 0;

  /* init tuning resources */
  g_cTuningResources = 0;

  if (argc <= startArg)
  {
    return -1;
  }

  i = startArg;
  while (i < argc)
  {
    if (argv[i][0] != (ARGV_CHAR)'-')
    {
      return -1;
    }

    switch (argv[i][1])
    {
    case (ARGV_CHAR)'l':
      i++;
      if (i == argc)
      {
        return -1;
      }
      bLanguage = NUAN_TRUE;
      TtsParam[cTtsParam].eID = VE_PARAM_LANGUAGE;
      ARGV_COPY_API(TtsParam[cTtsParam].uValue.szStringValue, argv[i]);
      cTtsParam++;
      break;
    case (ARGV_CHAR)'L':
      i++;
      if (i == argc)
      {
        return -1;
      }
      TtsParam[cTtsParam].eID = VE_PARAM_EXTRAESCLANG;
      ARGV_COPY_API(TtsParam[cTtsParam++].uValue.szStringValue, argv[i]);
      break;
    case (ARGV_CHAR)'v':
      i++;
      if (i == argc)
      {
        return -1;
      }
      bVoice = NUAN_TRUE;
      TtsParam[cTtsParam].eID = VE_PARAM_VOICE;
      ARGV_COPY_API(TtsParam[cTtsParam++].uValue.szStringValue, argv[i]);
      break;
    case (ARGV_CHAR)'O':
      i++;
      if (i == argc)
      {
        return -1;
      }
      TtsParam[cTtsParam].eID = VE_PARAM_VOICE_OPERATING_POINT;
      ARGV_COPY_API(TtsParam[cTtsParam++].uValue.szStringValue, argv[i]);
      break;
    case (ARGV_CHAR)'I':
      i++;
      if (i == argc)
      {
        return -1;
      }
      if (g_u16NbrOfDataInstallPaths < MAX_DATA_INSTALL_PATHS)
      {
        ARGV_COPY(g_aszDataInstallPaths[g_u16NbrOfDataInstallPaths], argv[i]);
        g_u16NbrOfDataInstallPaths++;
      }
      break;
    case (ARGV_CHAR)'b':
      i++;
      if (i == argc)
      {
        return -1;
      }
      ARGV_COPY(g_szBrokerInfoFile, argv[i]);
      break;
    case (ARGV_CHAR)'i':
      i++;
      if (i == argc)
      {
        return -1;
      }
      ARGV_COPY(szTextFile, argv[i]);
      break;
    case (ARGV_CHAR)'o':
      i++;
      if (i == argc)
      {
        return -1;
      }
      ARGV_COPY(szOutFile, argv[i]);
      break;
    case (ARGV_CHAR)'R':
      i++;
      if (i == argc)
      {
        return -1;
      }
      TtsParam[cTtsParam].eID = VE_PARAM_SPEECHRATE;
      TtsParam[cTtsParam++].uValue.usValue = ARGV_ATOI(argv[i]);
      break;
    case (ARGV_CHAR)'M':
      i++;
      if (i == argc)
      {
        return -1;
      }
      TtsParam[0].eID = VE_PARAM_MARKER_MODE;
      TtsParam[0].uValue.usValue = ARGV_ATOI(argv[i]);
      break;
    case (ARGV_CHAR)'F':
      i++;
      if (i == argc)
      {
        return -1;
      }
      TtsParam[cTtsParam].eID = VE_PARAM_FREQUENCY;
      TtsParam[cTtsParam++].uValue.usValue = ARGV_ATOI(argv[i]);
      break;
    case (ARGV_CHAR)'V':
      i++;
      if (i == argc)
      {
        return -1;
      }
      TtsParam[cTtsParam].eID = VE_PARAM_VOLUME;
      TtsParam[cTtsParam++].uValue.usValue = ARGV_ATOI(argv[i]);
      break;
    case (ARGV_CHAR)'P':
      i++;
      if (i == argc)
      {
        return -1;
      }
      TtsParam[cTtsParam].eID = VE_PARAM_PITCH;
      TtsParam[cTtsParam++].uValue.usValue = ARGV_ATOI(argv[i]);
      break;
    case (ARGV_CHAR)'T':
      i++;
      if (i == argc)
      {
        return -1;
      }
      TtsParam[cTtsParam].eID = VE_PARAM_TYPE_OF_CHAR;
      TtsParam[cTtsParam].uValue.usValue = ARGV_ATOI(argv[i]);
      cTtsParam++;
      break;
    case (ARGV_CHAR)'t':
      i++;
      if (i == argc)
      {
        return -1;
      }
      if (ARGV_ATOI(argv[i]) == 2)
      {
        eTextFormat = VE_SSML_TEXT;
      }
      else if (ARGV_ATOI(argv[i]) != 1)
      {
        vplatform_printf("Error: Invalid Text format setting, 1: plain text, 2: SSML\n");
        return -1;
      }
      break;
    case (ARGV_CHAR)'B':
      i++;
      if (i == argc)
      {
        return -1;
      }
      cDirScanning = (NUAN_S16)ARGV_ATOI(argv[i]);
      break;
    case (ARGV_CHAR)'e':
      bExtraInfo = NUAN_TRUE;
      break;
    case (ARGV_CHAR)'n':
      bNoOutput = NUAN_TRUE;
      break;
    case (ARGV_CHAR)'x':
      /* Argument is of the form "<MIME type> <path>",
      ** where <MIME type> is one of:
      **
      ** Description       <MIME type> value
      ** -----------------------------------------------------------
      ** User dictionary   application/edct-bin-dictionary
      ** User ruleset      application/x-vocalizer-rettt+text
      ** ActivePrompt DB   application/x-vocalizer-activeprompt-db
      */
      i++;
      if (i == argc)
      {
        return -1;
      }
      if (g_cTuningResources >= MAX_TUNING_RES)
      {
        vplatform_printf("Error: Too many tuning resources, max is %d\n",
          MAX_TUNING_RES);
        return -1;
      }

      ptr = argv[i];
      while ((*ptr != (ARGV_CHAR)0) && (*ptr != (ARGV_CHAR)' '))
      {
        ptr++;
      }
      if (*ptr == (ARGV_CHAR)0)
      {
        NUAN_APP_Print("Error: Invalid -x argument \"", argv[i]);
        vplatform_printf("\", must be in \"<MIME type> <path>\" form\n");
        return -1;
      }

      ARGV_COPY_API(g_tuningResourceMimeTypes[g_cTuningResources], argv[i]);
      g_tuningResourceMimeTypes[g_cTuningResources][ptr - argv[i]] = 0;
      ARGV_COPY(g_tuningResourceFiles[g_cTuningResources], ptr + 1);
      g_pTuningResourceData[g_cTuningResources] = NULL;
      g_cTuningResources++;
      break;

    default:
      return -1;
      }
      i++;
  }

  /* Check required options */
  if ((g_aszDataInstallPaths[0][0] == 0) && (g_szBrokerInfoFile[0] == 0) && (cDirScanning == -1))
  {
    vplatform_printf("No install path or broker info file specified\n");
    return -1;
  }
  if ((! bLanguage) && (! bVoice))
  {
    vplatform_printf("No language and/or voice specified\n");
    return -1;
  }
  if (szTextFile[0] == 0)
  {
    vplatform_printf("No input text file specified\n");
    return -1;
  }

  /* init pointers to install paths */
  for (i = 0; i < g_u16NbrOfDataInstallPaths; i++)
  {
    g_apDataInstallPaths[i] = g_aszDataInstallPaths[i];
  }

  return 0;
} /* ParseInputArgsFromArgv */

#ifdef UNDER_CE

/*------------------------------------------------------------------*/
static int ParseInputArgsFromString(
  LPTSTR szCmdLine)
{
  PLATFORM_TCHAR * pszStr = NULL;
  PLATFORM_TCHAR ** argv;
  int i, j, argc, pathwithspaces;
  PLATFORM_TCHAR szWorkStr[256];
  int iRet = 0;

  /* Count number of command line arguments */
  pszStr = szCmdLine;
  if (pszStr == NULL) return (-1);
  pathwithspaces = 0;
  argc = 0;
  do
  {
    switch (*pszStr)
    {
    case (PLATFORM_TCHAR)('\"'):
      if (pathwithspaces) pathwithspaces = 0;
      else pathwithspaces = 1;
      break;
    case (PLATFORM_TCHAR)(' '):
    case (PLATFORM_TCHAR)('\0'):
      if (!pathwithspaces) argc++;
      break;
    default:
      break;
    }
  }
  while (*pszStr++ != 0);

  /* Split the command line */
  argv = (PLATFORM_TCHAR**) vplatform_heap_Malloc(hHeap, sizeof(PLATFORM_TCHAR*) * (size_t) argc);
  if (argv == NULL)
  {
    vplatform_printf("Failed to allocate memory\n");
    return -1;
  }
  i = j = 0;
  pszStr = szCmdLine;
  do
  {
    switch (*pszStr)
    {
    case (PLATFORM_TCHAR)('\"'):
      if (pathwithspaces) pathwithspaces = 0;
      else pathwithspaces = 1;
      break;
    case (PLATFORM_TCHAR)(' '):
    case (PLATFORM_TCHAR)('\0'):
      if (!pathwithspaces)
      {
        szWorkStr[i] = 0;
        argv[j] = (PLATFORM_TCHAR*) vplatform_heap_Malloc(hHeap, (size_t)(i + 1) * sizeof(PLATFORM_TCHAR));
        if (argv[j] == NULL)
        {
          vplatform_printf("Failed to allocate memory\n");
          for (i = 0; i < j; j++) vplatform_heap_Free(hHeap, argv[i]);
          vplatform_heap_Free(hHeap, argv);
          return -1;
        }
        _tcscpy(argv[j], szWorkStr);
        i = 0;
        j++;
      }
      else
      {
        szWorkStr[i++] = *pszStr;
      }
      break;
    default:
      szWorkStr[i++] = *pszStr;
      break;
    }
  }
  while (*pszStr++ != 0);

  /* Parse command line */
  iRet = ParseInputArgsFromArgv(argc, argv, 0);

  /* Free allocated memory */
  for (i = 0; i < argc; i++)
  {
    vplatform_heap_Free(hHeap, argv[i]);
  }
  vplatform_heap_Free(hHeap, argv);

  return iRet;
} /* ParseInputArgsFromString */

#endif /* UNDER_CE */

/*------------------------------------------------------------------*/
#ifdef PLATFORM_UNICODE
static void Usage(
  const wchar_t *name)
#else
static void Usage(
  const char *name)
#endif

{
  NUAN_APP_Print("Usage: ", name);
  vplatform_printf(
    "  [options]\n" \
    "Required options:\n" \
    "  -I InstallPath\n" \
    "  -v Voice\n" \
    "  -i TextFile\n" \
    "Optional options:\n" \
    "  -l Language\n" \
    "  -L Extra language (for multi-lingual voices)\n" \
    "  -b BrokerInfoFile\n" \
    "  -O VoiceOperatingPoint (e.g. one of embedded-pro, embedded-compact, embedded-high, premium-high)\n" \
    "  -F Frequency\n" \
    "  -M MarkerEnable (0 | 1)\n" \
    "  -R SpeechRate\n" \
    "  -V VolumeLevel\n" \
    "  -P PitchLevel\n" \
    "  -T CharType (1 = UTF16 | 2 = UTF8)\n" \
    "  -t TextFormat (1 = Plain Text | 2 = SSML)\n" \
    "  -o OutputFile\n" \
    "  -x \"<MimeType> <TuningDataFile>\"\n" \
    "  -B <0 | 1> (0 = dir scan + save broker files | 1 = use broker files)\n" \
    "  -e (extra info)\n" \
    "  -n (no output)\n"
   );
  NUAN_APP_Print("Example:\n  ", name);
  vplatform_printf(" -I \"c:\\ve\" -v Samantha -i \"c:\\test.txt\" -o \"c:\\test.pcm\"\n");
} /* Usage */

/*------------------------------------------------------------------*/
static NUAN_ERROR CbOutNotifyToFile(
  VE_HINSTANCE           hTtsInst,
  void                 * pUserData,
  VE_CALLBACKMSG       * pcbMessage)
{
  VE_OUTDATA      *pTtsOutData;
  VPLATFORM_FILE_H fPCM;
  NUAN_U32         i;
  NUAN_ERROR       ret = NUAN_OK;
  VE_LIPSYNC       TtsLipSync;

  fPCM = (VPLATFORM_FILE_H)pUserData;

  /*lint -save -e788 enum constants not used within switch */
  switch (pcbMessage->eMessage)
  {
  case VE_MSG_BEGINPROCESS:
    vplatform_printf("VE_MSG_BEGINPROCESS");
    if ((bExtraInfo == NUAN_TRUE) || (bNoOutput == NUAN_TRUE))  vplatform_printf("\n");
    break;

  case VE_MSG_ENDPROCESS:
    if ((bExtraInfo == NUAN_FALSE) && (bNoOutput == NUAN_FALSE))  vplatform_printf("\n");
    vplatform_printf("VE_MSG_ENDPROCESS\n");
    break;

  case VE_MSG_OUTBUFDONE:
    pTtsOutData = (VE_OUTDATA *)pcbMessage->pParam;
    if ((pTtsOutData->cntPcmBufLen != 0) && (fPCM != NULL))
    {
      vplatform_file_Write(pTtsOutData->pOutPcmBuf, 1, pTtsOutData->cntPcmBufLen, fPCM);
    }

    if ((bExtraInfo == NUAN_TRUE) && (bNoOutput == NUAN_FALSE) )
    {
        vplatform_printf("VE_MSG_OUTBUFDONE: %u samples, %u markers\n",
        (unsigned int)pTtsOutData->cntPcmBufLen/2,
        (unsigned int)pTtsOutData->cntMrkListLen);
    }

    for (i = 0;i < pTtsOutData->cntMrkListLen ;i++)
    {
      if (pTtsOutData->pMrkList[i].eMrkType == VE_MRK_PHONEME)
      {
        if ((bExtraInfo == NUAN_TRUE) && (bNoOutput == NUAN_FALSE) )
        {
            vplatform_printf("[Phon=%-3u] %-6u\t",
            (unsigned int)pTtsOutData->pMrkList[i].usPhoneme,
            (unsigned int)pTtsOutData->pMrkList[i].cntDestPos);
        }

        ret = ve_ttsGetLipSyncInfo(hTtsInst,
          pTtsOutData->pMrkList[i].usPhoneme, &TtsLipSync);
        if (ret == 0)
        {
          if ((bExtraInfo == NUAN_TRUE) && (bNoOutput == NUAN_FALSE) )
          {
            vplatform_printf("lipSyncInfo (%s): ", TtsLipSync.szLHPhoneme);
            vplatform_printf("%d %d %d %d %d %d %d %d\n",
                    TtsLipSync.sJawOpen, TtsLipSync.sTeethUpVisible,
                    TtsLipSync.sTeethLoVisible, TtsLipSync.sMouthHeight,
                    TtsLipSync.sMouthWidth, TtsLipSync.sMouthUpturn,
              TtsLipSync.sTonguePos, TtsLipSync.sLipTension);
          }
          else if (bNoOutput == NUAN_FALSE)
          {
            vplatform_printf("%s", TtsLipSync.szLHPhoneme);
          }
        }
        else if ((bExtraInfo == NUAN_TRUE) && (bNoOutput == NUAN_FALSE) )
        {
          vplatform_printf("lipSyncInfo not available\n");
        }
      }
      else  if (pTtsOutData->pMrkList[i].eMrkType ==  VE_MRK_TEXTUNIT)
      {
        if ((bExtraInfo == NUAN_TRUE) && (bNoOutput == NUAN_FALSE) )
        {
            vplatform_printf("[TextUnit] %u\tSrcPos=%u SrcLen=%u\n",
            (unsigned int)pTtsOutData->pMrkList[i].cntDestPos,
            (unsigned int)pTtsOutData->pMrkList[i].cntSrcPos,
            (unsigned int)pTtsOutData->pMrkList[i].cntSrcTextLen);
        }
        else if (bNoOutput == NUAN_FALSE)
        {
            vplatform_printf("\n");
        }
      }
      else  if (pTtsOutData->pMrkList[i].eMrkType == VE_MRK_PROMPT)
      {
        if ((bExtraInfo == NUAN_TRUE) && (bNoOutput == NUAN_FALSE) )
        {
            vplatform_printf("[Prompt]\tDestPos=%u SrcPos=%u  ",
            (unsigned int)pTtsOutData->pMrkList[i].cntDestPos,
            (unsigned int)pTtsOutData->pMrkList[i].cntSrcPos);
            vplatform_printf("PromptId=%s\n",pTtsOutData->pMrkList[i].szPromptID);
        }
        else if (bNoOutput == NUAN_FALSE)
        {
            vplatform_printf("\n");
        }
      }
      else if ((bExtraInfo == NUAN_TRUE) && (bNoOutput == NUAN_FALSE) )
      {
        if (pTtsOutData->pMrkList[i].eMrkType == VE_MRK_WORD)
        {
            vplatform_printf("[Word] %u\tSrcPos=%u SrcLen=%u\n",
            (unsigned int)pTtsOutData->pMrkList[i].cntDestPos,
            (unsigned int)pTtsOutData->pMrkList[i].cntSrcPos,
            (unsigned int)pTtsOutData->pMrkList[i].cntSrcTextLen);
        }
        else if (pTtsOutData->pMrkList[i].eMrkType ==
          VE_MRK_BOOKMARK  )
        {
            vplatform_printf("[BookMark=%u] %u\tSrcPos=%u SrcLen=%u\n",
            (unsigned int)pTtsOutData->pMrkList[i].cntDestPos,
            (unsigned int)pTtsOutData->pMrkList[i].ulMrkId,
            (unsigned int)pTtsOutData->pMrkList[i].cntSrcPos,
            (unsigned int)pTtsOutData->pMrkList[i].cntSrcTextLen);
        }
        else
        {
            vplatform_printf("[Type=%u] %u\tSrcPos=%u SrcLen=%u\n",
            (unsigned int)pTtsOutData->pMrkList[i].eMrkType,
            (unsigned int)pTtsOutData->pMrkList[i].cntDestPos,
            (unsigned int)pTtsOutData->pMrkList[i].cntSrcPos,
            (unsigned int)pTtsOutData->pMrkList[i].cntSrcTextLen);
        }
      }
    }

    break;

  case VE_MSG_OUTBUFREQ:
    pTtsOutData = (VE_OUTDATA *)pcbMessage->pParam;
    pTtsOutData->pOutPcmBuf   = pPcmData;
    pTtsOutData->cntPcmBufLen  = PCM_BUF_SIZE;
    pTtsOutData->pMrkList     = pMrkData;
    pTtsOutData->cntMrkListLen = MRK_BUF_SIZE*sizeof(VE_MARKINFO);
    break;

  default:
    break;
  }

  /*lint -restore */
  return 0;
} /* CbOutNotifyToFile */

/*------------------------------------------------------------------*/
static NUAN_U32 ReadTextFromFile(
  const PLATFORM_TCHAR        *szName,
  void                       **ppText,
  size_t                      *pnTextLen)
{
    size_t nTextLen = 0;
    NUAN_ERROR fRet = NUAN_OK;
    VPLATFORM_FILE_H hFile;

    *ppText = NULL;
    *pnTextLen = 0;

    fRet = vplatform_file_Open(hDataClass, hHeap, szName, _T("rb"), &hFile);
    if (fRet != NUAN_OK) return fRet;

    nTextLen = vplatform_file_GetSize(hFile);

    *ppText = vplatform_heap_Calloc(hHeap, 1, nTextLen + 2);
    if (*ppText == NULL)
    {
        vplatform_file_Close(hFile);
        return NUAN_E_OUTOFMEMORY;
    }

    if (nTextLen > 0)
    {
        *pnTextLen = vplatform_file_Read(*ppText, sizeof(char), nTextLen, hFile);
    }

    vplatform_file_Close(hFile);

    return fRet;

} /* ReadTextFromFile */

/* ------------------------------------------------------------------
**  Main
**  The sample program creates the TTS class and a TTS instance, and
**  configures the TTS instance (with language, voice, voice operating point,
**  etc.). Then it has the TTS instance read a text file, and writes
**  the synthesized audio to file. At the end it closes the TTS instance
**  and the TTS class.
** -----------------------------------------------------------------*/

#ifdef UNDER_CE

int WINAPI WinMain(
  HINSTANCE hInstance,
  HINSTANCE hPrevInstance,
  LPTSTR szCmdLine,
  int iCmdShow)

#else

#ifdef PLATFORM_UNICODE
int wmain(
  int       argc,
  wchar_t * argv[])
#else
int main(
  int     argc,
  char *  argv[])
#endif

#endif
{
  int                    mainRc = 0;
  NUAN_ERROR             nErrcode = 0;
  VPLATFORM_FILE_H       fPCM = NULL;  /* file pointer for output PCM file */
  VE_INSTALL             stInstall;
  VPLATFORM_RESOURCES    stResources;
  VE_HSPEECH             hSpeech;
  VE_HINSTANCE           hTtsInst;
  VE_OUTDEVINFO          stOutDevInfo;
  VE_INTEXT              stInText;
  size_t                 nTextLen;
  void                 * pText = NULL;
  NUAN_U16               i = 0, j = 0, k = 0;
  NUAN_U16               nItem = 0, nItem2 = 0, nItem3 = 0;
  VE_LANGUAGE          * pLangList;
  VE_VOICEINFO         * pVoiceList;
  VE_NTSINFO             NtsInfo;
  VE_CLMINFO             ClmInfo;
  VE_SPEECHDBINFO      * pSpeechDBList;
  void                 * pBinBrokerInfo = NULL;
  size_t                 cntBrokerSize = 0;
  NUAN_U32               iRes = 0;
  VE_PRODUCT_VERSION     ttsProductVersion;

  nErrcode = ve_ttsGetProductVersion(&ttsProductVersion);
  if (nErrcode == NUAN_OK)
  {
      vplatform_printf("\nVocalizer Expressive v%d.%d.%d\n\n", ttsProductVersion.major, ttsProductVersion.minor, ttsProductVersion.maint);
  } /* end if */

  /* Parse the command line arguments */
#ifdef UNDER_CE
  if (ParseInputArgsFromString(szCmdLine))
  {
    Usage(_T("sample.exe"));
    return 1;
  }
#else
  if (ParseInputArgsFromArgv(argc, argv, 1))
  {
    Usage(argv[0]);
    return 1;
  }
#endif /* UNDER_CE */

  vplatform_printf("Initialize\n");

  memset(&stInstall, 0, sizeof(stInstall));
  stInstall.fmtVersion = VE_CURRENT_VERSION;

  /* Get the platform dependent interfaces */
  memset(&stResources, 0, sizeof(stResources));
  stResources.fmtVersion = VPLATFORM_CURRENT_VERSION;
  if (cDirScanning != 1)
  {
    stResources.u16NbrOfDataInstall = g_u16NbrOfDataInstallPaths;
    stResources.apDataInstall = g_apDataInstallPaths;
  }
  if (cDirScanning != -1)
  {
    stResources.szBinaryBrokerFile  = _T("binbroker.txt");
    stResources.szFileListFile      = _T("filelist.txt");
  }
#ifdef _PRIVATE_HEAP_
  stResources.stHeap.pStart = (void *)&g_aHeapMemory[0];
  stResources.stHeap.cByte = PRIV_HEAP_SIZE;
#endif
  nErrcode = vplatform_GetInterfaces(&stInstall, &stResources);
  if (nErrcode != NUAN_OK)
  {
    vplatform_printf("Error (vplatform_GetInterfaces) : 0x%x\n", (unsigned int)nErrcode);
    mainRc = 1;
    goto exitMain;
  }

  /* init globals */
  hHeap = stInstall.hHeap;
  hDataClass = stInstall.hDataClass;

  /* read optional binary broker file */
  if (g_szBrokerInfoFile[0] != 0)
  {
    nErrcode = ReadTextFromFile(g_szBrokerInfoFile, &pBinBrokerInfo, &cntBrokerSize);
    if (nErrcode != NUAN_OK)
    {
      NUAN_APP_Print("Can't open ", g_szBrokerInfoFile);
      vplatform_printf("\n");
      mainRc = 1;
      goto exitRelease;
    }

    if (stInstall.pBinBrokerInfo != NULL) vplatform_heap_Free(hHeap, (void *)(stInstall.pBinBrokerInfo));
    stInstall.pBinBrokerInfo = pBinBrokerInfo;
  }

  /* Read the input text */
  nErrcode = ReadTextFromFile(szTextFile, &pText, &nTextLen);
  if ((nErrcode != NUAN_OK) || (nTextLen == 0))
  {
    NUAN_APP_Print("Can't read input from the file ", szTextFile);
    vplatform_printf("\n");
    mainRc = 1;
    goto exitRelease;
  }

  /* Allocate our PCM and marker buffers */
  pPcmData = (NUAN_S16 *) vplatform_heap_Malloc(hHeap, PCM_BUF_SIZE);
  if (pPcmData == NULL)
  {
    vplatform_printf("Failed to allocate memory\n");
    mainRc = 1;
    goto exitRelease;
  }

  pMrkData = (VE_MARKINFO *) vplatform_heap_Malloc(hHeap, MRK_BUF_SIZE * sizeof(VE_MARKINFO));
  if (pMrkData == NULL)
  {
    vplatform_printf("Failed to allocate memory\n");
    mainRc = 1;
    goto exitRelease;
  }

  /* Open the output PCM file */
  if ((szOutFile[0] != 0) && (bNoOutput == NUAN_FALSE))
  {
    nErrcode = vplatform_file_Open(hDataClass, hHeap, szOutFile, _T("wb"), &fPCM);
    if (nErrcode != NUAN_OK)
    {
      NUAN_APP_Print("Can't open ", szOutFile);
      vplatform_printf("\n");
      mainRc = 1;
      goto exitRelease;
    }
  }
  else
  {
    fPCM = NULL;
  }


  /* Initialize the engine */
  nErrcode = ve_ttsInitialize(&stInstall, &hSpeech);
  if (pBinBrokerInfo != NULL)
  {
    vplatform_heap_Free(hHeap, pBinBrokerInfo);
    /* assure vplatform_ReleaseInterfaces cannot free this as well */
    stInstall.pBinBrokerInfo = NULL;
  }
  if (nErrcode != NUAN_OK)
  {
    vplatform_printf("Error (vauto_ttsInitialize) : 0x%x\n", (unsigned int)nErrcode);
    mainRc = 1;
    goto exitRelease;
  }

  /* Optionally print the list of available languages, voices, and
  ** speechbases */
  if (bExtraInfo)
  {
    vplatform_printf("Number of languages available: ");
    nErrcode = ve_ttsGetLanguageList(hSpeech, NULL, &nItem);
    if (nErrcode != NUAN_OK)
    {
      vplatform_printf("\nError (ve_ttsGetLanguageList) : 0x%x\n",
        (unsigned int)nErrcode);
      mainRc = 1;
      goto exitUnInit;
    }
    vplatform_printf("%u\n", nItem);

    pLangList = (VE_LANGUAGE *)
      vplatform_heap_Malloc(hHeap, sizeof(VE_LANGUAGE) * nItem);
    if ((pLangList) == NULL)
    {
      vplatform_printf("Failed to allocate memory");
      goto exitUnInit;
    }

    /* vplatform_printf("Get the list of languages available\n"); */
    nErrcode = ve_ttsGetLanguageList(hSpeech, pLangList, &nItem);
    if (nErrcode != NUAN_OK)
    {
      vplatform_printf("Error (ve_ttsGetLanguageList) : 0x%x\n",
        (unsigned int)nErrcode);
      if (pLangList) vplatform_heap_Free(hHeap, pLangList);
      mainRc = 1;
      goto exitUnInit;
    }

    for (i = 0; i < nItem; i++)
    {
      vplatform_printf("%d>> %s, %s, %s\n", i,
                        pLangList[i].szLanguage,
                        pLangList[i].szLanguageTLW,
                        pLangList[i].szVersion);

      nErrcode = ve_ttsGetClmInfo(hSpeech, pLangList[i].szLanguage,&ClmInfo);
      if (nErrcode != NUAN_OK)
      {
        vplatform_printf("\nError (ve_ttsGetClmInfo) : 0x%x\n", (unsigned int)nErrcode);
      }
      vplatform_printf("  %d>> CLM File version: %s \n", j,
                        ClmInfo.szFileVersion);
      vplatform_printf("  %d>> CLM Source version: %s \n", j,
                        ClmInfo.szSrcVersion);
      vplatform_printf("  %d>> CLM Destination version: %s \n", j,
                        ClmInfo.szDstVersion);

      nErrcode = ve_ttsGetNtsInfo(hSpeech, pLangList[i].szLanguage,&NtsInfo);
      if (nErrcode != NUAN_OK)
      {
        vplatform_printf("\nError (ve_ttsGetNtsInfo) : 0x%x\n", (unsigned int)nErrcode);
      }
      vplatform_printf("  %d>> NTS: %s\n", j,
                        NtsInfo.szVersion);

      vplatform_printf("  Number of voices available: ");
      nErrcode = ve_ttsGetVoiceList(hSpeech, pLangList[i].szLanguage, NULL, &nItem2);
      if (nErrcode != NUAN_OK)
      {
        vplatform_printf("\nError (ve_ttsGetVoiceList) : 0x%x\n", (unsigned int)nErrcode);
        if (pLangList) vplatform_heap_Free(hHeap, pLangList);
        mainRc = 1;
        goto exitUnInit;
      }
      vplatform_printf("%u\n", nItem2);

      pVoiceList = (VE_VOICEINFO *)
        vplatform_heap_Malloc(hHeap, sizeof(VE_VOICEINFO) * nItem2);
      if ((pVoiceList) == NULL)
      {
        vplatform_printf("Failed to allocate memory");
        if (pLangList) vplatform_heap_Free(hHeap, pLangList);
        goto exitUnInit;
      }

      /* vplatform_printf("  Get the list of voices available\n");*/
      nErrcode = ve_ttsGetVoiceList(hSpeech, pLangList[i].szLanguage, pVoiceList, &nItem2);
      if (nErrcode != NUAN_OK)
      {
        vplatform_printf("Error (ve_ttsGetVoiceList) : 0x%x\n", (unsigned int)nErrcode);
        mainRc = 1;
        if (pVoiceList) vplatform_heap_Free(hHeap, pVoiceList);
        if (pLangList) vplatform_heap_Free(hHeap, pLangList);
        goto exitUnInit;
      }

      for (j = 0; j < nItem2; j++)
      {
        vplatform_printf("  %d>> %s, %s, %s\n", j,
                          pVoiceList[j].szVoiceName,
                          pVoiceList[j].szVoiceAge,
                          pVoiceList[j].szVoiceType);

        vplatform_printf("    Number of speech DBs available: ");
        nErrcode = ve_ttsGetSpeechDBList(hSpeech,
          pLangList[i].szLanguage, pVoiceList[j].szVoiceName, NULL,
          &nItem3);
        if (nErrcode != NUAN_OK)
        {
          vplatform_printf("\nError (ve_ttsGetSpeechDBList) : 0x%x\n",
            (unsigned int)nErrcode);
          if (pVoiceList) vplatform_heap_Free(hHeap, pVoiceList);
          if (pLangList) vplatform_heap_Free(hHeap, pLangList);
          mainRc = 1;
          goto exitUnInit;
        }
        vplatform_printf("%u\n", nItem3);

        pSpeechDBList = (VE_SPEECHDBINFO *)
          vplatform_heap_Malloc(hHeap, sizeof(VE_SPEECHDBINFO) * nItem3);
        if ((pSpeechDBList) == NULL)
        {
          vplatform_printf("Failed to allocate memory");
          if (pLangList) vplatform_heap_Free(hHeap, pLangList);
          if (pVoiceList) vplatform_heap_Free(hHeap, pVoiceList);
          goto exitUnInit;
        }

        /* vplatform_printf("    Get the list of speech DBs available\n");*/
        nErrcode = ve_ttsGetSpeechDBList(hSpeech,
          pLangList[i].szLanguage, pVoiceList[j].szVoiceName,
          pSpeechDBList, &nItem3);
        if (nErrcode != NUAN_OK)
        {
          vplatform_printf("Error (ve_ttsGetSpeechDBList) : 0x%x\n",
            (unsigned int)nErrcode);
          if (pSpeechDBList) vplatform_heap_Free(hHeap, pSpeechDBList);
          if (pVoiceList) vplatform_heap_Free(hHeap, pVoiceList);
          if (pLangList) vplatform_heap_Free(hHeap, pLangList);
          mainRc = 1;
          goto exitUnInit;
        }

        for (k = 0; k < nItem3; k++)
        {
          vplatform_printf("    %d>> %s, %hu kHz, %s\n", k,
                             pSpeechDBList[k].szVoiceOperatingPoint,
                             pSpeechDBList[k].u16Freq,
                             pSpeechDBList[k].szVersion);
        }

        vplatform_heap_Free(hHeap, pSpeechDBList);
        pSpeechDBList = NULL;
      }

      vplatform_heap_Free(hHeap, pVoiceList);
      pVoiceList = NULL;
    }



    vplatform_heap_Free(hHeap, pLangList);
  }

  /* Create the TTS instance */
  vplatform_printf("Create TTS instance\n");
  nErrcode = ve_ttsOpen(hSpeech, stInstall.hHeap, stInstall.hLog,
    &hTtsInst);
  if (nErrcode != NUAN_OK)
  {
    vplatform_printf("Error (ve_ttsOpen) : 0x%x\n", (unsigned int)nErrcode);
    mainRc = 1;
    goto exitUnInit;
  }
  /* Set parameters, these are initialized from the command line arguments */
  vplatform_printf("Set TTS parameters\n");
  nErrcode = ve_ttsSetParamList(hTtsInst , &TtsParam[0] , cTtsParam);
  if (nErrcode != NUAN_OK)
  {
    vplatform_printf("Error (ve_ttsSetParamList) : 0x%x\n", (unsigned int)nErrcode);
    mainRc = 1;
    goto exitClose;
  }

  /* Configure the system to load all modules at startup */
  TtsParam[0].eID = VE_PARAM_INITMODE;
  TtsParam[0].uValue.usValue = VE_INITMODE_LOAD_ONCE_OPEN_ALL;
  nErrcode = ve_ttsSetParamList(hTtsInst , &TtsParam[0] , 1);
  if (nErrcode != NUAN_OK)
  {
    vplatform_printf("Error (ve_ttsSetParamList: VE_PARAM_INITMODE) : 0x%x\n", (unsigned int)nErrcode);
    mainRc = 1;
    goto exitClose;
  }

  /* Set the output device */
  memset(&stOutDevInfo, 0, sizeof(stOutDevInfo));
  stOutDevInfo.pUserData = (void *) fPCM;
  stOutDevInfo.pfOutNotify = CbOutNotifyToFile;
  nErrcode = ve_ttsSetOutDevice(hTtsInst, &stOutDevInfo);
  if (nErrcode != NUAN_OK)
  {
    vplatform_printf("Error (ve_ttsSetOutDevice) : 0x%x\n", (unsigned int)nErrcode);
    mainRc = 1;
    goto exitClose;
  }

  for (iRes = 0; iRes < g_cTuningResources; iRes++)
  {
    /* Open the input resource file */
    size_t nResLen;

    nErrcode = ReadTextFromFile(g_tuningResourceFiles[iRes], (void**)&g_pTuningResourceData[iRes], &nResLen);
    if (nErrcode != NUAN_OK)
    {
      NUAN_APP_Print("Can't read ", g_tuningResourceFiles[iRes]);
      vplatform_printf("\n");
      mainRc = 1;
      goto exitClose;
    }

    if (g_pTuningResourceData[iRes] && nResLen)
    {
      nErrcode = ve_ttsResourceLoad(hTtsInst,
        g_tuningResourceMimeTypes[iRes], (NUAN_U32)nResLen,
        g_pTuningResourceData[iRes], &g_hTuningResource[iRes]);
      if (nErrcode != NUAN_OK)
      {
        NUAN_APP_Print("Error (ve_ttsResourceLoad ", g_tuningResourceFiles[iRes]);
        vplatform_printf(") : 0x%x\n", (unsigned int)nErrcode);
      }
    }
  }

  /*
  * Do text to speech processing
  */
  vplatform_printf("Process\n");

  /* Specify the text to synthesize */
  stInText.eTextFormat  = VE_NORM_TEXT;
  stInText.cntTextLength = (NUAN_U32)  nTextLen;
  stInText.szInText     = (void *)    pText;
  stInText.eTextFormat  = eTextFormat;
  /* Synthesis */
  nErrcode = ve_ttsProcessText2Speech(hTtsInst, &stInText);
  if (nErrcode != NUAN_OK)
  {
    vplatform_printf("Error (ve_ttsProcessText2Speech) : 0x%x\n",
      (unsigned int)nErrcode);
  }

  for (iRes = 0; iRes < g_cTuningResources; iRes++)
  {
    nErrcode = ve_ttsResourceUnload(hTtsInst, g_hTuningResource[iRes]);
    if (nErrcode != NUAN_OK)
    {
        NUAN_APP_Print("Error (ve_ttsResourceUnload ", g_tuningResourceFiles[iRes]);
        vplatform_printf(") : 0x%x\n", (unsigned int)nErrcode);
    }

    /* ve_ttsResourceLoad( ) requires that we keep the data
    ** around until we either unload it or close the instance */
    if (g_pTuningResourceData[iRes])
    {
      vplatform_heap_Free(hHeap, g_pTuningResourceData[iRes]);
    }
  }

exitClose:
  /* Close the TTS instance */
  vplatform_printf("Close TTS instance\n");
  nErrcode = ve_ttsClose(hTtsInst);
  if (nErrcode != NUAN_OK)
  {
    vplatform_printf("Error (ve_ttsClose) : 0x%x\n", (unsigned int)nErrcode);
  }

exitUnInit:
  /* Uninitialize */
  vplatform_printf("Uninitialize\n");
  nErrcode = ve_ttsUnInitialize(hSpeech);
  if (nErrcode != NUAN_OK)
  {
    vplatform_printf("Error (ve_ttsUnInitialize) : 0x%x\n", (unsigned int)nErrcode);
  }
exitRelease:
  /* Clean up */
  if (pText != NULL) vplatform_heap_Free(hHeap, pText);
  if (pPcmData != NULL) vplatform_heap_Free(hHeap, pPcmData);
  if (pMrkData != NULL) vplatform_heap_Free(hHeap, pMrkData);
  if (fPCM != NULL) vplatform_file_Close(fPCM);

  /* release platform interfaces */
  vplatform_ReleaseInterfaces(&stInstall);

exitMain:
  return mainRc;
} /* main */

/* ******************************************************************
**  END
** ******************************************************************/
