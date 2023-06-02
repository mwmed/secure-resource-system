#pragma once
#include "baselib_includes.h"




namespace resource {
	class c_resource {
	public:


		virtual c_array write_data() = 0;
		virtual c_array write_main_data() = 0;

	public:
		std::string resource_path = "";

		uint8_t* resource_data = nullptr;

		uint32_t resource_size = 0;
	};
}