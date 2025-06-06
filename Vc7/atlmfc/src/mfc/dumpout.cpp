// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-2001 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#include "stdafx.h"
#include <stdarg.h>

#ifdef _DEBUG   // entire file

#ifdef AFX_AUX_SEG
#pragma code_seg(AFX_AUX_SEG)
#endif


/////////////////////////////////////////////////////////////////////////////
// Helper routines that can be called from debugger

void AFXAPI AfxDump(const CObject* pOb)
{
	afxDump << pOb;
}

/////////////////////////////////////////////////////////////////////////////
// Diagnostic Trace

void AFX_CDECL AfxTrace(LPCTSTR lpszFormat, ...)
{
	va_list args;
	va_start(args, lpszFormat);

	int nBuf;
	TCHAR szBuffer[512];

	nBuf = _vsntprintf(szBuffer, _countof(szBuffer), lpszFormat, args);

	// was there an error? was the expanded string too long?
	ASSERT(nBuf >= 0);

	afxDump << szBuffer;

	va_end(args);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
