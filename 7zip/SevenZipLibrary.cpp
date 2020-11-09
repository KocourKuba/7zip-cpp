#include "stdafx.h"
#include "SevenZipLibrary.h"
#include "GUIDs.h"


namespace SevenZip
{

	const TString DefaultLibraryPath = _T("7z.dll");

	SevenZipLibrary::~SevenZipLibrary()
	{
		Free();
	}

	bool SevenZipLibrary::Load()
	{
		return Load(DefaultLibraryPath);
	}

	bool SevenZipLibrary::Load(const TString& libraryPath)
	{
		Free();
		m_dll = LoadLibrary(libraryPath.c_str());
		if (m_dll == nullptr)
		{
			return false;
		}

		m_func = reinterpret_cast<CreateObjectFunc>(GetProcAddress(m_dll, "CreateObject"));
		if (m_func == nullptr)
		{
			Free();
			return false;
		}
		return true;
	}

	void SevenZipLibrary::Free()
	{
		if (m_dll != nullptr)
		{
			FreeLibrary(m_dll);
			m_dll = nullptr;
			m_func = nullptr;
		}
	}

	bool SevenZipLibrary::CreateObject(const GUID& clsID, const GUID& interfaceID, void** outObject) const
	{
		if (m_func == nullptr)
		{
			return false;
		}

		HRESULT hr = m_func(&clsID, &interfaceID, outObject);
		return SUCCEEDED(hr);
	}

}
