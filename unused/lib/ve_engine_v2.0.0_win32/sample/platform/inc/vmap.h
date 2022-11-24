#ifndef __VMAP_H__
#define __VMAP_H__

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

/*------------------------------------------------------------------*/
NUAN_ERROR vplatform_datamapping_Open(
  void *         hDataClass,
  void *     hHeap,
  const char                 * szName,
  void *     * phMapping
);
/*------------------------------------------------------------------*/
NUAN_ERROR vplatform_datamapping_Close(
  void *  hMapping
);
/*------------------------------------------------------------------*/
NUAN_ERROR vplatform_datamapping_Map(
  void *     hMapping,
  size_t                     cOffset,
  size_t                 * pcBytes,
  const void              ** ppData
);
/*------------------------------------------------------------------*/
NUAN_ERROR vplatform_datamapping_Unmap(
  void *     hMapping,
  const void               * pData
);
/*------------------------------------------------------------------*/
NUAN_ERROR vplatform_datamapping_Freeze(
  void *  hMapping
);

#if defined( __cplusplus )
}
#endif

/* ******************************************************************
**  END
** ******************************************************************/

#endif /* #ifndef __VMAP_H__ */
