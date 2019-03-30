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

#include "vplatform.h"
#include "vplatform_tchar.h"
#include "vheap.h"
#include "vdata.h"
#include "vfile.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

/* ******************************************************************
**  COMPILER DIRECTIVES
** ******************************************************************/


/* ******************************************************************
**  DEFINITIONS
** ******************************************************************/

/*lint -esym(715, hHeap, hDataClass) 
**           not used in this implementation */

/* ******************************************************************
**  LOCAL FUNCTIONS
** ******************************************************************/

/* ------------------------------------------------------------------
**  Data access services
**
**  Simple implementation : stream handle == file handle
** -----------------------------------------------------------------*/


/*------------------------------------------------------------------*/
NUAN_ERROR vplatform_datastream_Open(
  void *         hDataClass,
  void *     hHeap,
  const char                 * szName,
  const char                 * szMode,
  void *      * phStream)
{
  NUAN_ERROR fRet = NUAN_OK;

  *phStream = NULL;

  if ((! szName) || (! szMode) || (! hDataClass))
  {
    fRet = NUAN_E_INVALIDARG;
  }
  else if ((szMode[0] != 'r') || (strchr(szName, '.') != NULL))
  {
    /* Full file path in UTF-8 (broker strings never have a "." in them) or
    ** output file */
    fRet = vplatform_file_OpenUTF8(hDataClass, hHeap, szName, szMode, (VPLATFORM_FILE_H *)phStream);
  }
  else
  {
    /* This is a Vocalizer broker string for a data component.
    ** To simplify installation and experimentation, the Vocalizer SDK
    ** has a separate file for each data component, where the filename
    ** basically is the broker string with '/' converted to '_', and
    ** ".dat" appended.
    ** It also has separate directories for each language.
    ** For deployment you can change this as you see fit (such as
    ** combining them into one or a few files, combining them into a
    ** single directory, changing the naming conventions, etc.). 
    */
    PLATFORM_TCHAR *szFullPathName = NULL;
    
    /* ask the data class for the full path name of the data file */
    fRet = vplatform_data_GetFullPathName(szName, &szFullPathName, hDataClass);
    if (fRet == NUAN_OK)
    {
        fRet = vplatform_file_Open(hDataClass, hHeap, szFullPathName, _T("rb"), (VPLATFORM_FILE_H *)phStream);
    }
 
    vplatform_data_FreeFullPathName(szFullPathName, hDataClass);
  }
  
  return fRet;
} /* vplatform_datastream_Open */

/*------------------------------------------------------------------*/
NUAN_ERROR vplatform_datastream_Close(
  void *   hStream)
{
  return vplatform_file_Close((VPLATFORM_FILE_H)hStream);
} /* vplatform_datastream_Close */

/*------------------------------------------------------------------*/
size_t vplatform_datastream_Read(
  void                    * pBuffer,
  size_t                    cElementBytes,
  size_t                    cElements,
  void *     hStream)
{
  return vplatform_file_Read(pBuffer, cElementBytes, cElements, (VPLATFORM_FILE_H)hStream);
} /* vplatform_datastream_Read */

/*------------------------------------------------------------------*/
NUAN_ERROR vplatform_datastream_Seek(
  void *           hStream,
  size_t                          cOffset,
  VE_STREAM_ORIGIN             eOrigin,
  VE_STREAM_DIRECTION          eDirection)
{
  return vplatform_file_Seek((VPLATFORM_FILE_H)hStream, cOffset, eOrigin, eDirection);
} /* vplatform_datastream_Seek */

/*------------------------------------------------------------------*/
size_t vplatform_datastream_GetSize(
  void *   hStream)
{
  return vplatform_file_GetSize((VPLATFORM_FILE_H)hStream);
} /* vplatform_datastream_GetSize */

/*------------------------------------------------------------------*/
NUAN_ERROR vplatform_datastream_Error(
  void *   hStream)
{
  return vplatform_file_Error((VPLATFORM_FILE_H)hStream);
} /* vplatform_datastream_Error */

/*------------------------------------------------------------------*/
size_t vplatform_datastream_Write(
  const void              * pBuffer,
  size_t                    cElementBytes,
  size_t                    cElements,
  void *     hStream)
{
  return vplatform_file_Write(pBuffer, cElementBytes, cElements, (VPLATFORM_FILE_H)hStream);
} /* vplatform_datastream_Write */


/* ******************************************************************
**  END
** ******************************************************************/
