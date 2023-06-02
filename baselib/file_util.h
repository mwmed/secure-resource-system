#pragma once
#include "base_includes.h"







namespace baselib {
	namespace util {
		bool write_to_file(std::string file_name, char* buffer, size_t buffer_size);
		void create_if_not_exists(std::string file_name);

		std::vector<std::pair<std::string, uint64_t>> get_files_in_directory(std::string m_path);

		char* open_file(std::string file_name, std::streampos& file_size, bool& result);


	}
}