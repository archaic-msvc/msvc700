/***
*excpt.h - defines exception values, types and routines
*
*       Copyright (c) 1990-2001, Microsoft Corporation. All rights reserved.
*
*Purpose:
*       This file contains the definitions and prototypes for the compiler-
*       dependent intrinsics, support functions and keywords which implement
*       the structured exception handling extensions.
*
*       [Public]
*
****/

#if     _MSC_VER > 1000
#pragma once
#endif

#ifndef _INC_EXCPT
#define _INC_EXCPT

#if     !defined(_WIN32)
#error ERROR: Only Win32 target supported!
#endif


#ifdef  _MSC_VER
/*
 * Currently, all MS C compilers for Win32 platforms default to 8 byte
 * alignment.
 */
#pragma pack(push,8)
#endif  /* _MSC_VER */

#ifdef  __cplusplus
extern "C" {
#endif



/* Define _CRTIMP */

#ifndef _CRTIMP
#ifdef  _DLL
#define _CRTIMP __declspec(dllimport)
#else   /* ndef _DLL */
#define _CRTIMP
#endif  /* _DLL */
#endif  /* _CRTIMP */


/* Define __cdecl for non-Microsoft compilers */

#if     ( !defined(_MSC_VER) && !defined(__cdecl) )
#define __cdecl
#endif


/*
 * Exception disposition return values.
 */
typedef enum _EXCEPTION_DISPOSITION {
    ExceptionContinueExecution,
    ExceptionContinueSearch,
    ExceptionNestedException,
    ExceptionCollidedUnwind
} EXCEPTION_DISPOSITION;


/*
 * Prototype for SEH support function.
 */

#ifdef  _M_IX86

/*
 * Declarations to keep MS C 8 (386/486) compiler happy
 */
struct _EXCEPTION_RECORD;
struct _CONTEXT;

EXCEPTION_DISPOSITION __cdecl _except_handler (
    struct _EXCEPTION_RECORD *ExceptionRecord,
    void * EstablisherFrame,
    struct _CONTEXT *ContextRecord,
    void * DispatcherContext
    );

#elif   defined(_M_MRX000) || defined(_M_ALPHA) || defined(_M_PPC) || defined(_M_IA64)

/*
 * Declarations to keep MIPS, ALPHA, and PPC compiler happy
 */
typedef struct _EXCEPTION_POINTERS *Exception_info_ptr;
struct _EXCEPTION_RECORD;
struct _CONTEXT;
struct _DISPATCHER_CONTEXT;


#if defined(_M_IA64)
_CRTIMP EXCEPTION_DISPOSITION __C_specific_handler (
    struct _EXCEPTION_RECORD *ExceptionRecord,
    unsigned __int64 MemoryStackFp,
    unsigned __int64 BackingStoreFp,
    struct _CONTEXT *ContextRecord,
    struct _DISPATCHER_CONTEXT *DispatcherContext,
    unsigned __int64 GlobalPointer
    );
#else
_CRTIMP EXCEPTION_DISPOSITION __C_specific_handler (
    struct _EXCEPTION_RECORD *ExceptionRecord,
    void *EstablisherFrame,
    struct _CONTEXT *ContextRecord,
    struct _DISPATCHER_CONTEXT *DispatcherContext
    );
#endif /* defined(_M_IA64) */

#endif


/*
 * Keywords and intrinsics for SEH
 */

#ifdef  _MSC_VER

#define GetExceptionCode            _exception_code
#define exception_code              _exception_code
#define GetExceptionInformation     (struct _EXCEPTION_POINTERS *)_exception_info
#define exception_info              (struct _EXCEPTION_POINTERS *)_exception_info
#define AbnormalTermination         _abnormal_termination
#define abnormal_termination        _abnormal_termination

unsigned long __cdecl _exception_code(void);
void *        __cdecl _exception_info(void);
int           __cdecl _abnormal_termination(void);

#endif


/*
 * Legal values for expression in except().
 */

#define EXCEPTION_EXECUTE_HANDLER       1
#define EXCEPTION_CONTINUE_SEARCH       0
#define EXCEPTION_CONTINUE_EXECUTION    -1



#ifdef  __cplusplus
}
#endif

#ifdef  _MSC_VER
#pragma pack(pop)
#endif  /* _MSC_VER */

#endif  /* _INC_EXCPT */
