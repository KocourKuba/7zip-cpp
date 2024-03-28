#include "stdafx.h"

#include "SevenZipWrapper.h"
#include "FileSys.h"

namespace SevenZip
{
	SevenZipWrapper::SevenZipWrapper()
	{
		m_compressor.SetLibrary(&m_lib);
		m_extractor.SetLibrary(&m_lib);
	}

	SevenZipWrapper::SevenZipWrapper(const TString& libPath) : SevenZipWrapper()
	{
		SetLibPath(libPath);
	}

	bool SevenZipWrapper::OpenArchive(const TString& archivePath, const TString& password /*= _T("")*/)
	{
		ATLASSERT(m_libLoaded);
		if (m_libLoaded)
		{
			m_extractor.SetArchivePath(archivePath);
			m_extractor.SetPassword(password);
			m_extractor.ReadInArchiveMetadata();
		}

		return m_libLoaded;
	}

	bool SevenZipWrapper::CreateArchive(const TString& archivePath, const TString& password /*= _T("")*/)
	{
		ATLASSERT(m_libLoaded);
		if (!m_libLoaded)
		{
			return false;
		}

		m_compressor.SetPassword(password);
		m_compressor.SetArchivePath(archivePath);

		return m_compressor.DoCompress();
	}
}
