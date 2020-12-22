#include "stdafx.h"
#include "FileSys.h"
#include "PathScanner.h"
#include <ShlObj.h>


namespace SevenZip
{
namespace intl
{

class ScannerCallback : public PathScanner::Callback
{
private:

	std::vector< FilePathInfo > m_files;
	bool m_recursive = false;
	bool m_onlyFirst = false;

public:

	ScannerCallback(bool recursive, bool onlyFirst = false) : m_recursive(recursive), m_onlyFirst(onlyFirst) {}
	const std::vector< FilePathInfo >& GetFiles() { return m_files; }

	bool ShouldDescend(const FilePathInfo& directory) override { return m_recursive; }
	void ExamineFile(const FilePathInfo& file, bool& exit) override
	{
		m_files.push_back(file);
		if (m_onlyFirst)
		{
			exit = true;
		}
	}
};


class IsEmptyCallback : public PathScanner::Callback
{
private:

	bool m_isEmpty = true;

public:
	IsEmptyCallback() = default;
	bool IsEmpty() const { return m_isEmpty; }

	bool ShouldDescend(const FilePathInfo& directory) override { directory;  return true; }
	void ExamineFile(const FilePathInfo& file, bool& exit) override { file; exit = true; }
};

tstring FileSys::GetPath(const tstring& filePath)
{
	// Find the last "\" or "/" in the string and return it and everything before it.
	size_t index = filePath.rfind(_T('\\'));
	size_t index2 = filePath.rfind(_T('/'));

	if (index2 != std::string::npos && index2 > index)
	{
		index = index2;
	}

	if (index == std::string::npos)
	{
		// No path sep.
		return tstring();
	}

	if (index + 1 >= filePath.size())
	{
		// Last char is path sep, the whole thing is a path.
		return filePath;
	}

	return filePath.substr(0, index + 1);
}

tstring FileSys::GetFileName(const tstring& filePathOrName)
{
	// Find the last "\" or "/" in the string and return everything after it.
	size_t index = filePathOrName.rfind(_T('\\'));
	size_t index2 = filePathOrName.rfind(_T('/'));

	if (index2 != std::string::npos && index2 > index)
	{
		index = index2;
	}

	if (index == std::string::npos)
	{
		// No path sep, return the whole thing.
		return filePathOrName;
	}

	if (index + 1 >= filePathOrName.size())
	{
		// Last char is path sep, no filename.
		return tstring();
	}

	return filePathOrName.substr(index + 1, filePathOrName.size() - (index + 1));
}

tstring FileSys::AppendPath(const tstring& left, const tstring& right)
{
	if (left.empty())
	{
		return right;
	}

	TCHAR lastChar = left[left.size() - 1];
	if (lastChar == _T('\\') || lastChar == _T('/'))
	{
		return left + right;
	}

	return left + _T("\\") + right;
}

tstring FileSys::ExtractRelativePath(const tstring& basePath, const tstring& fullPath)
{
	if (basePath.size() >= fullPath.size())
	{
		return tstring();
	}

	if (basePath != fullPath.substr(0, basePath.size()))
	{
		return tstring();
	}

	return fullPath.substr(basePath.size(), fullPath.size() - basePath.size());
}

bool FileSys::DirectoryExists(const tstring& path)
{
	DWORD attributes = GetFileAttributes(path.c_str());

	if (attributes == INVALID_FILE_ATTRIBUTES)
	{
		return false;
	}

	return (attributes & FILE_ATTRIBUTE_DIRECTORY) != 0;
}

bool FileSys::IsDirectoryEmptyRecursive(const tstring& path)
{
	IsEmptyCallback cb;
	PathScanner::Scan(path, _T(""), cb);
	return cb.IsEmpty();
}

bool FileSys::CreateDirectoryTree(const tstring& path)
{
	int ret = SHCreateDirectoryEx(nullptr, path.c_str(), nullptr);
	return ret == ERROR_SUCCESS;
}

std::vector< FilePathInfo > FileSys::GetFile(const tstring& filePathOrName, bool absolutePath /*= false*/)
{
	tstring path = FileSys::GetPath(filePathOrName);
	tstring name = FileSys::GetFileName(filePathOrName);

	ScannerCallback cb(false, true);
	PathScanner::Scan(path, absolutePath ? _T("") : path, name, cb);
	return cb.GetFiles();
}

std::vector< FilePathInfo > FileSys::GetFilesInDirectory(const tstring& directory, const tstring& searchPattern, const tstring& pathPrefix, bool recursive)
{
	ScannerCallback cb(recursive);
	PathScanner::Scan(directory, pathPrefix, searchPattern, cb);
	return cb.GetFiles();
}

CComPtr< IStream > FileSys::OpenFileToRead(const tstring& filePath)
{
	CComPtr< IStream > fileStream;

#ifdef _UNICODE
	const wchar_t* filePathStr = filePath.c_str();
#else
	wchar_t filePathStr[MAX_PATH];
	MultiByteToWideChar(_AtlGetConversionACP(), 0, filePath.c_str(), (int)filePath.length() + 1, filePathStr, MAX_PATH);
#endif
	if (FAILED(SHCreateStreamOnFileEx(filePathStr, STGM_READ, FILE_ATTRIBUTE_NORMAL, FALSE, nullptr, &fileStream)))
	{
		return nullptr;
	}

	return fileStream;
}

CComPtr< IStream > FileSys::OpenFileToWrite(const tstring& filePath)
{
	CComPtr< IStream > fileStream;

#ifdef _UNICODE
	const wchar_t* filePathStr = filePath.c_str();
#else
	wchar_t filePathStr[MAX_PATH];
	MultiByteToWideChar(_AtlGetConversionACP(), 0, filePath.c_str(), (int)filePath.length() + 1, filePathStr, MAX_PATH);
#endif
	if (FAILED(SHCreateStreamOnFileEx(filePathStr, STGM_CREATE | STGM_WRITE, FILE_ATTRIBUTE_NORMAL, TRUE, nullptr, &fileStream)))
	{
		return nullptr;
	}

	return fileStream;
}

}
}
