#pragma once


#include "Enum.h"


namespace SevenZip
{
	struct CompressionFormat
	{
		enum class _Format
		{
			SevnZip,
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
		using _Value = intl::EnumerationValue<_Format, _Definition, _Format::Unknown>;
	};

	using CompressionFormatEnum = CompressionFormat::_Value;
}
