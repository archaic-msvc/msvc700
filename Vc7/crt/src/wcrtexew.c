/***
*wcrtexew.c - Initialization for Windows EXE using CRT DLL, wchar_t version
*
*       Copyright (c) 1985-2001, Microsoft Corporation. All rights reserved.
*
*Purpose:
*       This is the actual startup routine for Windows apps using wide
*       characters.  It calls the user's main routine wWinMain() after
*       performing C Run-Time Library initialization.
*
*******************************************************************************/

#ifdef CRTDLL

#define _WINMAIN_
#include "wcrtexe.c"

#endif  /* CRTDLL */
