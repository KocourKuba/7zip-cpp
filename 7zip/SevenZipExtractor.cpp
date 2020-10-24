#include "stdafx.h"
#include "SevenZipExtractor.h"
#include "GUIDs.h"
#include "FileSys.h"
#include "ArchiveOpenCallback.h"
#include "ArchiveExtractCallback.h"
#include "MemExtractCallback.h"
#include "InStreamWrapper.h"
#include "UsefulFunctions.h"


namespace SevenZip
{

	using namespace intl;

	SevenZipExtractor::SevenZipExtractor(SevenZipLibrary* library, const TString& archivePath)
		: SevenZipArchive(library, archivePath)
	{
	}

	SevenZipExtractor::~SevenZipExtractor()
	{
	}

	bool SevenZipExtractor::ExtractArchive(const TString& destDirectory, ProgressCallback* callback /*= nullptr*/)
	{
		CComPtr< IStream > archiveStream = FileSys::OpenFileToRead(m_archivePath);

		if (archiveStream == nullptr)
		{
			return false;	//Could not open archive
		}

		return ExtractFilesFromArchive(archiveStream, NULL, -1, destDirectory, callback);
	}

	bool SevenZipExtractor::ExtractFilesFromArchive(const unsigned int* fileIndices,
													const unsigned int numberFiles,
													const TString& destDirectory,
													ProgressCallback* callback /*= nullptr*/)
	{
		CComPtr< IStream > archiveStream = FileSys::OpenFileToRead(m_archivePath);

		if (archiveStream == nullptr)
		{
			return false;	//Could not open archive
		}

		return ExtractFilesFromArchive(archiveStream, fileIndices, numberFiles, destDirectory, callback);
	}

	bool SevenZipExtractor::ExtractFileToMemory(const unsigned int index, std::vector<BYTE>& out_buffer, ProgressCallback* callback /*= nullptr*/)
	{
		CComPtr< IStream > archiveStream = FileSys::OpenFileToRead(m_archivePath);
		if (archiveStream == nullptr)
		{
			return false;	//Could not open archive
		}

		CComPtr< IInArchive > archive = UsefulFunctions::GetArchiveReader(*m_library, m_compressionFormat);
		CComPtr< InStreamWrapper > inFile = new InStreamWrapper(archiveStream);
		CComPtr< ArchiveOpenCallback > openCallback = new ArchiveOpenCallback(m_password);

		HRESULT hr = archive->Open(inFile, 0, openCallback);
		if (hr != S_OK)
		{
			return false;	//Open archive error
		}

		const UInt32 indices[] = { index };

		CComPtr< MemExtractCallback > extractCallback = new MemExtractCallback(archive, out_buffer, m_archivePath, m_password, callback);

		hr = archive->Extract(indices, 1, false, extractCallback);
		if (hr != S_OK)
		{
			// returning S_FALSE also indicates error
			return false;	//Extract archive error
		}

		if (callback)
		{
			callback->OnDone(m_archivePath);
		}

		archive->Close();

		return true;

	}

	bool SevenZipExtractor::ExtractFilesFromArchive(const CComPtr<IStream>& archiveStream,
													const unsigned int* filesIndices,
													const unsigned int numberFiles,
													const TString& destDirectory,
													ProgressCallback* callback)
	{
		CComPtr< IInArchive > archive = UsefulFunctions::GetArchiveReader(*m_library, m_compressionFormat);
		CComPtr< InStreamWrapper > inFile = new InStreamWrapper(archiveStream);
		CComPtr< ArchiveOpenCallback > openCallback = new ArchiveOpenCallback(m_password);

		HRESULT hr = archive->Open(inFile, 0, openCallback);
		if (hr != S_OK)
		{
			return false;	//Open archive error
		}

		CComPtr< ArchiveExtractCallback > extractCallback = new ArchiveExtractCallback(archive, destDirectory, m_archivePath, m_password, callback);

		hr = archive->Extract(filesIndices, numberFiles, false, extractCallback);
		if (hr != S_OK)
		{
			// returning S_FALSE also indicates error
			return false;	//Extract archive error
		}

		if (callback)
		{
			callback->OnDone(m_archivePath);
		}

		archive->Close();

		return true;
	}

	bool SevenZipExtractor::ExtractFile(const TString& filename, const TString& path, bool bUseFullPath /*= false*/)
	{
		std::vector<int> indices;
		if (!FindFilesInArchive(filename, indices, bUseFullPath, true)) return false;

		return ExtractFilesFromArchive((unsigned int*)indices.data(), (unsigned int)indices.size(), path);
	}

	bool SevenZipExtractor::ExtractFileToMemory(const TString& filename, std::vector<BYTE>& memBuffer, bool bUseFullPath /*= false*/)
	{
		std::vector<int> indices;
		if (!FindFilesInArchive(filename, indices, bUseFullPath, true)) return false;

		// only first found file to extract
		return ExtractFileToMemory(indices[0], memBuffer);
	}

	bool SevenZipExtractor::FindFilesInArchive(const TString &filename, std::vector<int> &indices, bool bUseFullPath, bool bOnlyFirst)
	{
		const std::vector<std::wstring>& files = GetItemsNames();
#ifdef _UNICODE
		const wchar_t* filenameStr = filename.c_str();
#else
		wchar_t filenameStr[MAX_PATH];
		MultiByteToWideChar(_AtlGetConversionACP(), 0, filename.c_str(), (int)filename.length() + 1, filenameStr, MAX_PATH);
#endif

		int indice = 0;
		for (const auto& file : files)
		{
			int add = _wcsicmp(filenameStr, file.c_str());
			if (add != 0 && !bUseFullPath)
			{
				size_t pos = file.rfind('\\');
				if (pos != std::string::npos)
				{
					++pos;
					add = _wcsicmp(filenameStr, file.c_str() + pos);
				}
			}

			if (add == 0)
			{
				indices.push_back(indice);
				if (bOnlyFirst) break;
			}

			indice++;
		}

		return !indices.empty();
	}
}
