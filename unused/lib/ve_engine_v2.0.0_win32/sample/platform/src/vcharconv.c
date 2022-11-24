/* **********************************************************************
**  Nuance Communications, Inc.
** **********************************************************************/

/* **********************************************************************
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
**  (c) Copyright 2014 Nuance Communications, Inc.
**  All rights reserved. Company confidential.
**  
** **********************************************************************/

/* **********************************************************************
**  HEADER (INCLUDE) SECTION
** **********************************************************************/
#include <windows.h>
#include <string.h>
#include "vplatform.h"
#include "vheap.h"
#include "vcharconv.h"


/* **********************************************************************
**  COMPILER DIRECTIVES
** **********************************************************************/

/* **********************************************************************
**  DEFINITIONS
** **********************************************************************/

/* **********************************************************************
**  LOCAL FUNCTIONS
** **********************************************************************/

/* **********************************************************************
**  GLOBAL FUNCTIONS
**
**  Conversion from UTF-8 to PLATFORM_TCHAR 
**
**  PLATFORM_UNICODE defined: PLATFORM_TCHAR = wchar_t (UTF-16)
**  PLATFORM_UNICODE not defined: PLATFORM_TCHAR = char (ANSI code page)
** **********************************************************************/

/*----------------------------------------------------------------------*/
NUAN_ERROR vplatform_UTF8_to_PLATFORM_TCHAR(
    PLATFORM_TCHAR             **pDst,
    const char                  *pSrc,
    void *     hHeap)
{
    int len;
#if !defined(PLATFORM_UNICODE)
    wchar_t * pTmp;
#endif
    
    /* determine needed length */
    len = MultiByteToWideChar(CP_UTF8, 0, pSrc, -1, NULL, 0);    
    if (len == 0) return NUAN_E_INVALIDARG;
#if !defined(PLATFORM_UNICODE)
    /* there's no direct conversion from UTF-8 to the ANSI code page, work around by converting to UTF-16 first */
    pTmp = (wchar_t *)vplatform_heap_Calloc(hHeap, (NUAN_U32)len, sizeof(wchar_t));
    if (pTmp == NULL) return NUAN_E_OUTOFMEMORY;
    MultiByteToWideChar(CP_UTF8, 0, pSrc, -1, pTmp, len);
    /* now find length to convert to ANSI */
    len = WideCharToMultiByte(CP_ACP, 0, pTmp, -1, NULL, 0, NULL, NULL);
    if (len == 0)
    {
        vplatform_heap_Free(hHeap, pTmp);
        return NUAN_E_INVALIDARG;
    }
#endif
    
    /* allocate memory */
    *pDst = vplatform_heap_Calloc(hHeap, (NUAN_U32)len, sizeof(PLATFORM_TCHAR));
    if (*pDst == NULL) 
    {
#if !defined(PLATFORM_UNICODE)
        vplatform_heap_Free(hHeap, pTmp);
#endif
        return NUAN_E_OUTOFMEMORY;
    }

    /* Do the conversion */
#ifdef PLATFORM_UNICODE
    MultiByteToWideChar(CP_UTF8, 0, pSrc, -1, *pDst, len);
#else
    WideCharToMultiByte(CP_ACP, 0, pTmp, -1, *pDst, len, NULL, NULL);
    vplatform_heap_Free(hHeap, pTmp);
#endif
    
    return NUAN_OK;
}

/*----------------------------------------------------------------------*/
NUAN_ERROR vplatform_PLATFORM_TCHAR_to_UTF8(
    char                       **pDst,
    const PLATFORM_TCHAR        *pSrc,
    void *     hHeap)
{
    int len;
#if !defined(PLATFORM_UNICODE)
    wchar_t * pTmp;
#endif
    
    /* determine needed length */
#if defined(PLATFORM_UNICODE)
    len = WideCharToMultiByte(CP_UTF8, 0, pSrc, -1, NULL, 0, NULL, NULL);    
    if (len == 0) return NUAN_E_INVALIDARG;
#else
    /* there's no direct conversion from the ANSI code page to UTF-8, work around by converting to UTF-16 first */
    len = MultiByteToWideChar(CP_ACP, 0, pSrc, -1, NULL, 0);    
    if (len == 0) return NUAN_E_INVALIDARG;
    
    pTmp = (wchar_t *)vplatform_heap_Calloc(hHeap, (NUAN_U32)len, sizeof(wchar_t));
    if (pTmp == NULL) return NUAN_E_OUTOFMEMORY;
    
    MultiByteToWideChar(CP_ACP, 0, pSrc, -1, pTmp, len);

    /* now find length to convert to UTF-8 */
    len = WideCharToMultiByte(CP_UTF8, 0, pTmp, -1, NULL, 0, NULL, NULL);
    if (len == 0)
    {
        vplatform_heap_Free(hHeap, pTmp);
        return NUAN_E_INVALIDARG;
    }
#endif
    
    /* allocate memory */
    *pDst = vplatform_heap_Calloc(hHeap, (NUAN_U32)len, sizeof(PLATFORM_TCHAR));
    if (*pDst == NULL)
    {
#if !defined(PLATFORM_UNICODE)
        vplatform_heap_Free(hHeap, pTmp);
#endif
        return NUAN_E_OUTOFMEMORY;
    }

    /* Do the conversion */
#ifdef PLATFORM_UNICODE
    WideCharToMultiByte(CP_UTF8, 0, pSrc, -1, *pDst, len, NULL, NULL);
#else
    WideCharToMultiByte(CP_UTF8, 0, pTmp, -1, *pDst, len, NULL, NULL);
    vplatform_heap_Free(hHeap, pTmp);
#endif
    
    return NUAN_OK;
}
