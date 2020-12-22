// This file is based on the following file from the LZMA SDK (http://www.7-zip.org/sdk.html):
//   ./CPP/7zip/UI/Client7z/Client7z.cpp
#pragma once


#include <7zip/Archive/IArchive.h>
#include <7zip/IPassword.h>
#include <7zip/UI/Common/UpdateCallback.h>

#include "ProgressCallback.h"


namespace SevenZip
{
namespace intl
{
class ArchiveExtractCallback : public IArchiveExtractCallback, public ICryptoGetTextPassword
{
private:

	long m_refCount = 0;
	CComPtr< IInArchive > m_archiveHandler;
	tstring m_directory;

	tstring m_password;

	tstring m_relPath;
	tstring m_absPath;
	bool m_isDir = false;

	tstring m_archivePath;

	bool m_hasAttrib = false;
	UInt32 m_attrib = 0;

	bool m_hasModifiedTime = false;
	FILETIME m_modifiedTime = { 0 };

	bool m_hasNewFileSize = false;
	UInt64 m_newFileSize = 0;

	IProgressCallback* m_callback = nullptr;

public:

	ArchiveExtractCallback(const CComPtr< IInArchive >& archiveHandler, const tstring& directory, const tstring& archivePath, const tstring& password, IProgressCallback* callback);
	virtual ~ArchiveExtractCallback() = default;

	STDMETHOD(QueryInterface)(REFIID iid, void** ppvObject);
	STDMETHOD_(ULONG, AddRef)();
	STDMETHOD_(ULONG, Release)();

	// IProgress
	STDMETHOD(SetTotal)(UInt64 size);
	STDMETHOD(SetCompleted)(const UInt64* completeValue);

	// Early exit, this is not part of any interface
	STDMETHOD(CheckBreak)();

	// IArchiveExtractCallback
	STDMETHOD(GetStream)(UInt32 index, ISequentialOutStream** outStream, Int32 askExtractMode);
	STDMETHOD(PrepareOperation)(Int32 askExtractMode) { return S_OK; }
	STDMETHOD(SetOperationResult)(Int32 resultEOperationResult);

	// ICryptoGetTextPassword
	STDMETHOD(CryptoGetTextPassword)(BSTR* password);

private:

	void GetPropertyFilePath(UInt32 index);
	void GetPropertyAttributes(UInt32 index);
	void GetPropertyIsDir(UInt32 index);
	void GetPropertyModifiedTime(UInt32 index);
	void GetPropertySize(UInt32 index);

	void EmitDoneCallback();
	void EmitFileDoneCallback(const tstring& path);
};
}
}
