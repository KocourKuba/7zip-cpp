// This file is based on the following file from the LZMA SDK (http://www.7-zip.org/sdk.html):

#include "StdAfx.h"
#include "ArchiveOpenCallback.h"


namespace SevenZip
{
	namespace intl
	{

		ArchiveOpenCallback::ArchiveOpenCallback(const TString& password)
			: m_password(password)
		{
		}

		STDMETHODIMP ArchiveOpenCallback::QueryInterface(REFIID iid, void** ppvObject)
		{
			if (iid == __uuidof(IUnknown))
			{
				*ppvObject = reinterpret_cast<IUnknown*>(this);
				AddRef();
				return S_OK;
			}

			if (iid == IID_IArchiveOpenCallback)
			{
				*ppvObject = static_cast<IArchiveOpenCallback*>(this);
				AddRef();
				return S_OK;
			}

			if (iid == IID_ICryptoGetTextPassword)
			{
				*ppvObject = static_cast<ICryptoGetTextPassword*>(this);
				AddRef();
				return S_OK;
			}

			return E_NOINTERFACE;
		}

		STDMETHODIMP_(ULONG) ArchiveOpenCallback::AddRef()
		{
			return static_cast<ULONG>(InterlockedIncrement(&m_refCount));
		}

		STDMETHODIMP_(ULONG) ArchiveOpenCallback::Release()
		{
			ULONG res = static_cast<ULONG>(InterlockedDecrement(&m_refCount));
			if (res == 0)
			{
				delete this;
			}
			return res;
		}

		STDMETHODIMP ArchiveOpenCallback::SetTotal(const UInt64* /*files*/, const UInt64* /*bytes*/)
		{
			return S_OK;
		}

		STDMETHODIMP ArchiveOpenCallback::SetCompleted(const UInt64* /*files*/, const UInt64* /*bytes*/)
		{
			return S_OK;
		}

		STDMETHODIMP ArchiveOpenCallback::CryptoGetTextPassword(BSTR* password)
		{
			if (!m_password.empty())
				*password = TStringAllocSysString(m_password);

			return S_OK;
		}

	}
}
