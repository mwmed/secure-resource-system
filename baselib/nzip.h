#pragma once
#include "c_array.h"









namespace crypt {
	baselib::c_array compress_packet(baselib::c_array& message);
	BYTE* compress_data(BYTE* message, size_t inSize, size_t& outSize);
	baselib::c_array decompress_packet(baselib::c_array& message);
}