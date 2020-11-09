#pragma once


#include "Enum.h"


namespace SevenZip
{
	struct CompressionFormat
	{
		enum _Enum
		{
			Unknown,
			SevenZip,
			Zip,
			GZip,
			BZip2,
			Rar,
			Tar,
			Iso,
			Cab,
			Lzma,
			Lzma86,
			Arj,
			XZ,

		};

		using _Definition = intl::EnumerationDefinitionNoStrings;
		using _Value = intl::EnumerationValue< _Enum, _Definition, Unknown >;
	};

	using CompressionFormatEnum = CompressionFormat::_Value;
}
