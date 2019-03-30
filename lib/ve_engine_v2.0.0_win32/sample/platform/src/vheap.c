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
#include "vheap.h"

#include <stdlib.h>

/* ******************************************************************
**  COMPILER DIRECTIVES
** ******************************************************************/


/* ******************************************************************
**  DEFINITIONS
** ******************************************************************/

/*lint -esym(715, hHeap, pResources) 
**           not used in this implementation */


/* ------------------------------------------------------------------
**  Heap service
** -----------------------------------------------------------------*/

/*------------------------------------------------------------------*/
void* vplatform_heap_Malloc(
  void *   hHeap,
  size_t                     cBytes)
{
  return malloc(cBytes);
} /* vplatform_heap_Malloc */

/*------------------------------------------------------------------*/
void* vplatform_heap_Calloc(
  void *   hHeap,
  size_t                     cElements,
  size_t                     cElementBytes)
{
  return calloc(cElements, cElementBytes);
} /* vplatform_heap_Calloc */

/*------------------------------------------------------------------*/
void* vplatform_heap_Realloc(
  void *     hHeap,
  void                       * pData,
  size_t                       cBytes)
{
  return realloc(pData, cBytes);
} /* vplatform_heap_Realloc */

/*------------------------------------------------------------------*/
void vplatform_heap_Free(
  void *     hHeap,
  void                       * pData)
{
  free(pData);
} /* vplatform_heap_Free */

/*===================================================================
**  Definition of static interfaces
**==================================================================*/

static const VE_HEAP_INTERFACE     IHeap = {
  vplatform_heap_Malloc,
  vplatform_heap_Calloc,
  vplatform_heap_Realloc,
  vplatform_heap_Free
};

/* ******************************************************************
**  PUBLIC INTERFACE RETRIEVAL FUNCTIONS
** ******************************************************************/

/*------------------------------------------------------------------*/
NUAN_ERROR vplatform_heap_GetInterface(
    VE_INSTALL       * pInstall,
    VPLATFORM_RESOURCES * pResources)
{
  pInstall->pIHeap = &IHeap;
  pInstall->hHeap = NULL;
  return NUAN_OK;
} /* vplatform_heap_GetInterface */

/*------------------------------------------------------------------*/
NUAN_ERROR vplatform_heap_ReleaseInterface(
  void *               hHeap)
{
  (void) hHeap;
  return NUAN_OK;
} /* vplatform_heap_ReleaseInterface */


/* ******************************************************************
**  END
** ******************************************************************/
