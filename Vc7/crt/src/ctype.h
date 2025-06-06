/***
*ctype.h - character conversion macros and ctype macros
*
*       Copyright (c) 1985-2001, Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Defines macros for character classification/conversion.
*       [ANSI/System V]
*
*       [Public]
*
****/

#if _MSC_VER > 1000
#pragma once
#endif  /* _MSC_VER > 1000 */

#ifndef _INC_CTYPE
#define _INC_CTYPE

#if !defined (_WIN32)
#error ERROR: Only Win32 target supported!
#endif  /* !defined (_WIN32) */

#ifndef _CRTBLD
/* This version of the header files is NOT for user programs.
 * It is intended for use when building the C runtimes ONLY.
 * The version intended for public use will not have this message.
 */
#error ERROR: Use of C runtime library internal header file.
#endif  /* _CRTBLD */

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#ifndef _INTERNAL_IFSTRIP_
#include <cruntime.h>
#endif  /* _INTERNAL_IFSTRIP_ */


/* Define _CRTIMP */

#ifndef _CRTIMP
#ifdef CRTDLL
#define _CRTIMP __declspec(dllexport)
#else  /* CRTDLL */
#ifdef _DLL
#define _CRTIMP __declspec(dllimport)
#else  /* _DLL */
#define _CRTIMP
#endif  /* _DLL */
#endif  /* CRTDLL */
#endif  /* _CRTIMP */


/* Define __cdecl for non-Microsoft compilers */

#if (!defined (_MSC_VER) && !defined (__cdecl))
#define __cdecl
#endif  /* (!defined (_MSC_VER) && !defined (__cdecl)) */

#ifndef _WCHAR_T_DEFINED
typedef unsigned short wchar_t;
#define _WCHAR_T_DEFINED
#endif  /* _WCHAR_T_DEFINED */

#ifndef _WCTYPE_T_DEFINED
typedef unsigned short wint_t;
typedef unsigned short wctype_t;
#define _WCTYPE_T_DEFINED
#endif  /* _WCTYPE_T_DEFINED */

#ifndef WEOF
#define WEOF (wint_t)(0xFFFF)
#endif  /* WEOF */

#ifndef _CTYPE_DISABLE_MACROS
_CRTIMP extern const unsigned short _ctype[];
_CRTIMP extern const unsigned short _wctype[];
#ifndef _INTERNAL_IFSTRIP_
_CRTIMP const unsigned short * __cdecl __pctype_func(void);
_CRTIMP const wctype_t * __cdecl __pwctype_func(void);
#if defined (_DLL) && defined (_M_IX86)
/* Retained for compatibility with VC++ 5.0 and earlier versions */
_CRTIMP const unsigned short ** __cdecl __p__pctype(void);
_CRTIMP const wctype_t ** __cdecl __p__pwctype(void);
#endif  /* defined (_DLL) && defined (_M_IX86) */
#endif  /* _INTERNAL_IFSTRIP_ */
_CRTIMP extern const unsigned short *_pctype;
_CRTIMP extern const wctype_t *_pwctype;
#endif  /* _CTYPE_DISABLE_MACROS */


#ifndef _INTERNAL_IFSTRIP_
#ifdef _MT
struct  threadlocaleinfostruct;
typedef struct threadlocaleinfostruct * pthreadlocinfo;
extern pthreadlocinfo __ptlocinfo;
pthreadlocinfo __cdecl __updatetlocinfo(void);
#endif  /* _MT */
#endif  /* _INTERNAL_IFSTRIP_ */


/* set bit masks for the possible character types */

#define _UPPER          0x1     /* upper case letter */
#define _LOWER          0x2     /* lower case letter */
#define _DIGIT          0x4     /* digit[0-9] */
#define _SPACE          0x8     /* tab, carriage return, newline, */
                                /* vertical tab or form feed */
#define _PUNCT          0x10    /* punctuation character */
#define _CONTROL        0x20    /* control character */
#define _BLANK          0x40    /* space char */
#define _HEX            0x80    /* hexadecimal digit */

#define _LEADBYTE       0x8000                  /* multibyte leadbyte */
#define _ALPHA          (0x0100|_UPPER|_LOWER)  /* alphabetic character */


/* character classification function prototypes */

#ifndef _CTYPE_DEFINED

_CRTIMP int __cdecl _isctype(int, int);
_CRTIMP int __cdecl isalpha(int);
_CRTIMP int __cdecl isupper(int);
_CRTIMP int __cdecl islower(int);
_CRTIMP int __cdecl isdigit(int);
_CRTIMP int __cdecl isxdigit(int);
_CRTIMP int __cdecl isspace(int);
_CRTIMP int __cdecl ispunct(int);
_CRTIMP int __cdecl isalnum(int);
_CRTIMP int __cdecl isprint(int);
_CRTIMP int __cdecl isgraph(int);
_CRTIMP int __cdecl iscntrl(int);
_CRTIMP int __cdecl toupper(int);
_CRTIMP int __cdecl tolower(int);
_CRTIMP int __cdecl _tolower(int);
_CRTIMP int __cdecl _toupper(int);
_CRTIMP int __cdecl __isascii(int);
_CRTIMP int __cdecl __toascii(int);
_CRTIMP int __cdecl __iscsymf(int);
_CRTIMP int __cdecl __iscsym(int);
#define _CTYPE_DEFINED
#endif  /* _CTYPE_DEFINED */

#ifndef _WCTYPE_DEFINED

/* wide function prototypes, also declared in wchar.h  */

/* character classification function prototypes */

_CRTIMP int __cdecl iswalpha(wint_t);
_CRTIMP int __cdecl iswupper(wint_t);
_CRTIMP int __cdecl iswlower(wint_t);
_CRTIMP int __cdecl iswdigit(wint_t);
_CRTIMP int __cdecl iswxdigit(wint_t);
_CRTIMP int __cdecl iswspace(wint_t);
_CRTIMP int __cdecl iswpunct(wint_t);
_CRTIMP int __cdecl iswalnum(wint_t);
_CRTIMP int __cdecl iswprint(wint_t);
_CRTIMP int __cdecl iswgraph(wint_t);
_CRTIMP int __cdecl iswcntrl(wint_t);
_CRTIMP int __cdecl iswascii(wint_t);
_CRTIMP int __cdecl isleadbyte(int);

_CRTIMP wchar_t __cdecl towupper(wchar_t);
_CRTIMP wchar_t __cdecl towlower(wchar_t);

_CRTIMP int __cdecl iswctype(wint_t, wctype_t);

/* --------- The following functions are OBSOLETE --------- */
_CRTIMP int __cdecl is_wctype(wint_t, wctype_t);
/*  --------- The preceding functions are OBSOLETE --------- */

#define _WCTYPE_DEFINED
#endif  /* _WCTYPE_DEFINED */

/* the character classification macro definitions */

#ifndef _CTYPE_DISABLE_MACROS

/*
 * Maximum number of bytes in multi-byte character in the current locale
 * (also defined in stdlib.h).
 */
#ifndef MB_CUR_MAX
#ifndef _INTERNAL_IFSTRIP_
#if defined (_DLL) && defined (_M_IX86)
/* Retained for compatibility with VC++ 5.0 and earlier versions */
_CRTIMP int * __cdecl __p___mb_cur_max(void);
#endif  /* defined (_DLL) && defined (_M_IX86) */
#endif  /* _INTERNAL_IFSTRIP_ */
#define MB_CUR_MAX __mb_cur_max
_CRTIMP extern int __mb_cur_max;
/* These functions are for enabling STATIC_CPPLIB functionality */
_CRTIMP int __cdecl ___mb_cur_max_func(void);
#endif  /* MB_CUR_MAX */

/* Introduced to detect error when character testing functions are called
 * with illegal input of integer.
 */
#ifdef _DEBUG
_CRTIMP int __cdecl _chvalidator(int, int);
#define __chvalidchk(a,b)       _chvalidator(a,b)
#else  /* _DEBUG */
#define __chvalidchk(a,b)       (_pctype[a] & (b))
#endif  /* _DEBUG */


#ifndef _INTERNAL_IFSTRIP_
#define __ascii_isalpha(c)      ( __chvalidchk(c, _ALPHA))
#define __ascii_isdigit(c)      ( __chvalidchk(c, _DIGIT))
#define __ascii_tolower(c)      ( (((c) >= 'A') && ((c) <= 'Z')) ? ((c) - 'A' + 'a') : (c) )
#define __ascii_toupper(c)      ( (((c) >= 'a') && ((c) <= 'z')) ? ((c) - 'a' + 'A') : (c) )
#define __ascii_iswalpha(c)     ( ('A' <= (c) && (c) <= 'Z') || ( 'a' <= (c) && (c) <= 'z'))
#define __ascii_iswdigit(c)     ( '0' <= (c) && (c) <= '9')
#define __ascii_towlower(c)     ( (((c) >= L'A') && ((c) <= L'Z')) ? ((c) - L'A' + L'a') : (c) )
#define __ascii_towupper(c)     ( (((c) >= L'a') && ((c) <= L'z')) ? ((c) - L'a' + L'A') : (c) )
#endif  /* _INTERNAL_IFSTRIP_ */

#ifndef _MT
#ifndef __cplusplus
#define isalpha(_c)     (MB_CUR_MAX > 1 ? _isctype(_c,_ALPHA) : __chvalidchk(_c, _ALPHA))
#define isupper(_c)     (MB_CUR_MAX > 1 ? _isctype(_c,_UPPER) : __chvalidchk(_c, _UPPER))
#define islower(_c)     (MB_CUR_MAX > 1 ? _isctype(_c,_LOWER) : __chvalidchk(_c, _LOWER))
#define isdigit(_c)     (MB_CUR_MAX > 1 ? _isctype(_c,_DIGIT) : __chvalidchk(_c, _DIGIT))
#define isxdigit(_c)    (MB_CUR_MAX > 1 ? _isctype(_c,_HEX)   : __chvalidchk(_c, _HEX))
#define isspace(_c)     (MB_CUR_MAX > 1 ? _isctype(_c,_SPACE) : __chvalidchk(_c, _SPACE))
#define ispunct(_c)     (MB_CUR_MAX > 1 ? _isctype(_c,_PUNCT) : __chvalidchk(_c, _PUNCT))
#define isalnum(_c)     (MB_CUR_MAX > 1 ? _isctype(_c,_ALPHA|_DIGIT) : __chvalidchk(_c, (_ALPHA|_DIGIT)))
#define isprint(_c)     (MB_CUR_MAX > 1 ? _isctype(_c,_BLANK|_PUNCT|_ALPHA|_DIGIT) : __chvalidchk(_c, (_BLANK|_PUNCT|_ALPHA|_DIGIT)))
#define isgraph(_c)     (MB_CUR_MAX > 1 ? _isctype(_c,_PUNCT|_ALPHA|_DIGIT) : __chvalidchk(_c, (_PUNCT|_ALPHA|_DIGIT)))
#define iscntrl(_c)     (MB_CUR_MAX > 1 ? _isctype(_c,_CONTROL) : __chvalidchk(_c, _CONTROL))
#endif  /* __cplusplus */
#endif  /* _MT */

#ifdef _MT
                                                                                    /* _MTHREAD_ONLY */
#ifdef _DEBUG
int __cdecl _chvalidator_mt(pthreadlocinfo, int, int);                              /* _MTHREAD_ONLY */
#define __chvalidchk_mt(p, a, b)  _chvalidator_mt(p, a, b)                          /* _MTHREAD_ONLY */
#else  /* _DEBUG */
#define __chvalidchk_mt(p, a, b)  (p->pctype[a] & (b))                              /* _MTHREAD_ONLY */
#endif  /* _DEBUG */
                                                                                    /* _MTHREAD_ONLY */
int __cdecl __isctype_mt(pthreadlocinfo, int, int);                                 /* _MTHREAD_ONLY */
                                                                                    /* _MTHREAD_ONLY */
#define __ischartype_mt(p, c, a)    ( p->mb_cur_max > 1 ? __isctype_mt(p, c, (a)) : __chvalidchk_mt(p,c,a))    /* _MTHREAD_ONLY */
#define __isalpha_mt(p, c)      __ischartype_mt(p, c, _ALPHA)                       /* _MTHREAD_ONLY */
#define __isupper_mt(p, c)      __ischartype_mt(p, c, _UPPER)                       /* _MTHREAD_ONLY */
#define __islower_mt(p, c)      __ischartype_mt(p, c, _LOWER)                       /* _MTHREAD_ONLY */
#define __isdigit_mt(p, c)      __ischartype_mt(p, c, _DIGIT)                       /* _MTHREAD_ONLY */
#define __isxdigit_mt(p, c)     __ischartype_mt(p, c, _HEX)                         /* _MTHREAD_ONLY */
#define __isspace_mt(p, c)      __ischartype_mt(p, c, _SPACE)                       /* _MTHREAD_ONLY */
#define __ispunct_mt(p, c)      __ischartype_mt(p, c, _PUNCT)                       /* _MTHREAD_ONLY */
#define __isalnum_mt(p, c)      __ischartype_mt(p, c, _ALPHA|_DIGIT)                /* _MTHREAD_ONLY */
#define __isprint_mt(p, c)      __ischartype_mt(p, c, _BLANK|_PUNCT|_ALPHA|_DIGIT)  /* _MTHREAD_ONLY */
#define __isgraph_mt(p, c)      __ischartype_mt(p, c, _PUNCT|_ALPHA|_DIGIT)         /* _MTHREAD_ONLY */
#define __iscntrl_mt(p, c)      __ischartype_mt(p, c, _CONTROL)                     /* _MTHREAD_ONLY */
#define __isleadbyte_mt(p, c)   (p->pctype[(unsigned char)(c)] & _LEADBYTE)         /* _MTHREAD_ONLY */
                                                                                    /* _MTHREAD_ONLY */
#endif  /* _MT */

#define _tolower(_c)    ( (_c)-'A'+'a' )
#define _toupper(_c)    ( (_c)-'a'+'A' )

#define __isascii(_c)   ( (unsigned)(_c) < 0x80 )
#define __toascii(_c)   ( (_c) & 0x7f )

#ifndef _WCTYPE_INLINE_DEFINED
#ifndef __cplusplus
#define iswalpha(_c)    ( iswctype(_c,_ALPHA) )
#define iswupper(_c)    ( iswctype(_c,_UPPER) )
#define iswlower(_c)    ( iswctype(_c,_LOWER) )
#define iswdigit(_c)    ( iswctype(_c,_DIGIT) )
#define iswxdigit(_c)   ( iswctype(_c,_HEX) )
#define iswspace(_c)    ( iswctype(_c,_SPACE) )
#define iswpunct(_c)    ( iswctype(_c,_PUNCT) )
#define iswalnum(_c)    ( iswctype(_c,_ALPHA|_DIGIT) )
#define iswprint(_c)    ( iswctype(_c,_BLANK|_PUNCT|_ALPHA|_DIGIT) )
#define iswgraph(_c)    ( iswctype(_c,_PUNCT|_ALPHA|_DIGIT) )
#define iswcntrl(_c)    ( iswctype(_c,_CONTROL) )
#define iswascii(_c)    ( (unsigned)(_c) < 0x80 )
#define isleadbyte(_c)  ( _pctype[(unsigned char)(_c)] & _LEADBYTE)
#endif  /* __cplusplus */
#define _WCTYPE_INLINE_DEFINED
#endif  /* _WCTYPE_INLINE_DEFINED */

#ifdef _MT
                                                                                    /* _MTHREAD_ONLY */
int __cdecl __iswctype_mt(pthreadlocinfo, wchar_t, wctype_t);                       /* _MTHREAD_ONLY */
                                                                                    /* _MTHREAD_ONLY */
#define __iswupper_mt(p, _c)    ( __iswctype_mt(p, _c,_UPPER) )                     /* _MTHREAD_ONLY */
#define __iswlower_mt(p, _c)    ( __iswctype_mt(p, _c,_LOWER) )                     /* _MTHREAD_ONLY */
#define __iswspace_mt(p, _c)    ( __iswctype_mt(p, _c,_SPACE) )                     /* _MTHREAD_ONLY */
#endif  /* _MT */


/* MS C version 2.0 extended ctype macros */

#define __iscsymf(_c)   (isalpha(_c) || ((_c) == '_'))
#define __iscsym(_c)    (isalnum(_c) || ((_c) == '_'))

#endif  /* _CTYPE_DISABLE_MACROS */

#ifdef _MT
int __cdecl __tolower_mt(pthreadlocinfo, int);              /* _MTHREAD_ONLY */
int __cdecl __toupper_mt(pthreadlocinfo, int);              /* _MTHREAD_ONLY */
wchar_t __cdecl __towlower_mt(pthreadlocinfo, wchar_t);     /* _MTHREAD_ONLY */
wchar_t __cdecl __towupper_mt(pthreadlocinfo, wchar_t);     /* _MTHREAD_ONLY */
#else  /* _MT */
#define __tolower_mt(p, c)  tolower(c)                      /* _MTHREAD_ONLY */
#define __toupper_mt(p, c)  toupper(c)                      /* _MTHREAD_ONLY */
#define __towlower_mt(p, c) towlower(c)                     /* _MTHREAD_ONLY */
#define __towupper_mt(p, c) towupper(c)                     /* _MTHREAD_ONLY */
#endif  /* _MT */

#if !__STDC__

/* Non-ANSI names for compatibility */

#ifndef _CTYPE_DEFINED
_CRTIMP int __cdecl isascii(int);
_CRTIMP int __cdecl toascii(int);
_CRTIMP int __cdecl iscsymf(int);
_CRTIMP int __cdecl iscsym(int);
#else  /* _CTYPE_DEFINED */
#define isascii __isascii
#define toascii __toascii
#define iscsymf __iscsymf
#define iscsym  __iscsym
#endif  /* _CTYPE_DEFINED */

#endif  /* !__STDC__ */

#ifdef __cplusplus
}
#endif  /* __cplusplus */


#endif  /* _INC_CTYPE */
