#pragma once


#include "SevenZipLibrary.h"
#include "SevenZipArchive.h"
#include "ProgressCallback.h"


namespace SevenZip
{
	class SevenZipExtractor : public SevenZipArchive
	{
	public:

		SevenZipExtractor() {}
		SevenZipExtractor(SevenZipLibrary* library, const TString& archivePath);
		virtual ~SevenZipExtractor();

		virtual bool ExtractArchive(const TString& directory, ProgressCallback* callback = nullptr);
		virtual bool ExtractFilesFromArchive(const unsigned int* fileIndices,
											 const unsigned int numberFiles,
											 const TString& directory,
											 ProgressCallback* callback = nullptr);
		virtual bool ExtractFileToMemory(const unsigned int index, std::vector<BYTE>& out_buffer, ProgressCallback* callback = nullptr);

		virtual bool ExtractFile(const TString& filename, const TString& path, bool bUseFullPath = false);
		virtual bool ExtractFileToMemory(const TString& filename, std::vector<BYTE>& memBuffer, bool bUseFullPath = false);

	private:
		bool FindFilesInArchive(const TString &filename, std::vector<int> &indices, bool bUseFullPath, bool bOnlyFirst);
		bool ExtractFilesFromArchive(const CComPtr<IStream>& archiveStream,
									 const unsigned int* fileIndices,
									 const unsigned int numberFiles,
									 const TString& directory,
									 ProgressCallback* callback);
	};
}
