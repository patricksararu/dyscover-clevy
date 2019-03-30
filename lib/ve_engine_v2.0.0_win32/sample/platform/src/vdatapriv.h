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

#ifndef __VDATAPRIV_H__
#define __VDATAPRIV_H__

/* ******************************************************************
**  DEFINITIONS
** ******************************************************************/

#if defined( __cplusplus )
extern "C"
{
#endif

/* Class structure for the data interface */
typedef struct VPLATFORM_DATA_CLASS_S {
  void *   hHeap;
  void                     * pPlatformInfo;
  char                     * szBrokerInfo;
  VPLATFORM_FILELIST       * pFileList;
  NUAN_BOOL                  bErrorCheck;
} VPLATFORM_DATA_CLASS_T;

#if defined( __cplusplus )
}
#endif

/* ******************************************************************
**  END
** ******************************************************************/

#endif /* __VDATAPRIV_H__ */
