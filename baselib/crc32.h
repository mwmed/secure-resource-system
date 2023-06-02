#pragma once
#include "c_array.h"





namespace hash {
	std::uint32_t compute_hash_crc32(baselib::c_array message);

	std::uint32_t compute_hash_crc32(std::string message);
}