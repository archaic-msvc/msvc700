/***
*stdlib.h - declarations/definitions for commonly used library functions
*
*       Copyright (c) 1985-2001, Microsoft Corporation. All rights reserved.
*
*Purpose:
*       This include file contains the function declarations for commonly
*       used library functions which either don't fit somewhere else, or,
*       cannot be declared in the normal place for other reasons.
*       [ANSI]
*
*       [Public]
*
****/

#if _MSC_VER > 1000
#pragma once
#endif  /* _MSC_VER > 1000 */

#ifndef _INC_STDLIB
#define _INC_STDLIB

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

#ifdef _MSC_VER
/*
 * Currently, all MS C compilers for Win32 platforms default to 8 byte
 * alignment.
 */
#pragma pack(push,8)
#endif  /* _MSC_VER */

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#ifndef _INTERNAL_IFSTRIP_
#include <cruntime.h>
#endif  /* _INTERNAL_IFSTRIP_ */

#if !defined (_W64)
#if !defined(__midl) && (defined(_X86_) || defined(_M_IX86)) && _MSC_VER >= 1300
#define _W64 __w64
#else  /* !defined(__midl) && (defined(_X86_) || defined(_M_IX86)) && _MSC_VER >= 1300 */
#define _W64
#endif  /* !defined(__midl) && (defined(_X86_) || defined(_M_IX86)) && _MSC_VER >= 1300 */
#endif  /* !defined (_W64) */

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


#ifndef _SIZE_T_DEFINED
#ifdef _WIN64
typedef unsigned __int64    size_t;
#else  /* _WIN64 */
typedef _W64 unsigned int   size_t;
#endif  /* _WIN64 */
#define _SIZE_T_DEFINED
#endif  /* _SIZE_T_DEFINED */


#ifndef _WCHAR_T_DEFINED
typedef unsigned short wchar_t;
#define _WCHAR_T_DEFINED
#endif  /* _WCHAR_T_DEFINED */


/* Define NULL pointer value */

#ifndef NULL
#ifdef __cplusplus
#define NULL    0
#else  /* __cplusplus */
#define NULL    ((void *)0)
#endif  /* __cplusplus */
#endif  /* NULL */


/* Definition of the argument values for the exit() function */

#define EXIT_SUCCESS    0
#define EXIT_FAILURE    1


#ifndef _ONEXIT_T_DEFINED
typedef int (__cdecl * _onexit_t)(void);
#if !__STDC__
/* Non-ANSI name for compatibility */
#define onexit_t _onexit_t
#endif  /* !__STDC__ */
#define _ONEXIT_T_DEFINED
#endif  /* _ONEXIT_T_DEFINED */


/* Data structure definitions for div and ldiv runtimes. */

#ifndef _DIV_T_DEFINED

typedef struct _div_t {
        int quot;
        int rem;
} div_t;

typedef struct _ldiv_t {
        long quot;
        long rem;
} ldiv_t;

#define _DIV_T_DEFINED
#endif  /* _DIV_T_DEFINED */


/* Maximum value that can be returned by the rand function. */

#define RAND_MAX 0x7fff

/*
 * Maximum number of bytes in multi-byte character in the current locale
 * (also defined in ctype.h).
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


/* Minimum and maximum macros */

#define __max(a,b)  (((a) > (b)) ? (a) : (b))
#define __min(a,b)  (((a) < (b)) ? (a) : (b))

/*
 * Sizes for buffers used by the _makepath() and _splitpath() functions.
 * note that the sizes include space for 0-terminator
 */
#define _MAX_PATH   260 /* max. length of full pathname */
#define _MAX_DRIVE  3   /* max. length of drive component */
#define _MAX_DIR    256 /* max. length of path component */
#define _MAX_FNAME  256 /* max. length of file name component */
#define _MAX_EXT    256 /* max. length of extension component */

/*
 * Argument values for _set_error_mode().
 */
#define _OUT_TO_DEFAULT 0
#define _OUT_TO_STDERR  1
#define _OUT_TO_MSGBOX  2
#define _REPORT_ERRMODE 3

#if defined (_M_IX86)
/*
 * Typedefs and argument values for _set_security_error_handler()
 */
#define _SECERR_BUFFER_OVERRUN 1        /* void* arg ignored */
typedef void (__cdecl * _secerr_handler_func)(int, void *);
#endif  /* defined (_M_IX86) */

/* External variable declarations */

#if defined (_MT) || defined (_DLL)
_CRTIMP int * __cdecl _errno(void);
_CRTIMP unsigned long * __cdecl __doserrno(void);
#define errno       (*_errno())
#define _doserrno   (*__doserrno())
#else  /* defined (_MT) || defined (_DLL) */
_CRTIMP extern int errno;               /* XENIX style error number */
_CRTIMP extern unsigned long _doserrno; /* OS system error value */
#endif  /* defined (_MT) || defined (_DLL) */


_CRTIMP extern char * _sys_errlist[];   /* perror error message table */
_CRTIMP extern int _sys_nerr;           /* # of entries in sys_errlist table */


#if defined (_DLL) && defined (_M_IX86)

#define __argc      (*__p___argc())     /* count of cmd line args */
#define __argv      (*__p___argv())     /* pointer to table of cmd line args */
#define __wargv     (*__p___wargv())    /* pointer to table of wide cmd line args */
#define _environ    (*__p__environ())   /* pointer to environment table */
#define _wenviron   (*__p__wenviron())  /* pointer to wide environment table */
#define _pgmptr     (*__p__pgmptr())    /* points to the module (EXE) name */
#define _wpgmptr    (*__p__wpgmptr())   /* points to the module (EXE) wide name */

_CRTIMP int *          __cdecl __p___argc(void);
_CRTIMP char ***       __cdecl __p___argv(void);
_CRTIMP wchar_t ***    __cdecl __p___wargv(void);
_CRTIMP char ***       __cdecl __p__environ(void);
_CRTIMP wchar_t ***    __cdecl __p__wenviron(void);
_CRTIMP char **        __cdecl __p__pgmptr(void);
_CRTIMP wchar_t **     __cdecl __p__wpgmptr(void);

#ifndef _INTERNAL_IFSTRIP_
/* Retained for compatibility with VC++ 5.0 and earlier versions */
_CRTIMP int *          __cdecl __p__fmode(void);
_CRTIMP int *          __cdecl __p__fileinfo(void);
_CRTIMP unsigned int * __cdecl __p__osver(void);
_CRTIMP unsigned int * __cdecl __p__winver(void);
_CRTIMP unsigned int * __cdecl __p__winmajor(void);
_CRTIMP unsigned int * __cdecl __p__winminor(void);
#endif  /* _INTERNAL_IFSTRIP_ */

#else  /* defined (_DLL) && defined (_M_IX86) */

_CRTIMP extern int __argc;          /* count of cmd line args */
_CRTIMP extern char ** __argv;      /* pointer to table of cmd line args */
_CRTIMP extern wchar_t ** __wargv;  /* pointer to table of wide cmd line args */

_CRTIMP extern char ** _environ;    /* pointer to environment table */
_CRTIMP extern wchar_t ** _wenviron;    /* pointer to wide environment table */

_CRTIMP extern char * _pgmptr;      /* points to the module (EXE) name */
_CRTIMP extern wchar_t * _wpgmptr;  /* points to the module (EXE) wide name */

#endif  /* defined (_DLL) && defined (_M_IX86) */


#ifdef SPECIAL_CRTEXE
        extern int _fmode;          /* default file translation mode */
#else  /* SPECIAL_CRTEXE */
_CRTIMP extern int _fmode;          /* default file translation mode */
#endif  /* SPECIAL_CRTEXE */
_CRTIMP extern int _fileinfo;       /* open file info mode (for spawn) */


/* Windows major/minor and O.S. version numbers */

_CRTIMP extern unsigned int _osplatform;
_CRTIMP extern unsigned int _osver;
_CRTIMP extern unsigned int _winver;
_CRTIMP extern unsigned int _winmajor;
_CRTIMP extern unsigned int _winminor;


/* function prototypes */

#if _MSC_VER >= 1200
_CRTIMP __declspec(noreturn) void   __cdecl abort(void);
_CRTIMP __declspec(noreturn) void   __cdecl exit(int);
#else  /* _MSC_VER >= 1200 */
_CRTIMP void   __cdecl abort(void);
_CRTIMP void   __cdecl exit(int);
#endif  /* _MSC_VER >= 1200 */

#if defined (_M_MRX000)
_CRTIMP int    __cdecl abs(int);
#else  /* defined (_M_MRX000) */
        int    __cdecl abs(int);
#endif  /* defined (_M_MRX000) */
        __int64    __cdecl _abs64(__int64);
        int    __cdecl atexit(void (__cdecl *)(void));
_CRTIMP double __cdecl atof(const char *);
_CRTIMP int    __cdecl atoi(const char *);
_CRTIMP long   __cdecl atol(const char *);
_CRTIMP void * __cdecl bsearch(const void *, const void *, size_t, size_t,
        int (__cdecl *)(const void *, const void *));
        unsigned short __cdecl _byteswap_ushort(unsigned short);
        unsigned long  __cdecl _byteswap_ulong (unsigned long);
        unsigned __int64 __cdecl _byteswap_uint64(unsigned __int64);
_CRTIMP void * __cdecl calloc(size_t, size_t);
_CRTIMP div_t  __cdecl div(int, int);
_CRTIMP void   __cdecl free(void *);
_CRTIMP char * __cdecl getenv(const char *);
_CRTIMP char * __cdecl _itoa(int, char *, int);
#if _INTEGRAL_MAX_BITS >= 64   
_CRTIMP char * __cdecl _i64toa(__int64, char *, int);
_CRTIMP char * __cdecl _ui64toa(unsigned __int64, char *, int);
_CRTIMP __int64 __cdecl _atoi64(const char *);
_CRTIMP __int64 __cdecl _strtoi64(const char *, char **, int);
_CRTIMP unsigned __int64 __cdecl _strtoui64(const char *, char **, int);
#endif  /* _INTEGRAL_MAX_BITS >= 64    */
#if defined (_M_MRX000)
_CRTIMP long __cdecl labs(long);
#else  /* defined (_M_MRX000) */
        long __cdecl labs(long);
#endif  /* defined (_M_MRX000) */
_CRTIMP ldiv_t __cdecl ldiv(long, long);
_CRTIMP char * __cdecl _ltoa(long, char *, int);
_CRTIMP void * __cdecl malloc(size_t);
_CRTIMP int    __cdecl mblen(const char *, size_t);
_CRTIMP size_t __cdecl _mbstrlen(const char *s);
_CRTIMP int    __cdecl mbtowc(wchar_t *, const char *, size_t);
_CRTIMP size_t __cdecl mbstowcs(wchar_t *, const char *, size_t);
_CRTIMP void   __cdecl qsort(void *, size_t, size_t, int (__cdecl *)
        (const void *, const void *));
_CRTIMP int    __cdecl rand(void);
_CRTIMP void * __cdecl realloc(void *, size_t);
_CRTIMP int    __cdecl _set_error_mode(int);
#if defined (_M_IX86)
_CRTIMP _secerr_handler_func
               __cdecl _set_security_error_handler(_secerr_handler_func);
#endif  /* defined (_M_IX86) */
_CRTIMP void   __cdecl srand(unsigned int);
_CRTIMP double __cdecl strtod(const char *, char **);
_CRTIMP long   __cdecl strtol(const char *, char **, int);
_CRTIMP unsigned long __cdecl strtoul(const char *, char **, int);
_CRTIMP int    __cdecl system(const char *);
_CRTIMP char * __cdecl _ultoa(unsigned long, char *, int);
_CRTIMP int    __cdecl wctomb(char *, wchar_t);
_CRTIMP size_t __cdecl wcstombs(char *, const wchar_t *, size_t);


#ifndef _WSTDLIB_DEFINED

/* wide function prototypes, also declared in wchar.h  */

_CRTIMP wchar_t * __cdecl _itow (int, wchar_t *, int);
_CRTIMP wchar_t * __cdecl _ltow (long, wchar_t *, int);
_CRTIMP wchar_t * __cdecl _ultow (unsigned long, wchar_t *, int);
_CRTIMP double __cdecl wcstod(const wchar_t *, wchar_t **);
_CRTIMP long   __cdecl wcstol(const wchar_t *, wchar_t **, int);
_CRTIMP unsigned long __cdecl wcstoul(const wchar_t *, wchar_t **, int);
_CRTIMP wchar_t * __cdecl _wgetenv(const wchar_t *);
_CRTIMP int    __cdecl _wsystem(const wchar_t *);
_CRTIMP double __cdecl _wtof(const wchar_t *);
_CRTIMP int __cdecl _wtoi(const wchar_t *);
_CRTIMP long __cdecl _wtol(const wchar_t *);
#if _INTEGRAL_MAX_BITS >= 64   
_CRTIMP wchar_t * __cdecl _i64tow(__int64, wchar_t *, int);
_CRTIMP wchar_t * __cdecl _ui64tow(unsigned __int64, wchar_t *, int);
_CRTIMP __int64   __cdecl _wtoi64(const wchar_t *);
_CRTIMP __int64   __cdecl _wcstoi64(const wchar_t *, wchar_t **, int);
_CRTIMP unsigned __int64  __cdecl _wcstoui64(const wchar_t *, wchar_t **, int);
#endif  /* _INTEGRAL_MAX_BITS >= 64    */

#define _WSTDLIB_DEFINED
#endif  /* _WSTDLIB_DEFINED */



_CRTIMP char * __cdecl _ecvt(double, int, int *, int *);
#if _MSC_VER >= 1200
_CRTIMP __declspec(noreturn) void   __cdecl _exit(int);
#else  /* _MSC_VER >= 1200 */
_CRTIMP void   __cdecl _exit(int);
#endif  /* _MSC_VER >= 1200 */
_CRTIMP char * __cdecl _fcvt(double, int, int *, int *);
_CRTIMP char * __cdecl _fullpath(char *, const char *, size_t);
_CRTIMP char * __cdecl _gcvt(double, int, char *);
        unsigned long __cdecl _lrotl(unsigned long, int);
        unsigned long __cdecl _lrotr(unsigned long, int);
_CRTIMP void   __cdecl _makepath(char *, const char *, const char *, const char *,
        const char *);
        _onexit_t __cdecl _onexit(_onexit_t);
_CRTIMP void   __cdecl perror(const char *);
_CRTIMP int    __cdecl _putenv(const char *);
        unsigned int __cdecl _rotl(unsigned int, int);
        unsigned __int64 __cdecl _rotl64(unsigned __int64, int);
        unsigned int __cdecl _rotr(unsigned int, int);
        unsigned __int64 __cdecl _rotr64(unsigned __int64, int);
_CRTIMP void   __cdecl _searchenv(const char *, const char *, char *);
_CRTIMP void   __cdecl _splitpath(const char *, char *, char *, char *, char *);
_CRTIMP void   __cdecl _swab(char *, char *, int);

#ifndef _WSTDLIBP_DEFINED

/* wide function prototypes, also declared in wchar.h  */

_CRTIMP wchar_t * __cdecl _wfullpath(wchar_t *, const wchar_t *, size_t);
_CRTIMP void   __cdecl _wmakepath(wchar_t *, const wchar_t *, const wchar_t *, const wchar_t *,
        const wchar_t *);
_CRTIMP void   __cdecl _wperror(const wchar_t *);
_CRTIMP int    __cdecl _wputenv(const wchar_t *);
_CRTIMP void   __cdecl _wsearchenv(const wchar_t *, const wchar_t *, wchar_t *);
_CRTIMP void   __cdecl _wsplitpath(const wchar_t *, wchar_t *, wchar_t *, wchar_t *, wchar_t *);

#define _WSTDLIBP_DEFINED
#endif  /* _WSTDLIBP_DEFINED */

/* --------- The following functions are OBSOLETE --------- */
/* The Win32 API SetErrorMode, Beep and Sleep should be used instead. */
_CRTIMP void __cdecl _seterrormode(int);
_CRTIMP void __cdecl _beep(unsigned, unsigned);
_CRTIMP void __cdecl _sleep(unsigned long);
/* --------- The preceding functions are OBSOLETE --------- */



#if !__STDC__
/* --------- The declarations below should not be in stdlib.h --------- */
/* --------- and will be removed in a future release. Include --------- */
/* --------- ctype.h to obtain these declarations.            --------- */
#ifndef tolower
_CRTIMP int __cdecl tolower(int);
#endif  /* tolower */
#ifndef toupper
_CRTIMP int __cdecl toupper(int);
#endif  /* toupper */
/* --------- The declarations above will be removed.          --------- */
#endif  /* !__STDC__ */

#ifdef _MT
                                                            /* _MTHREAD_ONLY */
struct  threadlocaleinfostruct;                             /* _MTHREAD_ONLY */
typedef struct threadlocaleinfostruct * pthreadlocinfo;     /* _MTHREAD_ONLY */
                                                            /* _MTHREAD_ONLY */
char * __cdecl _getenv_lk(const char *);                    /* _MTHREAD_ONLY */
wchar_t * __cdecl _wgetenv_lk(const wchar_t *);             /* _MTHREAD_ONLY */
int    __cdecl _putenv_lk(const char *);                    /* _MTHREAD_ONLY */
int    __cdecl _wputenv_lk(const wchar_t *);                /* _MTHREAD_ONLY */
int    __cdecl _mblen_lk(const char *, size_t);             /* _MTHREAD_ONLY */
int    __cdecl __mbtowc_mt(pthreadlocinfo, wchar_t*, const char*,size_t);    /* _MTHREAD_ONLY */
size_t __cdecl __mbstowcs_mt(pthreadlocinfo, wchar_t*, const char*,size_t);  /* _MTHREAD_ONLY */
int    __cdecl __wctomb_mt(pthreadlocinfo, char*, wchar_t); /* _MTHREAD_ONLY */
size_t __cdecl __wcstombs_mt(pthreadlocinfo, char*, const wchar_t*,size_t);  /* _MTHREAD_ONLY */
#else  /* _MT */
#define _getenv_lk(envvar)  getenv(envvar)                  /* _MTHREAD_ONLY */
#define _wgetenv_lk(envvar)  _wgetenv(envvar)               /* _MTHREAD_ONLY */
#define _putenv_lk(envvar)  _putenv(envvar)                 /* _MTHREAD_ONLY */
#define _wputenv_lk(envvar)  _wputenv(envvar)               /* _MTHREAD_ONLY */
#define _mblen_lk(s,n) mblen(s,n)                           /* _MTHREAD_ONLY */
#define __mbtowc_mt(p,pwc,s,n)     mbtowc(pwc,s,n)          /* _MTHREAD_ONLY */
#define __mbstowcs_mt(p,pwcs,s,n)  mbstowcs(pwcs,s,n)       /* _MTHREAD_ONLY */
#define __wctomb_mt(p,s,wchar) wctomb(s,wchar)              /* _MTHREAD_ONLY */
#define __wcstombs_mt(p,s,pwcs,n) wcstombs(s,pwcs,n)        /* _MTHREAD_ONLY */
                                                            /* _MTHREAD_ONLY */
#endif  /* _MT */

#if !__STDC__


/* Non-ANSI names for compatibility */

#ifndef __cplusplus
#define max(a,b)    (((a) > (b)) ? (a) : (b))
#define min(a,b)    (((a) < (b)) ? (a) : (b))
#endif  /* __cplusplus */

#define sys_errlist _sys_errlist
#define sys_nerr    _sys_nerr
#define environ     _environ

_CRTIMP char * __cdecl ecvt(double, int, int *, int *);
_CRTIMP char * __cdecl fcvt(double, int, int *, int *);
_CRTIMP char * __cdecl gcvt(double, int, char *);
_CRTIMP char * __cdecl itoa(int, char *, int);
_CRTIMP char * __cdecl ltoa(long, char *, int);
        onexit_t __cdecl onexit(onexit_t);
_CRTIMP int    __cdecl putenv(const char *);
_CRTIMP void   __cdecl swab(char *, char *, int);
_CRTIMP char * __cdecl ultoa(unsigned long, char *, int);


#endif  /* !__STDC__ */

#ifdef __cplusplus
}

#endif  /* __cplusplus */

#ifdef _MSC_VER
#pragma pack(pop)
#endif  /* _MSC_VER */

#endif  /* _INC_STDLIB */
