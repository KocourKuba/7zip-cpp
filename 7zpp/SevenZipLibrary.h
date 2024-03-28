#pragma once

#include "SevenZipException.h"
#include "CompressionFormat.h"

namespace SevenZip
{
	class SevenZipLibrary
	{
	private:

		using CreateObjectFunc = UINT32(WINAPI *)(const GUID* clsID, const GUID* interfaceID, void** outObject);

		HMODULE				m_dll = nullptr;
		CreateObjectFunc	m_func = nullptr;

	public:

		SevenZipLibrary() = default;
		~SevenZipLibrary();

		bool Load();
		bool Load(const TString& libraryPath);
		void Free();

		bool CreateObject(const GUID& clsID, const GUID& interfaceID, void** outObject) const;
	};
}
