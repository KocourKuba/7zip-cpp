#pragma once

#include "7zpp.h"

namespace SevenZip
{
	class SevenZipWrapper
	{
	public:
		SevenZipWrapper();
		SevenZipWrapper(const TString& libPath);
		~SevenZipWrapper() = default;

		bool CreateArchive(const TString& archivePath, const TString& password = _T(""));
		bool OpenArchive(const TString& archivePath, const TString& password = _T(""));

		bool SetLibPath(const TString& libPath) { return (m_libLoaded = m_lib.Load(libPath)); }

		SevenZip::SevenZipCompressor& GetCompressor() { return m_compressor; }
		SevenZip::SevenZipExtractor& GetExtractor() { return m_extractor; }

	private:
		bool m_libLoaded;

		SevenZipLibrary m_lib;
		SevenZipCompressor m_compressor;
		SevenZipExtractor m_extractor;
	};
}