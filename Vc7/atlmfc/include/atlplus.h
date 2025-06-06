// This is a part of the Visual C++ COM Library
// Copyright (C) 1996-2001 Microsoft Corporation
// All rights reserved.

#ifndef __ATLPLUS_H__
#define __ATLPLUS_H__

#pragma once

#ifndef __cplusplus
	#error The Visual C++ COM Library requires C++ compilation (use a .cpp suffix)
#endif

#include <shellapi.h>
#include "atlevent.h"
#include <atlstr.h>

[
	provider(name="ATL4", uuid="BC6B4B8A-0E4A-4bc9-B319-9DC2ACFB61EE")
];

namespace ATL
{

template <typename __OuterClass = __current_class, unsigned long __ClassOffset = __member_offset>
class ContainerPair
{
public:
	typedef __OuterClass _ContainingClass;
	static unsigned long _GetContainerOffset()
	{
		return __ClassOffset;
	}
};

// Any contained class which wishes to access the members of its containing class can use this helper to properly
// accesss the this pointer of the containing class.  The contained class should derive from this.
//
template<typename T>
class OuterClassHelper
{
public:
	__declspec(property(get=__GetOuter)) T::_ContainingClass* outer;
	T::_ContainingClass* __GetOuter()
	{
		return reinterpret_cast<T::_ContainingClass *>(reinterpret_cast<char *>(this) - T::_GetContainerOffset());
	}
};

// This class is constructor helper
template <class TCtor, class T = ContainerPair<> >
class Constructor : public OuterClassHelper<T>
{
public:
	Constructor()
	{
		TCtor x(__GetOuter());
	}
};

// Registry

#define rgsopEndOfStream 0
#define rgsopAddKeyNoRemove  1
#define rgsopAddKeyForceRemove 2
#define rgsopAddKey 3
#define rgsopDeleteKey 4
#define rgsopOpenAsParent 5
#define rgsopCloseAsParent 6
#define rgsopOpenRootKey 7
#define rgsopAddStringValue 8
#define rgsopAddMultiStringValue 9
#define rgsopAddDWORDValue 10
#define rgsopAddBinaryValue 11

struct RGSStrings
{
	LPTSTR sz;
	BYTE bHasReplacement;
};
struct RGSDWORD
{
	DWORD dwValueOrIndex;
	BYTE bHasReplacement;
};
struct RGSBinary
{
	BYTE* pBytes;
	DWORD dwLenOrIndex;
	BYTE bHasReplacement;
};

typedef DWORD RGSOps;

enum RDXOperations
{
	eReadFromReg,
	eWriteToReg,
	eDeleteFromReg
};

class CVMExpansionVector : public CSimpleMap<LPTSTR, LPTSTR>
{
public:
	int FindKey(LPTSTR& key) const
	{
		for(int i = 0; i < m_nSize; i++)
		{
			if (lstrcmpi(m_aKey[i],key) == 0)
				return i;
		}
		return -1;  // not found
	}
	int FindVal(LPTSTR& val) const
	{
		for(int i = 0; i < m_nSize; i++)
		{
			if (lstrcmpi(m_aVal[i],val) == 0)
				return i;
		}
		return -1;  // not found
	}

	LPTSTR Lookup(LPTSTR key) const
	{
		int nIndex = FindKey(key);
		if(nIndex == -1)
			return NULL;    // must be able to convert
		return GetValueAt(nIndex);
	}
};


class CRegistryVirtualMachine : public IRegistrarBase
{
public:

	HRESULT STDMETHODCALLTYPE QueryInterface(const IID &riid,void ** ppv)
	{
		if (ppv == NULL)
			return E_POINTER;
		*ppv = NULL;
		if (IsEqualGUID(riid, __uuidof(IUnknown)) || IsEqualGUID(riid, __uuidof(IRegistrarBase)) )
		{
			*ppv = this;
			return S_OK;
		}
		return E_NOINTERFACE;
	}
	ULONG STDMETHODCALLTYPE AddRef(void) { return 1L; }

	ULONG STDMETHODCALLTYPE Release(void) {	return 1L; }
	
#ifndef _UNICODE
	virtual HRESULT STDMETHODCALLTYPE AddReplacement(LPCOLESTR key, LPCOLESTR item)
	{
		HRESULT hr = E_OUTOFMEMORY;
		_ATLTRY
		{
			hr = AddReplacement(COLE2T(key), COLE2T(item));
		}
		_ATLCATCHALL()
		{
		}
		return hr;
	}
#endif

public:
	~CRegistryVirtualMachine() throw()
	{
		ClearReplacements();
	}

	// Operations
	HRESULT  AddStandardReplacements() throw()
	{
		USES_CONVERSION;

		TCHAR szModule[MAX_PATH];

		DWORD dwFLen = GetModuleFileName(_AtlBaseModule.GetModuleInstance(), szModule, MAX_PATH);
		if( dwFLen == 0 )
			return AtlHresultFromLastError();
		else if( dwFLen == MAX_PATH )
			return HRESULT_FROM_WIN32(ERROR_INSUFFICIENT_BUFFER);

		// Convert to short path to work around bug in NT4's CreateProcess
		TCHAR szModuleShort[MAX_PATH];
		GetShortPathName(szModule, szModuleShort, MAX_PATH);
//              LPOLESTR pszModule = T2OLE(szModuleShort);

//              int nLen = ocslen(pszModule);
//              LPOLESTR pszModuleQuote = (LPOLESTR)alloca((nLen*2+1)*sizeof(OLECHAR));
//              ReplaceSingleQuote(pszModuleQuote, pszModule);
		HRESULT hr = AddReplacement(_T("Module"), szModule);
		if (FAILED(hr))
			return hr;
		OLECHAR* sz;
		hr = StringFromCLSID(CAtlModule::m_libid, &sz);
		if (FAILED(hr))
			return hr;
		hr = AddReplacement(_T("MODULEGUID"), OLE2T(sz));
		CoTaskMemFree(sz);
		if (FAILED(hr))
			return hr;
		SHFILEINFO shf;
		if (SHGetFileInfo(szModule, 0, &shf, sizeof(shf), SHGFI_EXETYPE) == 0)
			hr = AddReplacement(_T("MODULETYPE"), _T("InprocServer32"));
		else
			hr = AddReplacement(_T("MODULETYPE"), _T("LocalServer32"));

		if (SUCCEEDED(hr))
			hr = _pAtlModule->AddCommonRGSReplacements(this);
		return hr;
	}

	virtual HRESULT STDMETHODCALLTYPE AddReplacement(LPCTSTR lpszKey, LPCTSTR lpszItem) throw()
	{
		ATLASSERT(lpszKey != NULL && lpszItem != NULL);
		if (lpszKey == NULL || lpszItem == NULL)
			return E_INVALIDARG;

		HRESULT hr = E_OUTOFMEMORY; // If any of the new's fail.

		m_csMap.Lock();
		DWORD cb = lstrlen(lpszKey) + 1;
		CAutoVectorPtr<TCHAR> szNewKey;
		if (szNewKey.Allocate(cb))
		{			
			lstrcpy(szNewKey, lpszKey);
			cb = lstrlen(lpszItem) + 1;
			CAutoVectorPtr<TCHAR> szNewItem;
			if (szNewItem.Allocate(cb))
			{
				lstrcpy(szNewItem, lpszItem);
				if (m_RepMap.Add(szNewKey, szNewItem))
				{
					hr = S_OK;
					szNewKey.Detach();
					szNewItem.Detach();
				}
			}
		}
		m_csMap.Unlock();
		return hr;
	}

	HRESULT STDMETHODCALLTYPE ClearReplacements() throw()
	{
		m_csMap.Lock();
		HRESULT hr = S_OK;
		for (int i = 0; i < m_RepMap.GetSize(); i++)
		{
			delete [] m_RepMap.m_aKey[i];
			delete [] m_RepMap.m_aVal[i];
		}
		m_RepMap.RemoveAll();
		m_csMap.Unlock();
		return hr;
	}

	HRESULT VMUpdateRegistry(RGSOps* pOps, RGSStrings* rgStrings, RGSDWORD* rgDWORDS, RGSBinary *rgBinary, BOOL bRegister) throw()
	{
		ATLASSERT(pOps != NULL && rgStrings != NULL);
		if (pOps == NULL || rgStrings == NULL)
			return E_INVALIDARG;
		DWORD code, p1, p2;
		GetOpsFromDWORD(*pOps, code, p1, p2);

		if (code != rgsopOpenRootKey)
		{
			ATLASSERT(FALSE); // Bad parse tree
			return E_FAIL;
		}
		HKEY hKeyBase = (( HKEY ) (ULONG_PTR)((LONG)(p1 | 0x80000000)));
		pOps++;
		return VMUpdateRegistryRecurse(hKeyBase, pOps, rgStrings, rgDWORDS, rgBinary, bRegister);
	}

	// Implementation
	inline BYTE ChToByte(const TCHAR ch)
	{
		switch (ch)
		{
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
					return (BYTE) (ch - '0');
			case 'A':
			case 'B':
			case 'C':
			case 'D':
			case 'E':
			case 'F':
					return (BYTE) (10 + (ch - 'A'));
			case 'a':
			case 'b':
			case 'c':
			case 'd':
			case 'e':
			case 'f':
					return (BYTE) (10 + (ch - 'a'));
			default:
					ATLASSERT(FALSE);
					ATLTRACE(atlTraceRegistrar, 0, _T("Bogus value %c passed as binary Hex value\n"), ch);
					return 0;
		}
	}

	HRESULT GetStringAtLoc(RGSStrings* rgStrings, DWORD iLoc, CSimpleArray<TCHAR>& rgBytes, LPTSTR* pszCur) throw()
	{
		if (pszCur == NULL)
			return E_INVALIDARG;

		LPTSTR& szCur = *pszCur;
		szCur = rgStrings[iLoc].sz;
		if (szCur == NULL)
			return S_FALSE;

		if (rgStrings[iLoc].bHasReplacement)
		{
			rgBytes.RemoveAll();
			LPTSTR szTemp = szCur;
			szCur = NULL;
			while (*szTemp != NULL) // look for end
			{
				if (*szTemp == _T('%'))
				{
					szTemp = CharNext(szTemp);
					if (*szTemp == _T('%'))
						rgBytes.Add(*szTemp);
					else
					{
						LPTSTR lpszNext = _tcschr(szTemp, _T('%'));
						if (lpszNext == NULL)
						{
							ATLTRACE(atlTraceRegistrar, 0, _T("Error : closing '%' found\n"));
							return E_FAIL;
						}
						int nLength = (int)(INT_PTR)(lpszNext - szTemp);
						if (nLength > 31)
							return E_FAIL;
						TCHAR buf[32];
						lstrcpyn(buf, szTemp, nLength+1);
						LPTSTR lpszVar = m_RepMap.Lookup(buf);
						if (lpszVar == NULL)
							return E_FAIL;
						for (int iMapped = 0; lpszVar[iMapped] != NULL; iMapped++)
							rgBytes.Add(lpszVar[iMapped]);
						szTemp = lpszNext;
					}
				}
				else
					rgBytes.Add(*szTemp);
				szTemp = CharNext(szTemp);
			}
			rgBytes.Add(*szTemp);
		}
		return S_OK;
	}
	HRESULT GetDWORDAtLoc(RGSDWORD* rgDWORDS, DWORD iLoc, DWORD& dwValueOrIndex) throw()
	{
		dwValueOrIndex = rgDWORDS[iLoc].dwValueOrIndex;
		return rgDWORDS[iLoc].bHasReplacement == TRUE ? S_FALSE : S_OK;
	}

	HRESULT GetBinaryAtLoc(RGSBinary* rgBinary, DWORD iLoc, BYTE** ppValue, DWORD* pdwLen) throw()
	{
		*ppValue = rgBinary[iLoc].pBytes;
		*pdwLen = rgBinary[iLoc].dwLenOrIndex;
		return rgBinary[iLoc].bHasReplacement == TRUE ? S_FALSE : S_OK;
	}

	HRESULT DeleteKeyWithReplacement(HKEY hKeyParent, DWORD iString, RGSStrings* rgStrings) throw()
	{
		CRegKey rkForceRemove;
		CSimpleArray<TCHAR> rgBytes;
		LPTSTR szReplacement = NULL;
		HRESULT hr = GetStringAtLoc(rgStrings, (DWORD)iString, rgBytes, &szReplacement);
		if (FAILED(hr))
			return hr;
		rkForceRemove.Attach(hKeyParent);
		LONG lRet = rkForceRemove.RecurseDeleteKey((szReplacement) ? szReplacement : rgBytes.m_aT);
		if (lRet != ERROR_SUCCESS && lRet != ERROR_FILE_NOT_FOUND && lRet != ERROR_PATH_NOT_FOUND)
		{
			ATLTRACE(atlTraceRegistrar, 0, _T("Failed to delete key %s or one of its subkeys\n"),
				(szReplacement) ? szReplacement : rgBytes.m_aT);
			hr = AtlHresultFromWin32(lRet);
		}
		rkForceRemove.Detach();
		return hr;
	}

	HRESULT AddKeyWithReplacement(HKEY hKeyParent, CRegKey& rkCur, DWORD iString, RGSStrings* rgStrings) throw()
	{
		LPTSTR szReplacement = NULL;
		CSimpleArray<TCHAR> rgBytes;
		HRESULT hr = GetStringAtLoc(rgStrings, (DWORD)iString, rgBytes, &szReplacement);
		if (FAILED(hr))
			return hr;

		if (rkCur.Open(hKeyParent, (szReplacement) ? szReplacement : rgBytes.m_aT, 
					   KEY_READ | KEY_WRITE) != ERROR_SUCCESS)
		{
			LONG lRes = rkCur.Create(hKeyParent, (szReplacement) ? szReplacement : rgBytes.m_aT, REG_NONE, REG_OPTION_NON_VOLATILE , KEY_READ | KEY_WRITE); 
			if (lRes != ERROR_SUCCESS)
				return AtlHresultFromWin32(lRes);
		}
		return S_OK;
	}

	HRESULT VMUpdateRegistryRecurse(HKEY hKeyParent, RGSOps*& pOps, RGSStrings* rgStrings, RGSDWORD* rgDWORDS, RGSBinary *rgBinary, BOOL bRegister) throw()
	{
		CRegKey  rkCur;
		LONG     lRes = ERROR_SUCCESS;
		HRESULT  hr = S_OK;
		bool bNoRemove = false;
		RGSOps opParent = 0;
		DWORD code, p1, p2;

		GetOpsFromDWORD(*pOps, code, p1, p2);
		while (code != rgsopCloseAsParent)
		{
			switch(code)
			{
			case rgsopEndOfStream:
				return S_OK;
			case rgsopAddKeyNoRemove:
				bNoRemove = true;
				if (bRegister)
				{
					hr = AddKeyWithReplacement(hKeyParent, rkCur, p1, rgStrings);
					if (FAILED(hr))
						return hr;
				}
				pOps++; // NoRemove means do nothing during Unreg
				GetOpsFromDWORD(*pOps, code, p1, p2);
				break;
			case rgsopAddKeyForceRemove:
				if (bRegister) 
				{
					hr = DeleteKeyWithReplacement(hKeyParent, p1, rgStrings);
					if (FAILED(hr))
						return hr;
				}
				// fall through to normal add functionality
			case rgsopAddKey:
			{
				if (!bRegister)
				{
					LPTSTR szReplacement = NULL;
					CSimpleArray<TCHAR> rgBytes;
					hr = GetStringAtLoc(rgStrings, p1, rgBytes, &szReplacement);
					if (FAILED(hr))
						return hr;

					lRes = rkCur.Open(hKeyParent, (szReplacement) ? szReplacement : rgBytes.m_aT, KEY_READ | KEY_WRITE);
					if (lRes != ERROR_SUCCESS && lRes != ERROR_FILE_NOT_FOUND)
						return AtlHresultFromWin32(lRes); // Ignore if it is not in the registry and continue.
					opParent = *pOps; // remember this for the delete
				}
				else
				{
					hr = AddKeyWithReplacement(hKeyParent, rkCur, p1, rgStrings);
					if (FAILED(hr))
						return hr;
				}
				pOps++;
				GetOpsFromDWORD(*pOps, code, p1, p2);
				break;
			}
			case rgsopDeleteKey:
				ATLASSERT(FALSE); // Not yet implemented
				break;
			case rgsopOpenAsParent:
				pOps++;
				hr = VMUpdateRegistryRecurse(rkCur.m_hKey, pOps, rgStrings, rgDWORDS, rgBinary, bRegister);
				if (FAILED(hr))
					return hr;
				GetOpsFromDWORD(*pOps, code, p1, p2);
				if (!bRegister)
				{
					if (!bNoRemove)
					{
						DWORD codeTemp, p1Temp, p2Temp;
						GetOpsFromDWORD(opParent, codeTemp, p1Temp, p2Temp);
						hr = DeleteKeyWithReplacement(hKeyParent, p1Temp, rgStrings);
						if (FAILED(hr))
							return hr;
					}
					else
						bNoRemove = false; // turn it back off
				}
				break;
			case rgsopCloseAsParent:
				ATLASSERT(FALSE); // Should never get here it is the governing loop constraint
				return E_FAIL;
			case rgsopOpenRootKey:
			{
				HKEY hKeyBase = (( HKEY ) (ULONG_PTR)((LONG)(p1 | 0x80000000)));
				pOps++;
				return VMUpdateRegistryRecurse(hKeyBase, pOps, rgStrings, rgDWORDS, rgBinary, bRegister);
				break;
			}
			case rgsopAddStringValue:
			{
				if (bRegister)
				{
					bool bRestoreRK = false;
					if (rkCur.m_hKey == NULL)
					{
						rkCur.m_hKey = hKeyParent;
						bRestoreRK = true;
					}
					CSimpleArray<TCHAR> rgBytes, rgBytes2;
					LPTSTR szReplacement = NULL;
					LPTSTR szReplacement2 = NULL;
					hr = GetStringAtLoc(rgStrings, p2, rgBytes, &szReplacement);
					if (SUCCEEDED(hr) && p1 != 0)
						hr = GetStringAtLoc(rgStrings, p1, rgBytes2, &szReplacement2);

					CRegKey key;
					if (p1 == 0)
						key.m_hKey = rkCur.m_hKey;
					else
						key.m_hKey = hKeyParent;
					lRes = key.SetStringValue((p1 != 0) ? ((szReplacement2) ? szReplacement2 : rgBytes2.m_aT) : NULL, 
						(szReplacement) ? szReplacement : rgBytes.m_aT); 
					key.m_hKey = NULL;
					if (ERROR_SUCCESS != lRes)
						return AtlHresultFromWin32(lRes);
					if (bRestoreRK)
						rkCur.m_hKey = NULL;
					ATLTRACE(atlTraceRegistrar, 2, _T("Setting Value %s at %s\n"), (szReplacement) ? szReplacement : rgBytes.m_aT, (p1 != 0) ? (szReplacement2) ? szReplacement2 : rgBytes2.m_aT : _T("default"));
				}
				pOps++;
				GetOpsFromDWORD(*pOps, code, p1, p2);
				break;
			}
			case rgsopAddMultiStringValue:
			{
				if (bRegister)
				{
					bool bRestoreRK = false;
					if (rkCur.m_hKey == NULL)
					{
						rkCur.m_hKey = hKeyParent;
						bRestoreRK = true;
					}
					CSimpleArray<TCHAR> rgBytes, rgBytes2;
					LPTSTR szReplacement = NULL;
					LPTSTR szReplacement2 = NULL;
					hr = GetStringAtLoc(rgStrings, p2, rgBytes, &szReplacement);
					if (SUCCEEDED(hr) && p1 != 0)
						hr = GetStringAtLoc(rgStrings, p1, rgBytes2, &szReplacement2);

					ATLTRACE(atlTraceRegistrar, 2, _T("Setting Value %s at %s\n"), (szReplacement) ? szReplacement : rgBytes.m_aT, (p1 != 0) ? (szReplacement2) ? szReplacement2 : rgBytes2.m_aT : _T("default"));

					TCHAR* pszValue = (szReplacement) ? szReplacement : rgBytes.m_aT;
					int nLen = lstrlen(pszValue);
					CTempBuffer <TCHAR, 1024> pszDestValue;
					ATLTRY(pszDestValue.Allocate(nLen));
					if (pszDestValue != NULL)
					{
						TCHAR* p = pszDestValue;
						TCHAR* q = pszValue;
						nLen = 0;
						while (*q != NULL)
						{
							TCHAR* r = CharNext(q);
							if (*q == '\\' && *r == '0')
							{
								*p++ = NULL;
								q = CharNext(r);
							}
							else
							{
								*p = *q;
		#ifndef _UNICODE
								if (IsDBCSLeadByte(*q))
								{
									p++;
									q++;
									*p = *q;
								}
		#endif
								p++;
								q++;
							}
							nLen ++;
						}
						*p = NULL;

						CRegKey key;
						if (p1 == 0)
							key.m_hKey = rkCur.m_hKey;
						else
						key.m_hKey = hKeyParent;						
						lRes = key.SetMultiStringValue((p1 != 0) ? ((szReplacement2) ? szReplacement2 : rgBytes2.m_aT) : NULL, 
							pszDestValue); 
						key.m_hKey = NULL;
						if (ERROR_SUCCESS != lRes)
							return AtlHresultFromWin32(lRes);
						if (bRestoreRK)
							rkCur.m_hKey = NULL;
					}
					else
					{
						return E_OUTOFMEMORY;
					}
				}
				pOps++;
				GetOpsFromDWORD(*pOps, code, p1, p2);
				break;
			}

			case rgsopAddDWORDValue:
			{
				if (bRegister)
				{
					bool bRestoreRK = false;
					if (rkCur.m_hKey == NULL)
					{
						rkCur.m_hKey = hKeyParent;
						bRestoreRK = true;
					}
					CSimpleArray<TCHAR> rgBytes, rgBytes2;
					LPTSTR szReplacement = NULL;
					LPTSTR szReplacement2 = NULL;
					DWORD dwValue;
					hr = GetDWORDAtLoc(rgDWORDS, p2, dwValue);
					if (hr == S_FALSE)
					{
						GetStringAtLoc(rgStrings, dwValue, rgBytes, &szReplacement);
						USES_CONVERSION;
						hr = VarUI4FromStr(T2OLE(szReplacement ? szReplacement : rgBytes.m_aT), 0, 0, &dwValue);
						if (FAILED(hr))
						{
							// Replacement value exceeds max DWORD value
							return E_FAIL;
						}
					}

					if (SUCCEEDED(hr) && p1 != 0)
						hr = GetStringAtLoc(rgStrings, p1, rgBytes2, &szReplacement2);

					CRegKey key;
					if (p1 == 0)
						key.m_hKey = rkCur.m_hKey;
					else
						key.m_hKey = hKeyParent;						
					lRes = key.SetDWORDValue((szReplacement2) ? szReplacement2 : rgBytes2.m_aT, 
						dwValue);
					key.m_hKey = NULL; 
					if (ERROR_SUCCESS != lRes)
						return AtlHresultFromWin32(lRes);
					if (bRestoreRK)
						rkCur.m_hKey = NULL;
					ATLTRACE(atlTraceRegistrar, 2, _T("Setting Value %s at %s\n"), (szReplacement) ? szReplacement : rgBytes.m_aT, (p1 != 0) ? (szReplacement2) ? szReplacement2 : rgBytes2.m_aT : _T("default"));
				}
				pOps++;
				GetOpsFromDWORD(*pOps, code, p1, p2);
				break;
			}
			case rgsopAddBinaryValue:
			{
				if (bRegister)
				{
					bool bRestoreRK = false;
					if (rkCur.m_hKey == NULL)
					{
						rkCur.m_hKey = hKeyParent;
						bRestoreRK = true;
					}
					CSimpleArray<TCHAR> rgBytes, rgBytes2;
					LPTSTR szReplacement = NULL;
					LPTSTR szReplacement2 = NULL;
					BYTE* pByte;
					CTempBuffer <BYTE, 1024> pByteTemp;					
					DWORD dwLen;
					hr = GetBinaryAtLoc(rgBinary, p2, &pByte, &dwLen);
					if (hr == S_FALSE)
					{
						GetStringAtLoc(rgStrings, dwLen, rgBytes, &szReplacement);
						if (szReplacement == NULL)
						{
							szReplacement = rgBytes.m_aT;
						}
						int cbValue = lstrlen(szReplacement);
						if (cbValue & 0x00000001)
						{
							ATLTRACE(atlTraceRegistrar, 0, _T("Binary Data does not fall on BYTE boundries\n"));
							return E_FAIL;
						}
						dwLen = cbValue/2;
						ATLTRY(pByteTemp.Allocate(dwLen));
						if (pByteTemp == NULL)
						{
							return E_OUTOFMEMORY;
						}						
						pByte = pByteTemp;						
						memset(pByte, 0, dwLen);
						for (int irg = 0; irg < cbValue; irg++)
							pByte[(irg/2)] |= (ChToByte(szReplacement[irg])) << (4*(1 - (irg & 0x00000001)));
					}

					if (SUCCEEDED(hr) && p1 != 0)
						hr = GetStringAtLoc(rgStrings, p1, rgBytes2, &szReplacement2);

					CRegKey key;
					if (p1 == 0)
						key.m_hKey = rkCur.m_hKey;
					else
						key.m_hKey = hKeyParent;	
					lRes = key.SetBinaryValue((szReplacement2) ? szReplacement2 : rgBytes2.m_aT, 
						pByte, dwLen);
					key.m_hKey = NULL; 
					if (ERROR_SUCCESS != lRes)
						return AtlHresultFromWin32(lRes);
					if (bRestoreRK)
						rkCur.m_hKey = NULL;
					ATLTRACE(atlTraceRegistrar, 2, _T("Setting Value %s at %s\n"), (szReplacement) ? szReplacement : rgBytes.m_aT, (p1 != 0) ? (szReplacement2) ? szReplacement2 : rgBytes2.m_aT : _T("default"));
				}
				pOps++;
				GetOpsFromDWORD(*pOps, code, p1, p2);
				break;
			}


			default:
				ATLASSERT(FALSE); // Should never get here
				break;
			};
		}
		pOps++;
		GetOpsFromDWORD(*pOps, code, p1, p2);
		return hr;
	}

	void GetOpsFromDWORD(RGSOps op, DWORD& rcode, DWORD& rp1, DWORD& rp2) throw()
	{
		rcode = (op & 0xF0000000) >> 28;
		rp1 = (op & 0x0FFFC000) >> 14;
		rp2 = (op & 0x00003FFF);
	}


	CVMExpansionVector								m_RepMap;
	CComObjectThreadModel::AutoCriticalSection      m_csMap;
};


struct _RDXEntries
{
	enum KeyType
	{
		keyTypeString,
		keyTypeCString,
		keyTypeDWORD,
		keyTypeBinary,
		keyTypeNoEntry
	};

	HKEY iRootKey;
	LPTSTR szSubKey;
	LPTSTR szValue;
	enum KeyType nKey;
	void* pMember;
	DWORD cb;
};

#define BEGIN_RDX_MAP() \
	ATL::_RDXEntries* _GetRDXEntries() \
	{ \
		static ATL::_RDXEntries rgEntries [] = { 

#define RDX_TEXT(rootkey, subkey, valuename, member, member_size) \
{ rootkey, subkey, valuename, ATL::_RDXEntries::keyTypeString, member, member_size },

#define RDX_CSTRING_TEXT(rootkey, subkey, valuename, member, member_size) \
{ rootkey, subkey, valuename, ATL::_RDXEntries::keyTypeCString, &member, member_size },

#define RDX_DWORD(rootkey, subkey, valuename, member, member_size) \
{ rootkey, subkey, valuename, ATL::_RDXEntries::keyTypeDWORD, &member, member_size },

#define RDX_BINARY(rootkey, subkey, valuename, member, member_size) \
{ rootkey, subkey, valuename, ATL::_RDXEntries::keyTypeBinary, member, member_size },

#define END_RDX_MAP() \
		{NULL, NULL, NULL, ATL::_RDXEntries::keyTypeNoEntry, NULL, 0} };\
		return rgEntries; \
	} \
	HRESULT RegistryDataExchange(enum ATL::RDXOperations rdxOp, void* pItem = NULL) \
	{ \
		return ::RegistryDataExchange(this, rdxOp, pItem); \
	}

class CByteFilter 
{
public:

	static BYTE ChToByte(const TCHAR ch)
	{
		switch (ch)
		{
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
					return (BYTE) (ch - '0');
			case 'A':
			case 'B':
			case 'C':
			case 'D':
			case 'E':
			case 'F':
					return (BYTE) (10 + (ch - 'A'));
			case 'a':
			case 'b':
			case 'c':
			case 'd':
			case 'e':
			case 'f':
					return (BYTE) (10 + (ch - 'a'));
			default:
					ATLASSERT(FALSE);
					ATLTRACE(atlTraceRegistrar, 0, _T("Bogus value %c passed as binary Hex value\n"), ch);
					return 0;
		}
	}
};

template <class T>
HRESULT RegistryDataExchange(T* pT, enum RDXOperations rdxOp, void* pItem = NULL)
{
	_RDXEntries* pEntries = pT->_GetRDXEntries();
	ATLASSERT(pEntries != NULL);
	LONG lRes = ERROR_SUCCESS;

	while (pEntries->nKey != _RDXEntries::keyTypeNoEntry)
	{
		ATLASSERT(pEntries->pMember != NULL); 
		if (pItem == NULL || pItem == pEntries->pMember) 
		{ 
			if (rdxOp == eDeleteFromReg)
			{
				if (pEntries->szSubKey != NULL)
				{
					if (pEntries->szValue != NULL)
					{
						CRegKey rkDel;
						lRes = rkDel.Open(pEntries->iRootKey, pEntries->szSubKey, KEY_READ | KEY_WRITE);
						if (lRes != ERROR_SUCCESS)
						{
							ATLTRACE(atlTraceRegistrar, 0, _T("Failed to Open key in order to delete Value\n"));
							return AtlHresultFromWin32(lRes);
						}
						lRes = rkDel.DeleteValue(pEntries->szValue);
						if (lRes != ERROR_SUCCESS)
						{
							ATLTRACE(atlTraceRegistrar, 0, _T("Failed to delete Value\n"));
							return AtlHresultFromWin32(lRes);
						}
					}
					else
					{
						lRes = RegDeleteKey(pEntries->iRootKey, pEntries->szSubKey);
						if (lRes != ERROR_SUCCESS)
						{
							ATLTRACE(atlTraceRegistrar, 0, _T("RegDeleteKey failed\n"));
							return AtlHresultFromWin32(lRes);
						}
					}
				}
				else
				{
					if (pEntries->szValue)
					{
						lRes = RegDeleteValue(pEntries->iRootKey, pEntries->szValue);
						if (lRes != ERROR_SUCCESS)
						{
							ATLTRACE(atlTraceRegistrar, 0, _T("RegDeleteValue failed\n"));
							return AtlHresultFromWin32(lRes);
						}
					}
				}
				pEntries++;
				continue;
			}

			CRegKey rk; 
			REGSAM samDesired = KEY_READ;
			if (rdxOp == eWriteToReg)
				samDesired |= KEY_WRITE;
			lRes = rk.Open(pEntries->iRootKey, pEntries->szSubKey, samDesired); 
			if (lRes != ERROR_SUCCESS && rdxOp == eReadFromReg) 
				return AtlHresultFromWin32(lRes);
			else 
			{
				if (lRes != ERROR_SUCCESS) 
				{ 
					lRes = rk.Create(pEntries->iRootKey, pEntries->szSubKey, REG_NONE, REG_OPTION_NON_VOLATILE, KEY_WRITE | KEY_READ); 
					if (lRes != ERROR_SUCCESS) 
						return AtlHresultFromWin32(lRes);
				} 
			} 
			DWORD dwRet = pEntries->cb;
			switch(pEntries->nKey)
			{
			case _RDXEntries::keyTypeString:
				if (rdxOp == eWriteToReg) 
					lRes = rk.SetStringValue(pEntries->szValue, (LPCTSTR)pEntries->pMember); 
				else 
					lRes= rk.QueryStringValue(pEntries->szValue, (LPTSTR)pEntries->pMember, &dwRet); 
				break;
			case _RDXEntries::keyTypeCString:
				{
					CString& rStr = *((CString*)pEntries->pMember);
					if (rdxOp == eWriteToReg)
						lRes = rk.SetStringValue(pEntries->szValue, rStr); 
					else
					{
						lRes = rk.QueryStringValue(pEntries->szValue, NULL, &dwRet);
						if (lRes != ERROR_SUCCESS)
						{
							ATLTRACE(atlTraceRegistrar, 0, _T("Unable to determine keys length\n"));
							return AtlHresultFromWin32(lRes);
						}

						lRes= rk.QueryStringValue(pEntries->szValue, rStr.GetBuffer(dwRet), &dwRet); 
						rStr.ReleaseBuffer();
						if (lRes != ERROR_SUCCESS)
						{
							ATLTRACE(atlTraceRegistrar, 0, _T("Unable to copy keys data to CString\n"));
							return AtlHresultFromWin32(lRes);
						}
					}
				}
				break;
			case _RDXEntries::keyTypeDWORD:
				{
					DWORD& rdw = *((DWORD*)pEntries->pMember);
					if (rdxOp == eWriteToReg) 
						lRes = rk.SetDWORDValue(pEntries->szValue, rdw); 
					else 
						lRes= rk.QueryDWORDValue(pEntries->szValue, rdw); 
					break;
				}
			case _RDXEntries::keyTypeBinary:
				if (rdxOp == eReadFromReg)
				{
					DWORD dwType = REG_BINARY;
					DWORD dwCount = pEntries->cb;
					LONG lRes = RegQueryValueEx(rk, pEntries->szValue, NULL, &dwType, (LPBYTE)pEntries->pMember, &dwCount);
					if (lRes != ERROR_SUCCESS)
					{
						ATLTRACE(atlTraceRegistrar, 0, _T("Failed to query binary value\n"));
						return AtlHresultFromWin32(lRes);
					}
					ATLASSERT(dwType == REG_BINARY);
					break;
				}
				else
				{
					lRes = RegSetValueEx(rk, pEntries->szValue, 0, REG_BINARY, (const BYTE *)pEntries->pMember, pEntries->cb);
					break;
				}
			case _RDXEntries::keyTypeNoEntry:
				ATLASSERT(FALSE);  // Not yet implemented
				break;
			};
			if (pItem != NULL)
				return AtlHresultFromWin32(lRes); 
		}
		pEntries++;
	}
	return AtlHresultFromWin32(lRes); 
}
}; // namespace ATL

#endif // __ATLPLUS_H__
