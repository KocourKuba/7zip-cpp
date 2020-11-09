#pragma once


#include <tchar.h>
#include <string>
#include <OleAuto.h>

namespace SevenZip
{
#ifdef _UNICODE
	using TString = std::wstring;
#else
	using TString = std::string;
#endif
}
namespace SevenZip
{
	BSTR TStringAllocSysString(const TString& string);
	TString BstrToTString(BSTR string);
}
