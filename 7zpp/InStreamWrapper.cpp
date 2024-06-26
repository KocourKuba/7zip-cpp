#include "stdafx.h"
#include "InStreamWrapper.h"


namespace SevenZip
{
	namespace intl
	{

		InStreamWrapper::InStreamWrapper(const CComPtr< IStream >& baseStream)
			: m_baseStream(baseStream)
		{
		}

		STDMETHODIMP InStreamWrapper::QueryInterface(REFIID iid, void** ppvObject)
		{
			if (iid == __uuidof(IUnknown))
			{
				*ppvObject = reinterpret_cast<IUnknown*>(this);
				AddRef();
				return S_OK;
			}

			if (iid == IID_ISequentialInStream)
			{
				*ppvObject = static_cast<ISequentialInStream*>(this);
				AddRef();
				return S_OK;
			}

			if (iid == IID_IInStream)
			{
				*ppvObject = static_cast<IInStream*>(this);
				AddRef();
				return S_OK;
			}

			if (iid == IID_IStreamGetSize)
			{
				*ppvObject = static_cast<IStreamGetSize*>(this);
				AddRef();
				return S_OK;
			}

			return E_NOINTERFACE;
		}

		STDMETHODIMP_(ULONG) InStreamWrapper::AddRef()
		{
			return static_cast<ULONG>(InterlockedIncrement(&m_refCount));
		}

		STDMETHODIMP_(ULONG) InStreamWrapper::Release()
		{
			ULONG res = static_cast<ULONG>(InterlockedDecrement(&m_refCount));
			if (res == 0)
			{
				delete this;
			}
			return res;
		}

		STDMETHODIMP InStreamWrapper::Read(void* data, UInt32 size, UInt32* processedSize)
		{
			ULONG read = 0;
			HRESULT hr = m_baseStream->Read(data, size, &read);
			if (processedSize != nullptr)
			{
				*processedSize = read;
			}
			// Transform S_FALSE to S_OK
			return SUCCEEDED(hr) ? S_OK : hr;
		}

		STDMETHODIMP InStreamWrapper::Seek(Int64 offset, UInt32 seekOrigin, UInt64* newPosition)
		{
			LARGE_INTEGER move{};
			ULARGE_INTEGER newPos{};

			move.QuadPart = offset;
			HRESULT hr = m_baseStream->Seek(move, seekOrigin, &newPos);
			if (newPosition != nullptr)
			{
				*newPosition = newPos.QuadPart;
			}
			return hr;
		}

		STDMETHODIMP InStreamWrapper::GetSize(UInt64* size)
		{
			STATSTG statInfo;
			HRESULT hr = m_baseStream->Stat(&statInfo, STATFLAG_NONAME);
			if (SUCCEEDED(hr))
			{
				*size = statInfo.cbSize.QuadPart;
			}
			return hr;
		}

	}
}
