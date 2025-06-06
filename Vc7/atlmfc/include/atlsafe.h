// This is a part of the Active Template Library.
// Copyright (C) 1996-2001 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Active Template Library Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Active Template Library product.

#ifndef __ATLSAFE_H__
#define __ATLSAFE_H__

#pragma once

#include <atlbase.h>

namespace ATL
{

// This struct is used with CComSafeArray to set the matching VARTYPE based on
// template argument type passed in to CComSafeArray.
template <typename T>
struct _ATL_AutomationType
{
};

#define DEFINE_AUTOMATION_TYPE_FUNCTION(ctype, typewrapper, oleautomationtype) \
	template <> \
	struct _ATL_AutomationType<ctype> \
	{ \
		typedef typewrapper _typewrapper;\
		enum { type = oleautomationtype }; \
		static void* GetT(const T& t) throw() \
		{ \
			return (void*)&t; \
		} \
	};
	// specialization for BSTR so GetT doesn't return &BSTR
	template <>
	struct _ATL_AutomationType<BSTR>
	{
		typedef CComBSTR _typewrapper ;
		enum { type = VT_BSTR};
		static void* GetT(const T& t) throw()
		{
			return t;
		}
	};
	// specialization for LPUNKNOWN so GetT doesn't return &LPUNKNOWN
	template <>
	struct _ATL_AutomationType<LPUNKNOWN>
	{
		typedef CComPtr<IUnknown> _typewrapper;
		enum { type = VT_UNKNOWN};
		static void* GetT(const T& t) throw()
		{
			return t;
		}
	};
	// specialization for LPDISPATCH so GetT doesn't return &LPDISPATCH
	template <>
	struct _ATL_AutomationType<LPDISPATCH>
	{
		typedef CComPtr<IDispatch> _typewrapper;
		enum { type = VT_DISPATCH};
		static void* GetT(const T& t) throw()
		{
			return t;
		}
	};

	DEFINE_AUTOMATION_TYPE_FUNCTION(CHAR		, CHAR			,VT_I1)
	DEFINE_AUTOMATION_TYPE_FUNCTION(SHORT		, SHORT			,VT_I2)
	DEFINE_AUTOMATION_TYPE_FUNCTION(INT			, INT			,VT_I4)
	DEFINE_AUTOMATION_TYPE_FUNCTION(LONG		, LONG			,VT_I4)
	DEFINE_AUTOMATION_TYPE_FUNCTION(LONGLONG	, LONGLONG		,VT_I8)
	DEFINE_AUTOMATION_TYPE_FUNCTION(BYTE		, BYTE			,VT_UI1)
	DEFINE_AUTOMATION_TYPE_FUNCTION(USHORT		, USHORT		,VT_UI2)
	DEFINE_AUTOMATION_TYPE_FUNCTION(UINT		, UINT			,VT_UI4)
	DEFINE_AUTOMATION_TYPE_FUNCTION(ULONG		, ULONG			,VT_UI4)
	DEFINE_AUTOMATION_TYPE_FUNCTION(ULONGLONG	, ULONGLONG		,VT_UI8)
	DEFINE_AUTOMATION_TYPE_FUNCTION(FLOAT		, FLOAT			,VT_R4)
	DEFINE_AUTOMATION_TYPE_FUNCTION(DOUBLE		, DOUBLE		,VT_R8)
	DEFINE_AUTOMATION_TYPE_FUNCTION(DECIMAL		, DECIMAL		,VT_DECIMAL)
	DEFINE_AUTOMATION_TYPE_FUNCTION(VARIANT		, CComVariant	,VT_VARIANT)
	DEFINE_AUTOMATION_TYPE_FUNCTION(CY			, CY			,VT_CY)

// wrapper for SAFEARRAYBOUND used with CComSafeArray
class CComSafeArrayBound : public SAFEARRAYBOUND
{
public:
	CComSafeArrayBound(ULONG ulCount = 0, LONG lLowerBound = 0) throw()
	{
		cElements = ulCount;
		lLbound = lLowerBound;
	}
	CComSafeArrayBound& operator=(const CComSafeArrayBound& bound) throw()
	{
		cElements = bound.cElements;
		lLbound = bound.lLbound;
		return *this;
	}
	CComSafeArrayBound& operator=(ULONG ulCount) throw() 
	{
		cElements = ulCount;
		lLbound = 0;
		return *this;
	}
	ULONG GetCount() const throw()
	{
		return cElements;
	}
	ULONG SetCount(ULONG ulCount) throw()
	{
		cElements = ulCount;
		return cElements;
	}
	LONG GetLowerBound() const throw()
	{
		return lLbound;
	}
	LONG SetLowerBound(LONG lLowerBound) throw()
	{
		lLbound = lLowerBound;
		return lLbound;
	}
	LONG GetUpperBound() const throw()
	{
		return lLbound + cElements - 1;
	}
};

// wrapper for SAFEARRAY.  T is type stored (e.g. BSTR, VARIANT, etc.)
template <typename T, VARTYPE _vartype = _ATL_AutomationType<T>::type>
class CComSafeArray 
{
public:
// Constructors
	CComSafeArray() throw() : m_psa(NULL)
	{
	}
	// create SAFEARRAY where number of elements = ulCount
	explicit CComSafeArray(ULONG ulCount, LONG lLBound = 0) : m_psa(NULL)
	{
		CComSafeArrayBound bound(ulCount, lLBound);
		HRESULT hRes = Create(&bound);
		if (FAILED(hRes))
			AtlThrow(hRes);
	}
	explicit CComSafeArray(const SAFEARRAYBOUND& bound) : m_psa(NULL)
	{
		HRESULT hRes = Create(&bound); 
		if (FAILED(hRes))
			AtlThrow(hRes);
	}
	// pass an array of SAFEARRAYBOUNDs for multi-dimensional
	explicit CComSafeArray(const SAFEARRAYBOUND *pBound, UINT uDims = 1) : m_psa(NULL)
	{
		ATLASSERT(pBound != NULL);
		ATLASSERT(uDims > 0);
		HRESULT hRes = Create(pBound, uDims);
		if (FAILED(hRes))
			AtlThrow(hRes);
	}
	CComSafeArray(const SAFEARRAY *psaSrc) : m_psa(NULL)
	{ 
		ATLASSERT(psaSrc != NULL);
		HRESULT hRes = CopyFrom(psaSrc);
		if (FAILED(hRes))
			AtlThrow(hRes);
	}
	CComSafeArray(const SAFEARRAY& saSrc) : m_psa(NULL)
	{ 
		HRESULT hRes = CopyFrom(&saSrc);
		if (FAILED(hRes))
			AtlThrow(hRes);
	}
	CComSafeArray(const CComSafeArray& saSrc) : m_psa(NULL)
	{
		ATLASSERT(saSrc.m_psa != NULL);
		HRESULT hRes = CopyFrom(saSrc.m_psa);
		if (FAILED(hRes))
			AtlThrow(hRes);
	}

	~CComSafeArray() throw()
	{
		HRESULT hRes = Destroy();
		hRes;
		ATLASSERT(SUCCEEDED(hRes));
	} 

	HRESULT Attach(const SAFEARRAY *psaSrc) 
	{
		ATLASSERT(psaSrc != NULL);
		HRESULT	hRes = Destroy();
		ATLASSERT(SUCCEEDED(hRes));
		if (SUCCEEDED(hRes))
		{
			m_psa = const_cast<LPSAFEARRAY>(psaSrc);
			hRes = Lock();
		}
		return hRes;
	}
	LPSAFEARRAY Detach()
	{
		Unlock();
		LPSAFEARRAY pTemp = m_psa;
		m_psa = NULL;
		return pTemp;
	} 

// overloaded operators
	CComSafeArray<T>& operator=(const CComSafeArray& saSrc)
	{
		*this = saSrc.m_psa;
		return *this;
	}
	CComSafeArray<T>& operator=(const SAFEARRAY *psaSrc)
	{
		ATLASSERT(psaSrc != NULL);
		HRESULT hRes = CopyFrom(psaSrc);
		if (FAILED(hRes))
			AtlThrow(hRes);
		return *this;
	}
	operator const SAFEARRAY *() const throw()
	{
		return m_psa; 
	}
	operator LPSAFEARRAY() throw()
	{
		return m_psa; 
	}
	LPSAFEARRAY* GetSafeArrayPtr() throw()
	{
		return &m_psa;
	}
	const _ATL_AutomationType<T>::_typewrapper& operator[](int nIndex) const
	{
		return GetAt(nIndex);
	}
	_ATL_AutomationType<T>::_typewrapper& operator[](int nIndex)
	{
		return GetAt(nIndex);
	}
	const _ATL_AutomationType<T>::_typewrapper& operator[](LONG nIndex) const
	{
		return GetAt(nIndex);
	}
	_ATL_AutomationType<T>::_typewrapper& operator[](LONG nIndex)
	{
		return GetAt(nIndex);
	}

// info functions
	LONG GetLowerBound(UINT uDim = 0) const
	{
		ATLASSERT(m_psa != NULL);
		LONG lLBound = 0;
		HRESULT hRes = SafeArrayGetLBound(m_psa, uDim+1, &lLBound);
		if (FAILED(hRes))
			AtlThrow(hRes);
		return lLBound;
	}
	LONG GetUpperBound(UINT uDim = 0) const
	{
		ATLASSERT(m_psa != NULL);
		LONG lUBound = 0;
		HRESULT hRes = SafeArrayGetUBound(m_psa, uDim+1, &lUBound);
		if (FAILED(hRes))
			AtlThrow(hRes);
		return lUBound;
	}
	ULONG GetCount(UINT uDim = 0) const
	{
		ATLASSERT(m_psa != NULL);
		ATLASSERT(uDim < GetDimensions());
		LONG lLBound, lUBound;
		HRESULT hRes = SafeArrayGetLBound(m_psa, uDim+1, &lLBound);
		ATLASSERT(SUCCEEDED(hRes));
		hRes = SafeArrayGetUBound(m_psa, uDim+1, &lUBound);
		ATLASSERT(SUCCEEDED(hRes));
		return (lUBound - lLBound + 1);
	}
	UINT GetDimensions() const
	{
		ATLASSERT(m_psa != NULL);
		return SafeArrayGetDim(m_psa);
	}
	VARTYPE GetType() const throw()
	{
		return _vartype;
	}
	bool IsSizable() const
	{
		ATLASSERT(m_psa != NULL);
		return (m_psa->fFeatures & FADF_FIXEDSIZE) ? false : true;
	}

// miscellaneous functions
	const _ATL_AutomationType<T>::_typewrapper& GetAt(LONG lIndex) const
	{
		ATLASSERT(m_psa != NULL);
		LONG lLBound = GetLowerBound();
		ATLASSERT(lIndex >= lLBound);
		ATLASSERT(lIndex <= GetUpperBound());
		return ((_ATL_AutomationType<T>::_typewrapper*)m_psa->pvData)[lIndex-lLBound];
	}

	_ATL_AutomationType<T>::_typewrapper& GetAt(LONG lIndex)
	{
		ATLASSERT(m_psa != NULL);
		LONG lLBound = GetLowerBound();
		ATLASSERT(lIndex >= lLBound);
		ATLASSERT(lIndex <= GetUpperBound());
		return ((_ATL_AutomationType<T>::_typewrapper*)m_psa->pvData)[lIndex-lLBound];
	}
	HRESULT SetAt(LONG lIndex, const T& t, BOOL bCopy = TRUE)
	{
		bCopy;
		ATLASSERT(m_psa != NULL);
		LONG lLBound = GetLowerBound();
		ATLASSERT(lIndex >= lLBound);
		ATLASSERT(lIndex <= GetUpperBound());
		((T*)m_psa->pvData)[lIndex-lLBound] = t;
		return S_OK;
	}
	// multi-dimensional version
	HRESULT MultiDimGetAt(const LONG *alIndex, T& t)
	{
		ATLASSERT(m_psa != NULL);
		return SafeArrayGetElement(m_psa, const_cast<LONG*>(alIndex), _ATL_AutomationType<T>::GetT(t));
	}
	// multi-dimensional version
	HRESULT MultiDimSetAt(const LONG *alIndex, const T& t)
	{
		ATLASSERT(m_psa != NULL);
		return SafeArrayPutElement(m_psa, const_cast<LONG*>(alIndex), _ATL_AutomationType<T>::GetT(t));
	}
	// append an item
	HRESULT Add(const T& t, BOOL bCopy = TRUE)
	{
		HRESULT hRes = S_OK;
		if (NULL == m_psa)
		{
			hRes = Create();
			if (FAILED(hRes))
				return hRes;
		}
		ATLASSERT(GetDimensions() == 1); // not for multi-dimensional
		LONG lLBound = GetLowerBound();
		hRes = Resize(GetCount() + 1, lLBound);
		if (SUCCEEDED(hRes))
			return SetAt(GetCount() - 1 + lLBound, t, bCopy);
		else
			return hRes;
	}
	// appends an array of type T items
	HRESULT Add(ULONG ulCount, const T *pT, BOOL bCopy = TRUE)
	{
		ATLASSERT(pT != NULL);
		ATLASSERT(ulCount > 0);
		HRESULT hRes = S_OK;
		if (NULL == m_psa)
		{
			hRes = Create();
			if (FAILED(hRes))
				return hRes;
		}
		ATLASSERT(GetDimensions() == 1); // not for multi-dimensional
		ULONG ulLastIndex = GetCount();
		ULONG ulTotalCount = ulLastIndex + ulCount;
		LONG lLBound = GetLowerBound();
		hRes = Resize(ulTotalCount, lLBound);
		if (SUCCEEDED(hRes))
		{
			for (ULONG ulCntr=0; ulCntr<ulCount; ulCntr++, ulLastIndex++)
			{
				hRes = SetAt(ulLastIndex + lLBound, pT[ulCntr], bCopy);
				if (FAILED(hRes))
					break;
			}
		}
		return hRes;
	}
	// appends items in the safearray
	HRESULT Add(const SAFEARRAY *psaSrc)
	{
		ATLASSERT(psaSrc != NULL);
		// safearrays must only have one dimension
		ATLASSERT(SafeArrayGetDim(const_cast<LPSAFEARRAY>(psaSrc)) == 1);

		HRESULT hRes = S_OK;
		if (NULL == m_psa)
		{
			hRes = Create();
			if (FAILED(hRes))
				return hRes;
		}
		ATLASSERT(GetDimensions() == 1);

		// types must match
		VARTYPE vt;
		SafeArrayGetVartype(const_cast<LPSAFEARRAY>(psaSrc), &vt);
		ATLASSERT(GetType() == vt);

		// resize safearray
		ULONG ulCount = psaSrc->rgsabound[0].cElements;
		ULONG ulLastIndex = GetCount();
		ULONG ulTotalCount = ulLastIndex + ulCount;
		LONG lLBound = GetLowerBound();
		hRes = Resize(ulTotalCount, lLBound);
		if (SUCCEEDED(hRes))
		{
			CComSafeArray saTemp;
			hRes = saTemp.Attach(psaSrc);
			if (FAILED(hRes))
				return hRes;

			LONG lsrcLBound = saTemp.GetLowerBound();
			for(ULONG lCntr = 0; lCntr < ulCount; lCntr++, ulLastIndex++)
			{
				// copy data to end of our safearray
				hRes = SetAt(ulLastIndex + lLBound, saTemp.GetAt(lCntr + lsrcLBound));
				if (FAILED(hRes))
					break;
			}
			saTemp.Detach();
		}
		return hRes;
	}

	// Resize only resizes the right-most dimension
	HRESULT Resize(ULONG ulCount, LONG lLBound = 0)
	{
		ATLASSERT(m_psa != NULL);
		CComSafeArrayBound bound(ulCount, lLBound);
		return Resize(&bound);
	}
	// Resize only resizes the right-most dimension
	HRESULT Resize(const SAFEARRAYBOUND *pBound)
	{
		ATLASSERT(m_psa != NULL);
		ATLASSERT(pBound != NULL);
		if (!IsSizable())
			return E_FAIL;
		HRESULT hRes = Unlock();
		if (SUCCEEDED(hRes))
		{
			hRes = SafeArrayRedim(m_psa, const_cast<LPSAFEARRAYBOUND>(pBound));
			if (SUCCEEDED(hRes))
				hRes = Lock();
			else
				hRes = Lock();
		}
		return hRes;
	}
	HRESULT CopyFrom(const SAFEARRAY *psaSrc)
	{
		ATLASSERT(psaSrc != NULL);
		HRESULT hRes = Destroy();
		ATLASSERT(SUCCEEDED(hRes));
		if (SUCCEEDED(hRes))
			hRes = SafeArrayCopy(const_cast<LPSAFEARRAY>(psaSrc), &m_psa);
		if (m_psa && SUCCEEDED(hRes))
			hRes = Lock();
		return hRes;
	}
	HRESULT CopyTo(LPSAFEARRAY *ppArray)
	{
		ATLASSERT(m_psa != NULL);
		ATLASSERT(ppArray != NULL);
		HRESULT hRes = E_POINTER;
		if (ppArray != NULL)
			hRes = SafeArrayCopy(m_psa, ppArray);
		return hRes;
	}
	HRESULT Create(ULONG ulCount = 0, LONG lLBound = 0)
	{
		CComSafeArrayBound bound(ulCount, lLBound);
		return Create(&bound);
	}
	HRESULT Create(const SAFEARRAYBOUND *pBound, UINT uDims = 1)
	{
		ATLASSERT(m_psa == NULL);
		ATLASSERT(uDims > 0);
		HRESULT hRes = S_OK;
		m_psa = SafeArrayCreate(_vartype, uDims, const_cast<LPSAFEARRAYBOUND>(pBound));
		if (NULL == m_psa)
			hRes = E_OUTOFMEMORY;
		else
			hRes = Lock();
		return hRes;
	}
	HRESULT Destroy()
	{
		HRESULT hRes = S_OK;
		if (m_psa != NULL)
		{
			hRes = Unlock();
			if (SUCCEEDED(hRes))
			{
				hRes = SafeArrayDestroy(m_psa);
				if (SUCCEEDED(hRes))
					m_psa = NULL;
			}
		}
		return hRes;
	}
protected:
	HRESULT Lock()
	{
		ATLASSERT(m_psa != NULL);
		return SafeArrayLock(m_psa);
	}
	HRESULT Unlock()
	{
		ATLASSERT(m_psa != NULL);
		return SafeArrayUnlock(m_psa);
	}
public:
	LPSAFEARRAY m_psa;
};

template<>
HRESULT CComSafeArray<BSTR>::SetAt(LONG lIndex, const BSTR& strData, BOOL bCopy)
{
	ATLASSERT(strData != NULL);
	ATLASSERT(m_psa != NULL);
	LONG lLBound = GetLowerBound();
	ATLASSERT(lIndex >= lLBound);
	ATLASSERT(lIndex <= GetUpperBound());	

	BSTR strOrg = ((BSTR*)m_psa->pvData)[lIndex-lLBound];
	if (strOrg)
		::SysFreeString(strOrg);

	if (bCopy)
	{
		BSTR strTemp = ::SysAllocString(strData);
		if (NULL == strTemp)
			return E_OUTOFMEMORY;
		((BSTR*)m_psa->pvData)[lIndex-lLBound] = strTemp;
	}
	else
		((BSTR*)m_psa->pvData)[lIndex-lLBound] = strData;

	return S_OK;
}
template<>
HRESULT CComSafeArray<VARIANT>::SetAt(LONG lIndex, const VARIANT& varData, BOOL bCopy)
{
	ATLASSERT(m_psa != NULL);
	LONG lLBound = GetLowerBound();
	ATLASSERT(lIndex >= lLBound);
	ATLASSERT(lIndex <= GetUpperBound());

	if (bCopy)
		return VariantCopyInd(&((VARIANT*)m_psa->pvData)[lIndex-lLBound], const_cast<LPVARIANT>(&varData));
	else
	{	
		VARIANT varOrg = ((VARIANT*)m_psa->pvData)[lIndex-lLBound];
		if (V_VT(&varOrg) != VT_EMPTY)
			::VariantClear(&varOrg);
		((VARIANT*)m_psa->pvData)[lIndex-lLBound] = varData;
		return S_OK;
	}
}
template<>
HRESULT CComSafeArray<LPUNKNOWN>::SetAt(LONG lIndex, const LPUNKNOWN& pUnk, BOOL bAddRef)
{
	ATLASSERT(pUnk != NULL);
	ATLASSERT(m_psa != NULL);
	LONG lLBound = GetLowerBound();
	ATLASSERT(lIndex >= lLBound);
	ATLASSERT(lIndex <= GetUpperBound());
	LPUNKNOWN pOrgUnk = ((LPUNKNOWN*)m_psa->pvData)[lIndex-lLBound];
	if (pOrgUnk)
		pOrgUnk->Release();
	if (bAddRef)
		pUnk->AddRef();
	((LPUNKNOWN*)m_psa->pvData)[lIndex-lLBound] = pUnk;
	return S_OK;
}
template<>
HRESULT CComSafeArray<LPDISPATCH>::SetAt(LONG lIndex, const LPDISPATCH& pDisp, BOOL bAddRef)
{
	ATLASSERT(pDisp != NULL);
	ATLASSERT(m_psa != NULL);
	LONG lLBound = GetLowerBound();
	ATLASSERT(lIndex >= lLBound);
	ATLASSERT(lIndex <= GetUpperBound());
	LPDISPATCH pOrgDisp = ((LPDISPATCH*)m_psa->pvData)[lIndex-lLBound];
	if (pOrgDisp)
		pOrgDisp->Release();
	if (bAddRef)
		pDisp->AddRef();
	((LPDISPATCH*)m_psa->pvData)[lIndex-lLBound] = pDisp;
	return S_OK;
}

}; //namespace ATL

#endif //__ATLSAFE_H__
