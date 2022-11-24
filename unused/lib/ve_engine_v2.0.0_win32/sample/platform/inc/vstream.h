#ifndef __VSTREAM_H__
#define __VSTREAM_H__

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

#if defined( __cplusplus )
extern "C"
{
#endif

/* ******************************************************************
**  GLOBAL FUNCTION PROTOTYPES
** ******************************************************************/

/* ------------------------------------------------------------------
**  Data access services
** -----------------------------------------------------------------*/


/* ------------------------------------------------------------------
** interface functions                                                 
** -----------------------------------------------------------------*/

/*------------------------------------------------------------------*/
NUAN_ERROR vplatform_datastream_Open(
  void *         hDataClass,
  void *     hHeap,
  const char                 * szName,
  const char                 * szMode,
  void *      * phStream
);
/*------------------------------------------------------------------*/
NUAN_ERROR vplatform_datastream_Close(
  void *   hStream
);
/*------------------------------------------------------------------*/
size_t vplatform_datastream_Read(
  void                    * pBuffer,
  size_t                    cElementBytes,
  size_t                    cElements,
  void *     hStream
);
/*------------------------------------------------------------------*/
NUAN_ERROR vplatform_datastream_Seek(
  void *           hStream,
  size_t                          cOffset,
  VE_STREAM_ORIGIN             eOrigin,
  VE_STREAM_DIRECTION          eDirection
);
/*------------------------------------------------------------------*/
size_t vplatform_datastream_GetSize(
  void *   hStream
);
/*------------------------------------------------------------------*/
NUAN_ERROR vplatform_datastream_Error(
  void *   hStream
);
/*------------------------------------------------------------------*/
size_t vplatform_datastream_Write(
  const void              * pBuffer,
  size_t                    cElementBytes,
  size_t                    cElements,
  void *     hStream
);

#if defined( __cplusplus )
}
#endif

/* ******************************************************************
**  END
** ******************************************************************/

#endif /* #ifndef __VSTREAM_H__ */
