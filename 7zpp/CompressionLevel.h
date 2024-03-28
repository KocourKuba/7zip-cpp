#pragma once


#include "Enum.h"


namespace SevenZip
{
	struct CompressionLevel
	{
		enum _Enum
		{
			None,
			Fast,
			Normal
		};

		using _Definition = intl::EnumerationDefinitionNoStrings;
		using _Value = intl::EnumerationValue< _Enum, _Definition, Normal >;
	};

	using CompressionLevelEnum = CompressionLevel::_Value;
}
