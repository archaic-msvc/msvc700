/***
*nlsint.h - national language support internal defintions
*
*       Copyright (c) 1991-2001, Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Contains internal definitions/declarations for international functions,
*       shared between run-time and math libraries, in particular,
*       the localized decimal point.
*
*       [Internal]
*
****/

#if _MSC_VER > 1000
#pragma once
#endif  /* _MSC_VER > 1000 */

#ifndef _INC_NLSINT
#define _INC_NLSINT

#ifndef _CRTBLD
/*
 * This is an internal C runtime header file. It is used when building
 * the C runtimes only. It is not to be used as a public header file.
 */
#error ERROR: Use of C runtime library internal header file.
#endif  /* _CRTBLD */

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#if !defined (_W64)
#if !defined(__midl) && (defined(_X86_) || defined(_M_IX86)) && _MSC_VER >= 1300
#define _W64 __w64
#else  /* !defined(__midl) && (defined(_X86_) || defined(_M_IX86)) && _MSC_VER >= 1300 */
#define _W64
#endif  /* !defined(__midl) && (defined(_X86_) || defined(_M_IX86)) && _MSC_VER >= 1300 */
#endif  /* !defined (_W64) */

#ifndef _SIZE_T_DEFINED
#ifdef _WIN64
typedef unsigned __int64    size_t;
#else  /* _WIN64 */
typedef _W64 unsigned int   size_t;
#endif  /* _WIN64 */
#define _SIZE_T_DEFINED
#endif  /* _SIZE_T_DEFINED */

/*
 *  Definitions for a localized decimal point.
 *  Currently, run-times only support a single character decimal point.
 */
#define ___decimal_point                __decimal_point
extern char __decimal_point[];          /* localized decimal point string */

#define ___decimal_point_length         __decimal_point_length
extern size_t __decimal_point_length;   /* not including terminating null */

#define _ISDECIMAL(p)   (*(p) == *___decimal_point)
#define _PUTDECIMAL(p)  (*(p)++ = *___decimal_point)
#define _PREPUTDECIMAL(p)       (*(++p) = *___decimal_point)

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif  /* _INC_NLSINT */
