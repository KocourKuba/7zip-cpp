#include "stdafx.h"
#include "UsefulFunctions.h"
#include "PropVariant.h"
#include <array>


namespace SevenZip
{
	using namespace intl;

	const GUID* UsefulFunctions::GetCompressionGUID(const CompressionFormatEnum& format)
	{
		const GUID* guid = nullptr;

		switch (format)
		{
		case CompressionFormat::_Format::Zip:
			guid = &SevenZip::intl::CLSID_CFormatZip;
			break;

		case CompressionFormat::_Format::GZip:
			guid = &SevenZip::intl::CLSID_CFormatGZip;
			break;

		case CompressionFormat::_Format::BZip2:
			guid = &SevenZip::intl::CLSID_CFormatBZip2;
			break;

		case CompressionFormat::_Format::Rar:
			guid = &SevenZip::intl::CLSID_CFormatRar;
			break;

		case CompressionFormat::_Format::Tar:
			guid = &SevenZip::intl::CLSID_CFormatTar;
			break;

		case CompressionFormat::_Format::Iso:
			guid = &SevenZip::intl::CLSID_CFormatIso;
			break;

		case CompressionFormat::_Format::Cab:
			guid = &SevenZip::intl::CLSID_CFormatCab;
			break;

		case CompressionFormat::_Format::Lzma:
			guid = &SevenZip::intl::CLSID_CFormatLzma;
			break;

		case CompressionFormat::_Format::Lzma86:
			guid = &SevenZip::intl::CLSID_CFormatLzma86;
			break;

		default:
			guid = &SevenZip::intl::CLSID_CFormat7z;
			break;
		}
		return guid;
	}

	CComPtr< IInArchive > UsefulFunctions::GetArchiveReader(const SevenZipLibrary& library, const CompressionFormatEnum& format)
	{
		const GUID* guid = GetCompressionGUID(format);

		CComPtr< IInArchive > archive;
		library.CreateObject(*guid, IID_IInArchive, reinterpret_cast<void**>(&archive));
		return archive;
	}

	CComPtr< IOutArchive > UsefulFunctions::GetArchiveWriter(const SevenZipLibrary& library, const CompressionFormatEnum& format)
	{
		const GUID* guid = GetCompressionGUID(format);

		CComPtr< IOutArchive > archive;
		library.CreateObject(*guid, IID_IOutArchive, reinterpret_cast<void**>(&archive));
		return archive;
	}

	bool UsefulFunctions::GetNumberOfItems(const SevenZipLibrary & library, const TString & archivePath,
										   CompressionFormatEnum &format, size_t & numberofitems, const TString& password)
	{
		CComPtr< IStream > fileStream = FileSys::OpenFileToRead(archivePath);

		if (fileStream == nullptr)
		{
			return false;
		}

		CComPtr< IInArchive > archive = UsefulFunctions::GetArchiveReader(library, format);
		if (!archive)
		{
			return false;
		}

		CComPtr< InStreamWrapper > inFile = new InStreamWrapper(fileStream);
		CComPtr< ArchiveOpenCallback > openCallback = new ArchiveOpenCallback(password);

		HRESULT hr = archive->Open(inFile, nullptr, openCallback);
		if (hr != S_OK)
		{
			return false;
		}

		UInt32 mynumofitems;
		hr = archive->GetNumberOfItems(&mynumofitems);
		if (hr != S_OK)
		{
			return false;
		}
		numberofitems = size_t(mynumofitems);

		archive->Close();
		return true;
	}

	bool UsefulFunctions::GetItemsNames(const SevenZipLibrary & library, const TString & archivePath,
										CompressionFormatEnum &format, size_t & numberofitems,
										std::vector<std::wstring> & itemnames, std::vector<size_t> & origsizes, const TString& password)
	{
		CComPtr< IStream > fileStream = FileSys::OpenFileToRead(archivePath);

		if (fileStream == nullptr)
		{
			return false;
		}

		CComPtr< IInArchive > archive = UsefulFunctions::GetArchiveReader(library, format);
		if (!archive)
		{
			return false;
		}

		CComPtr< InStreamWrapper > inFile = new InStreamWrapper(fileStream);
		CComPtr< ArchiveOpenCallback > openCallback = new ArchiveOpenCallback(password);

		HRESULT hr = archive->Open(inFile, nullptr, openCallback);
		if (hr != S_OK)
		{
			return false;
		}

		UInt32 mynumofitems;
		hr = archive->GetNumberOfItems(&mynumofitems);
		if (hr != S_OK)
		{
			return false;
		}
		numberofitems = size_t(mynumofitems);

		itemnames.clear();
		itemnames.resize(numberofitems);

		origsizes.clear();
		origsizes.resize(numberofitems);

		for (UInt32 i = 0; i < numberofitems; i++)
		{
			// Get uncompressed size of file
			CPropVariant prop;
			hr = archive->GetProperty(i, kpidSize, &prop);
			if (hr != S_OK)
			{
				return false;
			}

			int size = prop.intVal;
			origsizes[i] = size_t(size);

			// Get name of file
			hr = archive->GetProperty(i, kpidPath, &prop);
			if (hr != S_OK)
			{
				return false;
			}

			//valid string? pass back the found value and call the callback function if set
			if (prop.vt == VT_BSTR)
			{
				std::wstring mypath(prop.bstrVal);
				std::swap(itemnames[i], mypath);
			}
		}

		archive->Close();
		return true;
	}

	bool UsefulFunctions::DetectCompressionFormat(const SevenZipLibrary & library, const TString & archivePath,
												  CompressionFormatEnum & archiveCompressionFormat, const TString& password)
	{
		CComPtr< IStream > fileStream = FileSys::OpenFileToRead(archivePath);

		if (fileStream == nullptr)
		{
			return false;
		}

		// Add more formats here if 7zip supports more formats in the future
		std::array<CompressionFormatEnum, (size_t)CompressionFormat::_Format::Unknown> myAvailableFormats = {
			CompressionFormat::_Format::Zip,
			CompressionFormat::_Format::SevnZip,
			CompressionFormat::_Format::Rar,
			CompressionFormat::_Format::GZip,
			CompressionFormat::_Format::BZip2,
			CompressionFormat::_Format::Tar,
			CompressionFormat::_Format::Lzma,
			CompressionFormat::_Format::Lzma86,
			CompressionFormat::_Format::Cab,
			CompressionFormat::_Format::Iso,
			CompressionFormat::_Format::Arj,
			CompressionFormat::_Format::XZ,
		};

		// Check each format for one that works
		for (auto myAvailableFormat : myAvailableFormats)
		{
			archiveCompressionFormat = myAvailableFormat;

			CComPtr< IInArchive > archive = UsefulFunctions::GetArchiveReader(library, archiveCompressionFormat);
			if (!archive) continue;

			CComPtr< InStreamWrapper > inFile = new InStreamWrapper(fileStream);
			CComPtr< ArchiveOpenCallback > openCallback = new ArchiveOpenCallback(password);

			HRESULT hr = archive->Open(inFile, nullptr, openCallback);
			if (hr == S_OK)
			{
				// We know the format if we get here, so return
				archive->Close();
				return true;
			}

			archive->Close();
		}

		//
		//  There is a problem that GZip files will not be detected using the above method.
		//  This is a fix.
		//
		if (true)
		{
			size_t myNumOfItems = 0;
			archiveCompressionFormat = CompressionFormat::_Format::GZip;
			bool result = GetNumberOfItems(library, archivePath, archiveCompressionFormat, myNumOfItems, password);
			if (result == true && myNumOfItems > 0)
			{
				// We know this file is a GZip file, so return
				return true;
			}
		}

		// If you get here, the format is unknown
		archiveCompressionFormat = CompressionFormat::_Format::Unknown;
		return false;
	}

	TString UsefulFunctions::EndingFromCompressionFormat(const CompressionFormatEnum& format)
	{
		switch (format)
		{
		case CompressionFormat::_Format::Zip:
			return _T(".zip");
		case CompressionFormat::_Format::SevnZip:
			return _T(".7z");
		case CompressionFormat::_Format::Rar:
			return _T(".rar");
		case CompressionFormat::_Format::GZip:
			return _T(".gz");
		case CompressionFormat::_Format::BZip2:
			return _T(".bz");
		case CompressionFormat::_Format::Tar:
			return _T(".tar");
		case CompressionFormat::_Format::Lzma:
			return _T(".lzma");
		case CompressionFormat::_Format::Lzma86:
			return _T(".lzma86");
		case CompressionFormat::_Format::Cab:
			return _T(".cab");
		case CompressionFormat::_Format::Iso:
			return _T(".iso");
		case CompressionFormat::_Format::Arj:
			return _T(".arj");
		case CompressionFormat::_Format::XZ:
			return _T(".xz");
		}
		return _T(".zip");
	}

}

