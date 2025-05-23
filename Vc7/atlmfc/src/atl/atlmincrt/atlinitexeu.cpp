// This is a part of the Active Template Library.
// Copyright (C) 1996-2001 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Active Template Library Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Active Template Library product.

#include "StdAfx.H"

extern "C" void DoInitialization();
extern "C" void DoCleanup();

// wWinMain is not defined in winbase.h.
extern "C" int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nShowCmd);

#define SPACECHAR   _T(' ')
#define DQUOTECHAR  _T('\"')


extern "C" void wWinMainCRTStartup()
{
	LPWSTR lpszCommandLine = ::GetCommandLineW();
	if (lpszCommandLine == NULL)
		::ExitProcess((UINT)-1);

	DoInitialization();

	// Skip past program name (first token in command line).
	// Check for and handle quoted program name.
	if (*lpszCommandLine == DQUOTECHAR)
	{
		// Scan, and skip over, subsequent characters until
		// another double-quote or a null is encountered.
		do
		{
			lpszCommandLine = ::CharNextW(lpszCommandLine);
		}
		while ((*lpszCommandLine != DQUOTECHAR) && (*lpszCommandLine != _T('\0')));

		// If we stopped on a double-quote (usual case), skip over it.
		if (*lpszCommandLine == DQUOTECHAR)
			lpszCommandLine = ::CharNextW(lpszCommandLine);
	}
	else
	{
		while (*lpszCommandLine > SPACECHAR)
			lpszCommandLine = ::CharNextW(lpszCommandLine);
	}

	// Skip past any white space preceeding the second token.
	while (*lpszCommandLine && (*lpszCommandLine <= SPACECHAR))
		lpszCommandLine = ::CharNextW(lpszCommandLine);

	STARTUPINFOW StartupInfo;
	StartupInfo.dwFlags = 0;
	::GetStartupInfoW(&StartupInfo);

	int nRet = wWinMain(::GetModuleHandleA(NULL), NULL, lpszCommandLine,
		(StartupInfo.dwFlags & STARTF_USESHOWWINDOW) ?
		StartupInfo.wShowWindow : SW_SHOWDEFAULT);

	DoCleanup();

	::ExitProcess((UINT)nRet);
}

