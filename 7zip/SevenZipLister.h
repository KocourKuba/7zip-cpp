#pragma once

#include "SevenZipLibrary.h"
#include "SevenZipArchive.h"
#include "CompressionFormat.h"
#include "ListCallback.h"


namespace SevenZip
{
	class SevenZipLister : public SevenZipArchive
	{
	public:
		SevenZipLister() = default;
		SevenZipLister(SevenZipLibrary* library, const TString& archivePath);
		virtual ~SevenZipLister() = default;

		virtual bool ListArchive(const TString& password, ListCallback* callback = nullptr) const;


	private:
		bool ListArchive(const CComPtr< IStream >& archiveStream, const TString& password, ListCallback* callback) const;
	};
}
