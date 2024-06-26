// This file is based on the following file from the LZMA SDK (http://www.7-zip.org/sdk.html):
//   ./CPP/Windows/PropVariant.cpp
#include "stdafx.h"
#include "PropVariant.h"
#include <7zip/CPP/Common/Defs.h>


namespace SevenZip
{
	namespace intl
	{

		CPropVariant::CPropVariant(const PROPVARIANT &varSrc)
		{
			vt = VT_EMPTY;
			InternalCopy(&varSrc);
		}

		CPropVariant::CPropVariant(const CPropVariant &varSrc)
		{
			vt = VT_EMPTY;
			InternalCopy(&varSrc);
		}

		CPropVariant::CPropVariant(BSTR bstrSrc)
		{
			vt = VT_EMPTY;
			*this = bstrSrc;
		}

		CPropVariant::CPropVariant(LPCOLESTR lpszSrc)
		{
			vt = VT_EMPTY;
			*this = lpszSrc;
		}

		CPropVariant& CPropVariant::operator=(const CPropVariant &varSrc)
		{
			InternalCopy(&varSrc);
			return *this;
		}
		CPropVariant& CPropVariant::operator=(const PROPVARIANT &varSrc)
		{
			InternalCopy(&varSrc);
			return *this;
		}

		CPropVariant& CPropVariant::operator=(BSTR bstrSrc)
		{
			*this = (LPCOLESTR)bstrSrc;
			return *this;
		}

		static const char *kMemException = "out of memory";

		CPropVariant& CPropVariant::operator=(LPCOLESTR lpszSrc)
		{
			InternalClear();
			vt = VT_BSTR;
			wReserved1 = 0;
			bstrVal = ::SysAllocString(lpszSrc);
			if (bstrVal == nullptr && lpszSrc != nullptr)
			{
				throw kMemException;
			}
			return *this;
		}


		CPropVariant& CPropVariant::operator=(const char *s)
		{
			InternalClear();
			vt = VT_BSTR;
			wReserved1 = 0;
			UINT len = (UINT)strlen(s);
			bstrVal = ::SysAllocStringByteLen(nullptr, (UINT)len * sizeof(OLECHAR));
			if (bstrVal == nullptr)
			{
				throw kMemException;
			}

			for (UINT i = 0; i <= len; i++)
					bstrVal[i] = s[i];
			return *this;
		}

		CPropVariant& CPropVariant::operator=(bool bSrc)
		{
			if (vt != VT_BOOL)
			{
				InternalClear();
				vt = VT_BOOL;
			}
			boolVal = bSrc ? VARIANT_TRUE : VARIANT_FALSE;
			return *this;
		}

		CPropVariant& CPropVariant::operator=(Byte value)
		{
			if (vt != VT_UI1)
			{
				InternalClear();
				vt = VT_UI1;
			}
			bVal = value;
			return *this;
		}

		CPropVariant& CPropVariant::operator=(Int16 value)
		{
			if (vt != VT_I2)
			{
				InternalClear();
				vt = VT_I2;
			}
			iVal = value;
			return *this;
		}

		CPropVariant& CPropVariant::operator=(UInt16 value)
		{
			if (vt != VT_UI2)
			{
				InternalClear();
				vt = VT_UI2;
			}
			uiVal = value;
			return *this;
		}

		CPropVariant& CPropVariant::operator=(Int32 value)
		{
			if (vt != VT_I4)
			{
				InternalClear();
				vt = VT_I4;
			}
			lVal = value;
			return *this;
		}

		CPropVariant& CPropVariant::operator=(UInt32 value)
		{
			if (vt != VT_UI4)
			{
				InternalClear();
				vt = VT_UI4;
			}
			ulVal = value;
			return *this;
		}

		CPropVariant& CPropVariant::operator=(Int64 value)
		{
			if (vt != VT_I8)
			{
				InternalClear();
				vt = VT_I8;
			}
			hVal.QuadPart = value;
			return *this;
		}

		CPropVariant& CPropVariant::operator=(UInt64 value)
		{
			if (vt != VT_UI8)
			{
				InternalClear();
				vt = VT_UI8;
			}
			uhVal.QuadPart = value;
			return *this;
		}

		CPropVariant& CPropVariant::operator=(const FILETIME &value)
		{
			if (vt != VT_FILETIME)
			{
				InternalClear();
				vt = VT_FILETIME;
			}
			filetime = value;
			return *this;
		}

		static HRESULT MyPropVariantClear(PROPVARIANT *prop)
		{
			switch (prop->vt)
			{
			case VT_UI1:
			case VT_I1:
			case VT_I2:
			case VT_UI2:
			case VT_BOOL:
			case VT_I4:
			case VT_UI4:
			case VT_R4:
			case VT_INT:
			case VT_UINT:
			case VT_ERROR:
			case VT_FILETIME:
			case VT_UI8:
			case VT_R8:
			case VT_CY:
			case VT_DATE:
				prop->vt = VT_EMPTY;
				prop->wReserved1 = 0;
				return S_OK;
			}
			return ::VariantClear((VARIANTARG *)prop);
		}

		HRESULT CPropVariant::Clear()
		{
			return MyPropVariantClear(this);
		}

		HRESULT CPropVariant::Copy(const PROPVARIANT* pSrc)
		{
			::VariantClear((tagVARIANT *)this);
			switch (pSrc->vt)
			{
			case VT_UI1:
			case VT_I1:
			case VT_I2:
			case VT_UI2:
			case VT_BOOL:
			case VT_I4:
			case VT_UI4:
			case VT_R4:
			case VT_INT:
			case VT_UINT:
			case VT_ERROR:
			case VT_FILETIME:
			case VT_UI8:
			case VT_R8:
			case VT_CY:
			case VT_DATE:
				memmove((PROPVARIANT*)this, pSrc, sizeof(PROPVARIANT));
				return S_OK;
			}
			return ::VariantCopy((tagVARIANT *)this, (tagVARIANT *)const_cast<PROPVARIANT *>(pSrc));
		}


		HRESULT CPropVariant::Attach(PROPVARIANT *pSrc)
		{
			HRESULT hr = Clear();
			if (FAILED(hr))
				return hr;
			memcpy(this, pSrc, sizeof(PROPVARIANT));
			pSrc->vt = VT_EMPTY;
			return S_OK;
		}

		HRESULT CPropVariant::Detach(PROPVARIANT *pDest)
		{
			HRESULT hr = MyPropVariantClear(pDest);
			if (FAILED(hr))
				return hr;
			memcpy(pDest, this, sizeof(PROPVARIANT));
			vt = VT_EMPTY;
			return S_OK;
		}

		HRESULT CPropVariant::InternalClear()
		{
			HRESULT hr = Clear();
			if (FAILED(hr))
			{
				vt = VT_ERROR;
				scode = hr;
			}
			return hr;
		}

		void CPropVariant::InternalCopy(const PROPVARIANT *pSrc)
		{
			HRESULT hr = Copy(pSrc);
			if (FAILED(hr))
			{
				if (hr == E_OUTOFMEMORY)
					throw kMemException;
				vt = VT_ERROR;
				scode = hr;
			}
		}

		int CPropVariant::Compare(const CPropVariant &a)
		{
			if (vt != a.vt)
				return MyCompare(vt, a.vt);
			switch (vt)
			{
			case VT_EMPTY: return 0;
			// case VT_I1: return MyCompare(cVal, a.cVal);
			case VT_UI1: return MyCompare(bVal, a.bVal);
			case VT_I2: return MyCompare(iVal, a.iVal);
			case VT_UI2: return MyCompare(uiVal, a.uiVal);
			case VT_I4: return MyCompare(lVal, a.lVal);
			case VT_UI4: return MyCompare(ulVal, a.ulVal);
			// case VT_UINT: return MyCompare(uintVal, a.uintVal);
			case VT_I8: return MyCompare(hVal.QuadPart, a.hVal.QuadPart);
			case VT_UI8: return MyCompare(uhVal.QuadPart, a.uhVal.QuadPart);
			case VT_BOOL: return -MyCompare(boolVal, a.boolVal);
			case VT_FILETIME: return ::CompareFileTime(&filetime, &a.filetime);
			case VT_BSTR:
				return 0; // Not implemented
				// return MyCompare(aPropVarint.cVal);
			default: return 0;
			}
		}

	}
}
