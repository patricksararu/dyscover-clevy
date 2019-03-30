#ifndef __VPLATFORM_TCHAR_H__
#define __VPLATFORM_TCHAR_H__

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

/* ******************************************************************
**  DEFINITIONS
** ******************************************************************/

#ifdef PLATFORM_UNICODE

//#define UNICODE
//#define _UNICODE

#define _tprintf wprintf
#define _tcslen  wcslen
#define _tcscpy  wcscpy
#define _tcscat  wcscat
#define _tcsstr  wcsstr

#if defined(WIN32)||defined(UNDER_CE)
#define _tfopen  _wfopen
#else
#define _tfopen  wfopen
#endif

#define _tcscmp  wcscmp
#define _tcsicmp _wcsicmp
#define _tcschr  wcschr
#define _stprintf swprintf
#define _vstprintf vswprintf
#define _ttoi    _wtoi
#define _T(x)     L ## x

#else

#define _tprintf printf
#define _tcslen  strlen
#define _tcscpy  strcpy
#define _tcscat  strcat
#define _tcsstr  strstr
#define _tfopen  fopen
#define _tcscmp  strcmp
#if defined(LINSO) || defined(__SYMBIAN32__)
#define _tcsicmp strcasecmp
#else
#define _tcsicmp _stricmp
#endif
#define _tcschr  strchr
#define _stprintf sprintf
#define _vstprintf vsprintf
#define _ttoi    atoi
#define _T(x)    x

#endif

/* ******************************************************************
**  END
** ******************************************************************/

#endif /* #ifndef __VPLATFORM_TCHAR_H__ */
