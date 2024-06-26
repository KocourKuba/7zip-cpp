// This file is based on the following file from the LZMA SDK (http://www.7-zip.org/sdk.html):
#pragma once


#include <7zip/CPP/7zip/Archive/IArchive.h>
#include <7zip/CPP/7zip/ICoder.h>
#include <7zip/CPP/7zip/IPassword.h>
#include <vector>
#include "FileInfo.h"
#include "ProgressCallback.h"

namespace SevenZip
{
	namespace intl
	{
		class ArchiveUpdateCallback : public IArchiveUpdateCallback, public ICryptoGetTextPassword2, public ICompressProgressInfo
		{
		private:

			long m_refCount = 0;
			TString m_outputPath;
			TString m_password;
			const std::vector< FilePathInfo >& m_filePaths;
			IProgressCallback* m_callback = nullptr;

		public:

			ArchiveUpdateCallback(const std::vector< FilePathInfo >& filePaths, const TString& outputFilePath, const TString& password, IProgressCallback* callback);
			virtual ~ArchiveUpdateCallback() = default;

			STDMETHOD(QueryInterface)(REFIID iid, void** ppvObject);
			STDMETHOD_(ULONG, AddRef)();
			STDMETHOD_(ULONG, Release)();

			// IProgress
			STDMETHOD(SetTotal)(UInt64 size);
			STDMETHOD(SetCompleted)(const UInt64* completeValue);

			// Early exit, this is not part of any interface
			STDMETHOD(CheckBreak)();

			// IArchiveUpdateCallback
			STDMETHOD(GetUpdateItemInfo)(UInt32 index, Int32* newData, Int32* newProperties, UInt32* indexInArchive);
			STDMETHOD(GetProperty)(UInt32 index, PROPID propID, PROPVARIANT* value);
			STDMETHOD(GetStream)(UInt32 index, ISequentialInStream** inStream);
			STDMETHOD(SetOperationResult)(Int32 operationResult);

			// ICryptoGetTextPassword2
			STDMETHOD(CryptoGetTextPassword2)(Int32* passwordIsDefined, BSTR* password);

			// ICompressProgressInfo
			STDMETHOD(SetRatioInfo)(const UInt64* inSize, const UInt64* outSize);
		};
	}
}
