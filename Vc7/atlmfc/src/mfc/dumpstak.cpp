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

#include <atlutil.h>

#ifdef AFX_AUX_SEG
#pragma code_seg(AFX_AUX_SEG)
#endif

/////////////////////////////////////////////////////////////////////////////
// Routine to produce stack dump

class CTraceClipboardData : public IStackDumpHandler
{
	HGLOBAL m_hMemory;
	DWORD	m_dwSize;
	DWORD m_dwUsed;
	DWORD m_dwTarget;

public:
	void __stdcall OnBegin()
	{
		SendOut("=== begin AfxDumpStack output ===\r\n");
	}
	void __stdcall OnEntry(void *pvAddress, LPCSTR szModule, LPCSTR szSymbol)
	{
		char sz[20];
		wsprintfA(sz, "%8.8X: ", pvAddress);
		SendOut(sz);

		if (szModule)
		{
			ATLASSERT(szSymbol);
			SendOut(szModule);
			SendOut("! ");
			SendOut(szSymbol);
		}
		else
			SendOut("symbol not found");
		SendOut("\r\n");
	}

	void __stdcall OnError(LPCSTR szError)
	{
		SendOut(szError);
	}
	void __stdcall OnEnd()
	{
		SendOut("=== end AfxDumpStack() output ===\r\n");
	}
	void SendOut(LPCSTR pszData);
	CTraceClipboardData(DWORD dwTarget);
	~CTraceClipboardData();
};

CTraceClipboardData::CTraceClipboardData(DWORD dwTarget)
	: m_dwTarget(dwTarget), m_dwSize(0), m_dwUsed(0), m_hMemory(NULL)
{
}

CTraceClipboardData::~CTraceClipboardData()
{
	if (m_hMemory != NULL)
	{
		// chuck it onto the clipboard
		// don't free it unless there's an error

		if (!OpenClipboard(NULL))
			GlobalFree(m_hMemory);
		else if (!EmptyClipboard() ||
				SetClipboardData(CF_TEXT, m_hMemory) == NULL)
		{
			GlobalFree(m_hMemory);
		}
		else
			CloseClipboard();
	}
}

void CTraceClipboardData::SendOut(LPCSTR pszData)
{
	int nLength;
	if (pszData == NULL || (nLength = lstrlenA(pszData)) == 0)
		return;

	// send it to TRACE (can be redirected)
	if (m_dwTarget & AFX_STACK_DUMP_TARGET_TRACE)
		TRACE(traceAppMsg, 0, "%hs", pszData);

	// send it to OutputDebugString() (can't redirect)
	if (m_dwTarget & AFX_STACK_DUMP_TARGET_ODS)
		OutputDebugStringA(pszData);

	// build a buffer for the clipboard
	if (m_dwTarget & AFX_STACK_DUMP_TARGET_CLIPBOARD)
	{
		if (m_hMemory == NULL)
		{
			if( (nLength + 1) > (1024L*1024L) )
			{
				TRACE(traceAppMsg, 0, "AfxDumpStack Error: pszData larger than one megabyte.\n");
				m_dwTarget &= ~AFX_STACK_DUMP_TARGET_CLIPBOARD;
			}
			else
			{
				m_hMemory = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, max( 1024L, nLength + 1 ) );
				if (m_hMemory == NULL)
				{
					TRACE(traceAppMsg, 0, "AfxDumpStack Error: No memory available for clipboard.\n");
					m_dwTarget &= ~AFX_STACK_DUMP_TARGET_CLIPBOARD;
				}
				else
				{
					m_dwUsed = nLength;
					m_dwSize = max( 1024L, nLength + 1 );
					LPSTR pstr = (LPSTR) GlobalLock(m_hMemory);
					if (pstr != NULL)
					{
						lstrcpyA(pstr, pszData);
						GlobalUnlock(m_hMemory);
					}
					else
					{
						TRACE(traceAppMsg, 0, "AfxDumpStack Error: Couldn't lock memory!\n");
						GlobalFree(m_hMemory);
						m_hMemory = NULL;
						m_dwTarget &= ~AFX_STACK_DUMP_TARGET_CLIPBOARD;
					}
				}
			}
		}
		else
		{
			if ((m_dwUsed + nLength + 1) >= m_dwSize)
			{
				// grow by leaps and bounds
				m_dwSize *= 2;
				if (m_dwSize > (1024L*1024L))
				{
					TRACE(traceAppMsg, 0, "AfxDumpStack Error: more than one megabyte on clipboard.\n");
					m_dwTarget &= ~AFX_STACK_DUMP_TARGET_CLIPBOARD;
				}

				HGLOBAL hMemory = GlobalReAlloc(m_hMemory, m_dwSize, GMEM_MOVEABLE);
				if (hMemory == NULL)
				{
					TRACE(traceAppMsg, 0, "AfxDumpStack Error: Couldn't get %d bytes!\n", m_dwSize);
					m_dwTarget &= ~AFX_STACK_DUMP_TARGET_CLIPBOARD;
				}
				else
					m_hMemory = hMemory;
			}

			LPSTR pstr = (LPSTR) GlobalLock(m_hMemory);
			if (pstr != NULL)
			{
				lstrcpyA(pstr + m_dwUsed, pszData);
				m_dwUsed += nLength;
				GlobalUnlock(m_hMemory);
			}
			else
			{
				TRACE(traceAppMsg, 0, "AfxDumpStack Error: Couldn't lock memory!\n");
				m_dwTarget &= ~AFX_STACK_DUMP_TARGET_CLIPBOARD;
			}
		}
	}

	return;
}

/////////////////////////////////////////////////////////////////////////////
// AfxDumpStack API

void AFXAPI AfxDumpStack(DWORD dwTarget /* = AFX_STACK_DUMP_TARGET_DEFAULT */)
{
	CTraceClipboardData clipboardData(dwTarget);

	AtlDumpStack(&clipboardData);
}
