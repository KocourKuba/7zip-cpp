#pragma once


#include "Enum.h"


namespace SevenZip
{
	struct CompressionFormat
	{
		enum _Enum
		{
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
			Unknown, // Must be last in the enum
		};

		using _Definition = intl::EnumerationDefinitionNoStrings;
		using _Value = intl::EnumerationValue< _Enum, _Definition, Unknown >;
	};

	using CompressionFormatEnum = CompressionFormat::_Value;
}
