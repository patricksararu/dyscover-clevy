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
#ifdef UNDER_CE
#include <windows.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vprintf.h"
#include "ve_ttsapi.h"
#include "vplatform.h"
#include "vplatform_tchar.h"
#include "vheap.h"
#include "vfile.h"

/* ******************************************************************
**  DEFINITIONS
** ******************************************************************/
#define PCM_BUF_SIZE                4096 /* in bytes */
#define MRK_BUF_SIZE                 100 /* in VE_MARKINFO structs */
#define TA_BUF_SIZE                  500

#define ARGV_CHAR                   PLATFORM_TCHAR
#define ARGV_ATOI(x)                ((NUAN_U16)_ttoi(x))
#define ARGV_COPY(dest, src)        _tcscpy(dest, src)
#define ARGV_COPY_API(dest, src)    NUAN_APP_tcscpy(dest, src)

/* ******************************************************************
**  GLOBALS
** ******************************************************************/
static PLATFORM_TCHAR   szTextFile[VE_MAX_STRING_LENGTH]           = { 0 };
static PLATFORM_TCHAR   szOutFile[VE_MAX_STRING_LENGTH]            = { 0 };
static VE_PARAM         TtsParam[16];
static NUAN_U16         cTtsParam                                     = 0;

static NUAN_S16       * pPcmData                                      = NULL;
static VE_MARKINFO    * pMrkData                                      = NULL;

static NUAN_BOOL bExtraInfo = NUAN_FALSE;
static VE_TEXTFORMAT eTextFormat = VE_NORM_TEXT;

#define MAX_DATA_INSTALL_PATHS   64
static NUAN_U16         g_u16NbrOfDataInstallPaths                    = 0;
static PLATFORM_TCHAR   g_aszDataInstallPaths[MAX_DATA_INSTALL_PATHS][VE_MAX_STRING_LENGTH];
static PLATFORM_TCHAR * g_apDataInstallPaths[MAX_DATA_INSTALL_PATHS];
static PLATFORM_TCHAR   g_szBrokerInfoFile[VE_MAX_STRING_LENGTH]   = { 0 };

#define MAX_TUNING_RES          128
#define MAX_MIME_CONTENT_TYPE   128
static char             g_tuningResourceMimeTypes[MAX_TUNING_RES][MAX_MIME_CONTENT_TYPE];
static PLATFORM_TCHAR   g_tuningResourceFiles[MAX_TUNING_RES][VE_MAX_STRING_LENGTH];
static unsigned char  * g_pTuningResourceData[MAX_TUNING_RES];
static VE_HRESOURCE     g_hTuningResource[MAX_TUNING_RES];
static NUAN_U32         g_cTuningResources                            = 0U;

static VE_TA_NODE       g_taInfo[TA_BUF_SIZE];
static char           * g_rewrittenTextBuf                            = NULL;
static unsigned long    g_textIndex                                   = 0;
static unsigned long    g_taIndex                                     = 0;
static unsigned long    g_taCurIndex                                  = 0;

/* Global variables to help simulate user interrupts
** to jump to a different portion of the text. */
struct jumpToStruct
{
  NUAN_U32  jumpToPos;
  NUAN_U32  interruptAfterNrOfFrames;
};
static NUAN_U32             g_nrOfJumpToPointsOnCommandLine = 0;
static struct jumpToStruct  g_jumpToArray[20];
static NUAN_BOOL            g_simulateInterrupt             = NUAN_FALSE;
static NUAN_U32             g_outputBufferCounter           = 0;
static NUAN_U32             g_interruptOnBuffer             = 0;

static void * hHeap = NULL;
static void *     hDataClass = NULL;

/* ------------------------------------------------------------------
**  Parse program arguments
** -----------------------------------------------------------------*/

static int ParseInputArgsFromArgv(
  int         argc,
  ARGV_CHAR * argv[],
  int         startArg
);

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
  VE_CALLBACKMSG       * pcbMessage
);


/* ------------------------------------------------------------------
**  Read Text
** -----------------------------------------------------------------*/
static NUAN_U32 ReadTextFromFile(
  const PLATFORM_TCHAR * szText,
  void                **ppText,
  NUAN_U32             *pnTextLen
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
  int              i          = 0;
  NUAN_BOOL        bLanguage  = NUAN_FALSE;
  NUAN_BOOL        bVoice     = NUAN_FALSE;
  const ARGV_CHAR *ptr        = NULL;

  /* Initialize params with default values */
  memset(TtsParam, 0, sizeof(TtsParam));
  cTtsParam = 0;

  TtsParam[cTtsParam].eID              = VE_PARAM_MARKER_MODE;
  TtsParam[cTtsParam++].uValue.usValue = (NUAN_U16) VE_MRK_ON;

  TtsParam[cTtsParam].eID              = VE_PARAM_WAITFACTOR;
  TtsParam[cTtsParam++].uValue.usValue = 1;

  /* init mode : no modules loaded */
  TtsParam[cTtsParam].eID              = VE_PARAM_INITMODE;
  TtsParam[cTtsParam++].uValue.usValue = VE_INITMODE_LOAD_OPEN_ALL_EACH_TIME;

  /* init install path */
  g_aszDataInstallPaths[0][0] = 0;
  g_u16NbrOfDataInstallPaths  = 0;

  /* init tuning resources */
  g_cTuningResources = 0;

  if (argc <= startArg)
  {
    return -1;
  } /* end if */

  i = startArg;
  while (i < argc)
  {
    if (argv[i][0] != (ARGV_CHAR)'-')
    {
      return -1;
    } /* end if */

    switch (argv[i][1])
    {
      case (ARGV_CHAR)'l':
        i++;
        bLanguage = NUAN_TRUE;
        TtsParam[cTtsParam].eID = VE_PARAM_LANGUAGE;
        ARGV_COPY_API(TtsParam[cTtsParam].uValue.szStringValue, argv[i]);
        cTtsParam++;
        break;
      case (ARGV_CHAR)'L':
        i++;
        TtsParam[cTtsParam].eID = VE_PARAM_EXTRAESCLANG;
        ARGV_COPY_API(TtsParam[cTtsParam++].uValue.szStringValue, argv[i]);
        break;
      case (ARGV_CHAR)'v':
        i++;
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
        if (g_u16NbrOfDataInstallPaths < MAX_DATA_INSTALL_PATHS)
        {
          ARGV_COPY(g_aszDataInstallPaths[g_u16NbrOfDataInstallPaths], argv[i]);
          g_u16NbrOfDataInstallPaths++;
        } /* end if */
        break;
      case (ARGV_CHAR)'b':
        i++;
        ARGV_COPY(g_szBrokerInfoFile, argv[i]);
        break;
      case (ARGV_CHAR)'i':
        i++;
        ARGV_COPY(szTextFile, argv[i]);
        break;
      case (ARGV_CHAR)'o':
        i++;
        ARGV_COPY(szOutFile, argv[i]);
        break;
      case (ARGV_CHAR)'R':
        i++;
        TtsParam[cTtsParam].eID              = VE_PARAM_SPEECHRATE;
        TtsParam[cTtsParam++].uValue.usValue = ARGV_ATOI(argv[i]);
        break;
      case (ARGV_CHAR)'F':
        i++;
        TtsParam[cTtsParam].eID              = VE_PARAM_FREQUENCY;
        TtsParam[cTtsParam++].uValue.usValue = ARGV_ATOI(argv[i]);
        break;
      case (ARGV_CHAR)'V':
        i++;
        TtsParam[cTtsParam].eID              = VE_PARAM_VOLUME;
        TtsParam[cTtsParam++].uValue.usValue = ARGV_ATOI(argv[i]);
        break;
      case (ARGV_CHAR)'P':
        i++;
        TtsParam[cTtsParam].eID              = VE_PARAM_PITCH;
        TtsParam[cTtsParam++].uValue.usValue = ARGV_ATOI(argv[i]);
        break;
      case (ARGV_CHAR)'T':
        i++;
        TtsParam[cTtsParam].eID              = VE_PARAM_TYPE_OF_CHAR;
        TtsParam[cTtsParam++].uValue.usValue = ARGV_ATOI(argv[i]);
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
      case (ARGV_CHAR)'M':
        i++;
        TtsParam[cTtsParam].eID              = VE_PARAM_TEXTMODE;
        TtsParam[cTtsParam++].uValue.usValue = ARGV_ATOI(argv[i]);
        break;
      case (ARGV_CHAR)'j':
        i++;
        g_jumpToArray[g_nrOfJumpToPointsOnCommandLine].jumpToPos = ARGV_ATOI(argv[i]);
        i++;
        g_jumpToArray[g_nrOfJumpToPointsOnCommandLine].interruptAfterNrOfFrames = ARGV_ATOI(argv[i]);
        g_nrOfJumpToPointsOnCommandLine++;
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
        if (g_cTuningResources >= MAX_TUNING_RES)
        {
          vplatform_printf("Error: Too many tuning resources, max is %d\n", MAX_TUNING_RES);
          return -1;
        } /* end if */

        ptr = argv[i];
        while ((*ptr != (ARGV_CHAR)0) && (*ptr != (ARGV_CHAR)' '))
        {
          ptr++;
        } /* end while */
        if (*ptr == (ARGV_CHAR)0)
        {
          NUAN_APP_Print("Error: Invalid -x argument \"", argv[i]);
          vplatform_printf("\", must be in \"<MIME type> <path>\" form\n");          return -1;
        } /* end if */

        ARGV_COPY_API(g_tuningResourceMimeTypes[g_cTuningResources], argv[i]);
        g_tuningResourceMimeTypes[g_cTuningResources][ptr - argv[i]] = 0;
        ARGV_COPY(g_tuningResourceFiles[g_cTuningResources], ptr + 1);
        g_pTuningResourceData[g_cTuningResources] = NULL;
        g_cTuningResources++;
        break;
      case (ARGV_CHAR)'e':
        bExtraInfo = NUAN_TRUE;
        break;
      default:
        return -1;
    }  /* end switch */
    i++;
  } /* end while */

  /* Check required options */
  if ((g_aszDataInstallPaths[0][0] == 0) && (g_szBrokerInfoFile[0] == 0))
  {
    vplatform_printf("No install path or broker info file specified\n");
    return -1;
  } /* end if */

  if ((!bLanguage) && (!bVoice))
  {
    vplatform_printf("No language and/or voice specified\n");
    return -1;
  } /* end if */

  if (szTextFile[0] == 0)
  {
    vplatform_printf("No input text file specified\n");
    return -1;
  } /* end if */

  /* init pointers to install paths */
  for (i = 0; i < g_u16NbrOfDataInstallPaths; i++)
  {
    g_apDataInstallPaths[i] = g_aszDataInstallPaths[i];
  } /* end for */

  return 0;
} /* ParseInputArgsFromArgv */

#ifdef UNDER_CE

#define MAX_NR_ARGS 128

int ParseInputArgs(LPTSTR szCmdLine, TCHAR **argv)
{
    TCHAR *pszStr = szCmdLine, *pszNext = NULL, *pszArg;
    int i = 0;

    /* copy dummy into first argument */
    argv[0] =  _T("sample_text_nav");
    i++;

    while ((pszStr != NULL) && (*pszStr != 0))
    {
        pszNext = pszStr;
        if (pszNext == NULL) break;
        if (*pszNext == '"')
        {
            pszArg = pszNext+1;
            pszNext = _tcschr(pszArg, '"');
        }
        else
        {
            pszArg = pszNext;
            pszNext = _tcschr(pszArg, ' ');
        }
        if (pszNext != NULL)
        {
            *pszNext = 0;
        }

        if (i < MAX_NR_ARGS)
        {
            argv[i] = pszArg;
            i++;
        }
        else
        {
            return i;
        }

        if (pszNext != NULL)
        {
            pszStr = pszNext+1;
            while (*pszStr == ' ') pszStr++;
        }
        else
        {
            pszStr = NULL;
        }
    }
    return i;
}
#endif


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
    "  -R SpeechRate\n" \
    "  -V VolumeLevel\n" \
    "  -P PitchLevel\n" \
    "  -T CharType (1=UTF16 | 2=UTF8)\n" \
    "  -t TextFormat (1 = Plain Text | 2 = SSML)\n" \
    "  -M TextMode (1=Standard | 2=SMS)\n" \
    "  -o OutputFile\n" \
    "  -x \"<MimeType> <TuningDataFile>\"\n" \
    "  -j jumpPos nrOfFrames\n" \
    "  -e (extrainfo)\n" \
   );
  NUAN_APP_Print("Example:\n  ", name);
  vplatform_printf(" -I \"c:\\ve\" -v Samantha -i \"c:\\test.txt\" -o \"c:\\test.pcm\"\n");
  NUAN_APP_Print("  ", name);
  vplatform_printf(" -I \"c:\\ve\" -v Samantha -i \"c:\\test.txt\" -o \"c:\\test.pcm\" -j 1 23 -j 5 10 -j 3 0\n");
} /* Usage */


static void writeBeep(VPLATFORM_FILE_H outFile)
{
  NUAN_S16   beepArray[20]  = { 5096,  9648,  13282,  15616,  16422,  15616,  13282,  9648,  5096,  6,
                               -5069, -9648, -13282, -15616, -16422, -15616, -13282, -9648, -5096, -6, };
  NUAN_S16   silenceBlock[1000 * sizeof(NUAN_S16)];
  NUAN_S16   sineBlock[30 * sizeof(beepArray)];
  NUAN_U32   ctr                                      = 0;
  NUAN_U32   index                                    = 0;

  for (ctr = 0; ctr < (1000 * sizeof(NUAN_S16)); ctr++)
  {
    silenceBlock[ctr] = 0;
  } /* end for */
  for (ctr = 0; ctr < (30 * sizeof(beepArray)); ctr++)
  {
    sineBlock[ctr] = beepArray[index];
    index++;
    if (index>=20)
    {
      index = 0;
    } /* end if */
  } /* end for */

  vplatform_file_Write(silenceBlock, 1000, sizeof(NUAN_S16), outFile);
  vplatform_file_Write(sineBlock, 30, sizeof(beepArray), outFile);
  vplatform_file_Write(silenceBlock, 1000, sizeof(NUAN_S16), outFile);
} /* end writeBeep */


/*------------------------------------------------------------------*/
/*lint -esym(715, hTtsInst) not used */
/*lint -esym(715, UserData) not used */
static NUAN_ERROR CbOutNotifyToFile(
  VE_HINSTANCE           hTtsInst,
  void                 * pUserData,
  VE_CALLBACKMSG       * pcbMessage)
{
  VE_OUTDATA   *pTtsOutData      = NULL;
  VPLATFORM_FILE_H fPCM          = NULL;
  VE_OUTTAINFO *pTtsOutg_taInfo  = NULL;
  NUAN_U32         ctr              = 0;

  fPCM = (VPLATFORM_FILE_H)pUserData;

  switch (pcbMessage->eMessage)
  {
    case VE_MSG_BEGINPROCESS:
      vplatform_printf("VE_MSG_BEGINPROCESS - PCM generation\n");
      break;

    case VE_MSG_ENDPROCESS:
      vplatform_printf("\n");
      vplatform_printf("VE_MSG_ENDPROCESS --- PCM generation\n");
      break;

    case VE_MSG_OUTBUFDONE:
      if (bExtraInfo == NUAN_FALSE) vplatform_printf(".");
      g_outputBufferCounter++;
      if ( (g_simulateInterrupt == NUAN_TRUE) && (g_outputBufferCounter == g_interruptOnBuffer) )
      {
        vplatform_printf(" Interrupted by user!");
        return NUAN_E_TTS_USERSTOP;
      }
      else
      {
        pTtsOutData = (VE_OUTDATA *)pcbMessage->pParam;
        if ( (pTtsOutData->cntPcmBufLen != 0) && (fPCM != NULL) )
        {
          vplatform_file_Write(pTtsOutData->pOutPcmBuf, 1, pTtsOutData->cntPcmBufLen, fPCM);
        } /* end if */
        if (bExtraInfo == NUAN_TRUE)
        {
          for (ctr = 0; ctr < pTtsOutData->cntMrkListLen; ctr++)
          {
            if (pTtsOutData->pMrkList[ctr].eMrkType ==  VE_MRK_TEXTUNIT)
            {
              vplatform_printf("[TextUnit] %lu\tSrcPos=%lu SrcLen=%lu\n",
                 (unsigned long) pTtsOutData->pMrkList[ctr].cntDestPos,
                 (unsigned long) pTtsOutData->pMrkList[ctr].cntSrcPos + g_taInfo[g_taCurIndex].positionInText,
                 (unsigned long) pTtsOutData->pMrkList[ctr].cntSrcTextLen);
            }
            else if (pTtsOutData->pMrkList[ctr].eMrkType == VE_MRK_WORD)
            {
              vplatform_printf("[Word] %lu\tSrcPos=%lu SrcLen=%lu\n",
                 (unsigned long) pTtsOutData->pMrkList[ctr].cntDestPos,
                 (unsigned long) pTtsOutData->pMrkList[ctr].cntSrcPos + g_taInfo[g_taCurIndex].positionInText,
                 (unsigned long) pTtsOutData->pMrkList[ctr].cntSrcTextLen);
            }
            else if (pTtsOutData->pMrkList[ctr].eMrkType == VE_MRK_BOOKMARK)
            {
              vplatform_printf("[BookMark=%lu] %lu\tSrcPos=%lu SrcLen=%lu\n",
                 (unsigned long) pTtsOutData->pMrkList[ctr].ulMrkId,
                 (unsigned long) pTtsOutData->pMrkList[ctr].cntDestPos,
                 (unsigned long) pTtsOutData->pMrkList[ctr].cntSrcPos + g_taInfo[g_taCurIndex].positionInText,
                 (unsigned long) pTtsOutData->pMrkList[ctr].cntSrcTextLen);
            }
          } /* end for */
        } /* end if */
      } /* end if */
      break;

    case VE_MSG_OUTBUFREQ:
      pTtsOutData = (VE_OUTDATA *)pcbMessage->pParam;
      pTtsOutData->pOutPcmBuf  = pPcmData;
      pTtsOutData->cntPcmBufLen = PCM_BUF_SIZE;
      pTtsOutData->pMrkList     = pMrkData;
      pTtsOutData->cntMrkListLen = MRK_BUF_SIZE*sizeof(VE_MARKINFO);
      break;

    /* New messages for text navigation */
    case VE_MSG_TAIBEGIN:
      vplatform_printf("VE_MSG_TAIBEGIN --- Text Analysis\n");
      break;

    case VE_MSG_TAIEND:
      vplatform_printf("VE_MSG_TAIEND ----- Text Analysis\n");
      break;

    case VE_MSG_TAIBUFREQ:
      vplatform_printf("VE_MSG_TAIBUFREQ -- Text Analysis\n");
      pTtsOutg_taInfo = (VE_OUTTAINFO *)pcbMessage->pParam;
      pTtsOutg_taInfo->pRewrittenTextBuf = (char*)vplatform_heap_Calloc(hHeap, sizeof(char), (pTtsOutg_taInfo->cntRewrittenTextLen+1));
      pTtsOutg_taInfo->pTaInfoList       = (void*)vplatform_heap_Calloc(hHeap, sizeof(VE_TA_NODE), pTtsOutg_taInfo->cntTaInfoListLen);
      break;

    case VE_MSG_TAIBUFDONE:
      vplatform_printf("VE_MSG_TAIBUFDONE - Text Analysis\n");
      pTtsOutg_taInfo = (VE_OUTTAINFO *)pcbMessage->pParam;

      memmove(g_rewrittenTextBuf+g_textIndex, pTtsOutg_taInfo->pRewrittenTextBuf, pTtsOutg_taInfo->cntRewrittenTextLen);
      g_textIndex = g_textIndex + (unsigned long)pTtsOutg_taInfo->cntRewrittenTextLen;

      memmove(&g_taInfo[g_taIndex], pTtsOutg_taInfo->pTaInfoList, sizeof(VE_TA_NODE)*pTtsOutg_taInfo->cntTaInfoListLen);
      g_taIndex = g_taIndex + (unsigned long)pTtsOutg_taInfo->cntTaInfoListLen;

      for (ctr=0; ctr<pTtsOutg_taInfo->cntTaInfoListLen; ctr++)
      {
        VE_TA_NODE *aNode;

        aNode = (VE_TA_NODE*)pTtsOutg_taInfo->pTaInfoList;
        vplatform_printf("\n");
        vplatform_printf("VE_MSG_TAIBUFDONE: jmpPointType   is %d\n", (int)aNode[ctr].jmpPointType);
        vplatform_printf("VE_MSG_TAIBUFDONE: positionInText is %d\n", (int)aNode[ctr].positionInText);
        vplatform_printf("VE_MSG_TAIBUFDONE: languageIdent  is %s\n", (char*)aNode[ctr].languageIdent);
      } /* end for */

      if (pTtsOutg_taInfo->pRewrittenTextBuf != NULL)
      {
        vplatform_heap_Free(hHeap, pTtsOutg_taInfo->pRewrittenTextBuf);
        pTtsOutg_taInfo->pRewrittenTextBuf = NULL;
      } /* end if */

      if (pTtsOutg_taInfo->pTaInfoList != NULL)
      {
        vplatform_heap_Free(hHeap, pTtsOutg_taInfo->pTaInfoList);
        pTtsOutg_taInfo->pTaInfoList = NULL;
      } /* end if */
      break;
      /* End new message for text navigation */

    default:
      break;
  } /* end switch */

  return 0;
} /* CbOutNotifyToFile */

/*------------------------------------------------------------------*/
static NUAN_U32 ReadTextFromFile(
  const PLATFORM_TCHAR        *szName,
  void                       **ppText,
  NUAN_U32                    *pnTextLen)
{
    NUAN_U32 nTextLen = 0;
    NUAN_ERROR fRet = NUAN_OK;
    VPLATFORM_FILE_H hFile;

    *ppText = NULL;
    *pnTextLen = 0;

    fRet = vplatform_file_Open(hDataClass, hHeap, szName, _T("rb"), &hFile);
    if (fRet != NUAN_OK) return fRet;

    nTextLen = (NUAN_U32)vplatform_file_GetSize(hFile);

    *ppText = vplatform_heap_Calloc(hHeap, 1, nTextLen + 2);
    if (*ppText == NULL)
    {
        vplatform_file_Close(hFile);
        return NUAN_E_OUTOFMEMORY;
    }

    if (nTextLen > 0)
    {
        *pnTextLen = (NUAN_U32)vplatform_file_Read(*ppText, sizeof(char), nTextLen, hFile);
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
  int                    mainRc             = 0;
  NUAN_ERROR             nErrcode           = 0;
  VPLATFORM_FILE_H       fPCM               = NULL;  /* file pointer for output PCM file */
  VE_INSTALL             stInstall;
  VPLATFORM_RESOURCES    stResources;
  VE_HSPEECH             hSpeech;
  VE_HINSTANCE           hTtsInst;
  VE_OUTDEVINFO          stOutDevInfo;
  VE_INTEXT              stInText;
  VE_INTEXT              rewrittenInText;
  NUAN_U32               nTextLen           = 0;
  void                 * pText              = NULL;
  void                 * pBinBrokerInfo     = NULL;
  NUAN_U32               u32BrokerSize      = 0;
  NUAN_U32               iRes               = 0;
  VE_PRODUCT_VERSION     ttsProductVersion;
  int                    ctr                = 0;
#ifdef UNDER_CE
  int                    argc;
  TCHAR                 *argv[MAX_NR_ARGS];

  argc = ParseInputArgs(szCmdLine, argv);
#endif

  nErrcode = ve_ttsGetProductVersion(&ttsProductVersion);
  if (nErrcode == NUAN_OK)
  {
      vplatform_printf("\nVocalizer Expressive v%d.%d.%d\n\n", ttsProductVersion.major, ttsProductVersion.minor, ttsProductVersion.maint);
  } /* end if */

  /* Parse the command line arguments */
  if (ParseInputArgsFromArgv(argc, argv, 1))
  {
    Usage(argv[0]);
    return 1;
  } /* end if */

  vplatform_printf("Initialize\n");

  memset(&stInstall, 0, sizeof(stInstall));
  stInstall.fmtVersion = VE_CURRENT_VERSION;

  /* Get the platform dependent interfaces */
  memset(&stResources, 0, sizeof(stResources));
  stResources.fmtVersion          = VPLATFORM_CURRENT_VERSION;
  stResources.u16NbrOfDataInstall = g_u16NbrOfDataInstallPaths;
  stResources.apDataInstall       = g_apDataInstallPaths;

  nErrcode = vplatform_GetInterfaces(&stInstall, &stResources);
  if (nErrcode != NUAN_OK)
  {
    vplatform_printf("Error (vplatform_GetInterfaces) : 0x%lx\n", (unsigned long) nErrcode);
    mainRc = 1;
    goto exitMain;
  }

  /* init globals */
  hHeap = stInstall.hHeap;
  hDataClass = stInstall.hDataClass;

  /* read optional binary broker file */
  if (g_szBrokerInfoFile[0] != 0)
  {
    nErrcode = ReadTextFromFile(g_szBrokerInfoFile, &pBinBrokerInfo, &u32BrokerSize);
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
  if (szOutFile[0] != 0)
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
    vplatform_printf("Error (vauto_ttsInitialize) : 0x%lx\n", (unsigned long) nErrcode);
    mainRc = 1;
    goto exitRelease;
  } /* end if */

  /* Create the TTS instance */
  vplatform_printf("Create TTS instance\n");
  nErrcode = ve_ttsOpen(hSpeech, stInstall.hHeap, stInstall.hLog, &hTtsInst);
  if (nErrcode != NUAN_OK)
  {
    vplatform_printf("Error (ve_ttsOpen) : 0x%lx\n", (unsigned long) nErrcode);
    mainRc = 1;
    goto exitUnInit;
  } /* end if */

  /* Set parameters, these are initialized from the command line arguments */
  vplatform_printf("Set TTS parameters\n");
  nErrcode = ve_ttsSetParamList(hTtsInst , &TtsParam[0] , cTtsParam);
  if (nErrcode != NUAN_OK)
  {
    vplatform_printf("Error (ve_ttsSetParamList) : 0x%lx\n", (unsigned long) nErrcode);
    mainRc = 1;
    goto exitClose;
  } /* end if */

  /* Configure the system to load all modules at startup */
  TtsParam[0].eID            = VE_PARAM_INITMODE;
  TtsParam[0].uValue.usValue = VE_INITMODE_LOAD_ONCE_OPEN_ALL;
  nErrcode = ve_ttsSetParamList(hTtsInst , &TtsParam[0] , 1);
  if (nErrcode != NUAN_OK)
  {
    vplatform_printf("Error (ve_ttsSetParamList: VE_PARAM_INITMODE) : 0x%lx\n", (unsigned long) nErrcode);
    mainRc = 1;
    goto exitClose;
  } /* end if */

  /* Set the output device */
  memset(&stOutDevInfo, 0, sizeof(stOutDevInfo));
  stOutDevInfo.pUserData = (void *) fPCM;
  stOutDevInfo.pfOutNotify     = CbOutNotifyToFile;
  nErrcode = ve_ttsSetOutDevice(hTtsInst, &stOutDevInfo);
  if (nErrcode != NUAN_OK)
  {
    vplatform_printf("Error (ve_ttsSetOutDevice) : 0x%lx\n", (unsigned long) nErrcode);
    mainRc = 1;
    goto exitClose;
  } /* end if */

  for (iRes = 0; iRes < g_cTuningResources; iRes++)
  {
    /* Open the input resource file */
    NUAN_U32   nResLen = 0;

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
                                       g_tuningResourceMimeTypes[iRes],
                                       nResLen,
                                       g_pTuningResourceData[iRes],
                                       &g_hTuningResource[iRes]);
      if (nErrcode != NUAN_OK)
      {
        NUAN_APP_Print("Error (ve_ttsResourceLoad ", g_tuningResourceFiles[iRes]);
        vplatform_printf(") : 0x%lx\n", (unsigned long) nErrcode);
      } /* end if */
    } /* end if */
  } /* end for */

  /*
  * Do text to speech processing
  */
  vplatform_printf("Process\n");

  /* Specify the text to synthesize */
  stInText.eTextFormat  = VE_NORM_TEXT;
  stInText.cntTextLength = (NUAN_U32)nTextLen;
  stInText.szInText     = (void *)pText;
  stInText.eTextFormat  = eTextFormat; /* could be SSML */

  /* Text analysis */
  g_rewrittenTextBuf = (char*)vplatform_heap_Calloc(hHeap, 1, 1000000);  /* Will contain the rewritten text (in UTF16) generated by ve_ttsAnalyzeText() */
  if (g_rewrittenTextBuf == NULL)
  {
    vplatform_printf("Failed to allocate memory\n");
    goto exitClose;
  }
  g_textIndex        = 0;
  g_taIndex          = 0;
  nErrcode = ve_ttsAnalyzeText(hTtsInst, &stInText);
  if (nErrcode != NUAN_OK)
  {
    vplatform_printf("Error (ve_ttsAnalyzeText) : 0x%lx\n", (unsigned long) nErrcode);
  } /* end if */

  vplatform_printf("\nNumber of jump points is : %lu\n\n", g_taIndex);

  rewrittenInText.eTextFormat  = stInText.eTextFormat;
  rewrittenInText.cntTextLength = (NUAN_U32)(g_textIndex);
  rewrittenInText.szInText     = (void *)g_rewrittenTextBuf;

  /* Overview */
  vplatform_printf("Overview of jump to points entered on the command line:\n");
  vplatform_printf("-------------------------------------------------------\n");
  vplatform_printf("Number of jumps to execute: %d \n", (int)g_nrOfJumpToPointsOnCommandLine);
  for (ctr = 0; ctr < (int)g_nrOfJumpToPointsOnCommandLine; ctr++)
  {
    NUAN_U32 index = 0;
    if (g_jumpToArray[ctr].jumpToPos < g_taIndex)
    {

      vplatform_printf("%d : Jump to position: %d, after %d frames : \"", ctr, \
                                                                 (int)g_jumpToArray[ctr].jumpToPos, \
                                                                 (int)g_jumpToArray[ctr].interruptAfterNrOfFrames);

      while ((g_rewrittenTextBuf[(g_taInfo[g_jumpToArray[ctr].jumpToPos].positionInText) + index] != 0) && (index < 20))
      {
        if (    (g_rewrittenTextBuf[(g_taInfo[g_jumpToArray[ctr].jumpToPos].positionInText) + index] < 0x7F)
             && (g_rewrittenTextBuf[(g_taInfo[g_jumpToArray[ctr].jumpToPos].positionInText) + index] > 0x20) )
        {
          vplatform_printf("%c", g_rewrittenTextBuf[(g_taInfo[g_jumpToArray[ctr].jumpToPos].positionInText) + index]);
        }
        else
        {
          vplatform_printf(" ");
        } /* end if */
        index++;
      } /* end while */
      vplatform_printf(" ...\"\n");
    }
    else
    {
      vplatform_printf("%d : Jump position %d out of range, only %lu positions found\n", (int)ctr, (int)g_jumpToArray[ctr].jumpToPos, g_taIndex);
    }
  } /* end for */

  /* Synthesis */
  if (g_nrOfJumpToPointsOnCommandLine == 0)
  {
    g_simulateInterrupt   = NUAN_FALSE;
    g_outputBufferCounter = 0;
    g_interruptOnBuffer   = 0;
    g_taCurIndex          = 0;
    vplatform_printf("\nStarting from jump point 0 till the end\n");
    nErrcode = ve_ttsProcessText2SpeechStartingAt(hTtsInst, &rewrittenInText, g_taInfo, 0);
    if (nErrcode != NUAN_OK)
    {
      vplatform_printf("Error (ve_ttsProcessText2SpeechStartingAt) : 0x%lx\n", (unsigned long) nErrcode);
    } /* end if */
  }
  else
  {
    for (ctr = 0; ctr < (int)g_nrOfJumpToPointsOnCommandLine; ctr++)
    {
      if (g_jumpToArray[ctr].jumpToPos < g_taIndex)
      {
        g_simulateInterrupt   = NUAN_TRUE;
        g_outputBufferCounter = 0;
        g_interruptOnBuffer   = (NUAN_U32)g_jumpToArray[ctr].interruptAfterNrOfFrames;
        g_taCurIndex = g_jumpToArray[ctr].jumpToPos;
        vplatform_printf("\nJumping to jump point %d\n", (int)g_jumpToArray[ctr].jumpToPos);
        if (ctr != 0)
        {
          writeBeep(fPCM);
        } /* end if */
        nErrcode = ve_ttsProcessText2SpeechStartingAt(hTtsInst, &rewrittenInText, g_taInfo, g_jumpToArray[ctr].jumpToPos);
        if (g_interruptOnBuffer != 0)
        {
          if ((nErrcode != NUAN_E_TTS_USERSTOP) && (nErrcode != NUAN_OK))
          {
            vplatform_printf("Error (ve_ttsProcessText2SpeechStartingAt) : 0x%lx\n", (unsigned long) nErrcode);
          } /* end if */
        }
        else
        {
          if (nErrcode != NUAN_OK)
          {
            vplatform_printf("Error (ve_ttsProcessText2SpeechStartingAt) : 0x%lx\n", (unsigned long) nErrcode);
          } /* end if */
        } /* end if */
      } /* end if */
    } /* end for */
  } /* end if */


  for (iRes = 0; iRes < g_cTuningResources; iRes++)
  {
    nErrcode = ve_ttsResourceUnload(hTtsInst, g_hTuningResource[iRes]);
    if (nErrcode != NUAN_OK)
    {
      NUAN_APP_Print("Error (ve_ttsResourceUnload ", g_tuningResourceFiles[iRes]);
      vplatform_printf(") : 0x%lx\n", (unsigned long) nErrcode);
    } /* end if */

    /* ve_ttsResourceLoad( ) requires that we keep the data
    ** around until we either unload it or close the instance */
    if (g_pTuningResourceData[iRes])
    {
      vplatform_heap_Free(hHeap, g_pTuningResourceData[iRes]);
    } /* end if */
  } /* end for */

exitClose:
  /* Close the TTS instance */
  vplatform_printf("Close TTS instance\n");
  nErrcode = ve_ttsClose(hTtsInst);
  if (nErrcode != NUAN_OK)
  {
    vplatform_printf("Error (ve_ttsClose) : 0x%lx\n", (unsigned long) nErrcode);
  } /* end if */

exitUnInit:
  /* Uninitialize */
  vplatform_printf("Uninitialize\n");
  nErrcode = ve_ttsUnInitialize(hSpeech);
  if (nErrcode != NUAN_OK)
  {
    vplatform_printf("Error (ve_ttsUnInitialize) : 0x%lx\n", (unsigned long) nErrcode);
  } /* end if */

exitRelease:
  /* Clean up */
  if (pText != NULL) vplatform_heap_Free(hHeap, pText);
  if (pPcmData != NULL) vplatform_heap_Free(hHeap, pPcmData);
  if (pMrkData != NULL) vplatform_heap_Free(hHeap, pMrkData);
  if (g_rewrittenTextBuf != NULL) vplatform_heap_Free(hHeap, g_rewrittenTextBuf);
  if (fPCM != NULL) vplatform_file_Close(fPCM);

  /* release platform interfaces */
  vplatform_ReleaseInterfaces(&stInstall);

exitMain:

  return mainRc;
} /* main */

/* ******************************************************************
**  END
** ******************************************************************/
