#pragma once
#include "base_includes.h"






namespace baselib {
	class c_scalarlength {
	public:
		inline c_scalarlength() {}

		inline c_scalarlength(uint64_t length)
		{
			scalar_length = length;

			fix_index();
		}

		uint8_t scalar_index = 0;
		uint64_t scalar_length = 0;



		void fix_index() {
			uint64_t obj = scalar_length;
			if ((std::numeric_limits<std::int8_t>::min)() <= obj && obj <= (std::numeric_limits<std::int8_t>::max)())
			{
				scalar_index = 1;
			}
			else if ((std::numeric_limits<int16_t>::min)() <= obj && obj <= (std::numeric_limits<int16_t>::max)())
			{
				scalar_index = 2;

			}
			else if ((std::numeric_limits<int32_t>::min)() <= obj && obj <= (std::numeric_limits<int32_t>::max)())
			{
				scalar_index = 4;

			}
			else
			{
				scalar_index = 8;
			}
		}
	};
}