#pragma once


#include <exception>
#include "SevenString.h"


namespace SevenZip
{
	TString StrFmt(const TCHAR* format, ...);
	TString GetWinErrMsg(const TString& contextMessage, DWORD lastError);
	TString GetCOMErrMsg(const TString& contextMessage, HRESULT lastError);

	class SevenZipException
	{
	protected:

		TString m_message;

	public:

		SevenZipException() = default;
		SevenZipException(const TString& message);
		virtual ~SevenZipException() = default;

		const TString& GetMessage() const { return m_message; }
	};
}
