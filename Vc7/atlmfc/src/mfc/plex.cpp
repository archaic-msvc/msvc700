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

// Collection support
#ifdef AFX_COLL_SEG
#pragma code_seg(AFX_COLL_SEG)
#endif


#define new DEBUG_NEW

/////////////////////////////////////////////////////////////////////////////
// CPlex

CPlex* PASCAL CPlex::Create(CPlex*& pHead, UINT_PTR nMax, UINT_PTR cbElement)
{
	ASSERT(nMax > 0 && cbElement > 0);
	CPlex* p = (CPlex*) new BYTE[sizeof(CPlex) + nMax * cbElement];
			// may throw exception
	p->pNext = pHead;
	pHead = p;  // change head (adds in reverse order for simplicity)
	return p;
}

void CPlex::FreeDataChain()     // free this one and links
{
	CPlex* p = this;
	while (p != NULL)
	{
		BYTE* bytes = (BYTE*) p;
		CPlex* pNext = p->pNext;
		delete[] bytes;
		p = pNext;
	}
}
