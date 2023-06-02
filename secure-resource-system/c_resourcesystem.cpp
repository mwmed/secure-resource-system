#include "c_resourcesystem.h"
#include "c_baseresource.h"
#include <nzip.h>
#include <crc32.h>
void resource::c_resourcesystem::create_directory(std::string directory_name)
{
    c_resourcedirectory* res_directory = new c_resourcedirectory();

    res_directory->directory_name = directory_name;

    m_directories[directory_name] = res_directory;
}

bool resource::c_resourcesystem::add_resource_to_directory(std::string directory_name, std::string resource_path, std::string resource_name)
{
	// Create a new c_baseresource object to store the resource data
	c_baseresource* readable_resource = new c_baseresource();

	std::streampos file_size = 0;
	bool result = false;

	printf("Opening resource %s \n", resource_path.c_str());

	// Open the resource file and get the buffer
	auto buffer = util::open_file(resource_path, file_size, result);
	if (!result) {
		printf("Failed to open file \n");
		return false;
	}

	// Set the resource size, data, and path in the readable_resource object
	readable_resource->resource_size = file_size;
	readable_resource->resource_data = (std::uint8_t*)buffer;
	readable_resource->resource_path = resource_name;

	// Add the readable_resource to the m_resources map of the specified directory
	m_directories[directory_name]->m_resources[resource_name] = readable_resource;

	return true;
}

void resource::c_resourcesystem::save_main_resource()
{
	c_array arr;

	// Write the number of directories to the array
	arr.write<uint32_t>(m_directories.size());

	// Iterate over each directory and write its main data to the array
	for (auto resource : m_directories) {
		auto buffer = resource.second->write_main_data();

		// Write the directory name and its main data buffer to the array
		arr.write_string(resource.second->directory_name);
		arr.write(buffer.get_buffer(), buffer.get_buffer_length());
	}

	// Write the number of resources to the array
	arr.write<uint32_t>(m_resources.size());

	// Iterate over each resource and write its information to the array
	for (auto resource : m_resources) {
		// Write the resource path and its size to the array
		arr.write_string(resource.second->resource_path);
		arr.write<uint32_t>(resource.second->resource_size);
	}
	
	auto compressed = crypt::compress_packet(arr);


	// Create or overwrite the "resource.ld" file with the contents of the array
	util::create_if_not_exists("resource.ld");
	util::write_to_file("resource.ld", (char*)compressed.get_buffer(), compressed.get_buffer_length());
}

void resource::c_resourcesystem::save_resources()
{
	c_array arr;

	// Set the maximum file size for each resource file
	size_t max_file_size = 400000;

	for (auto dir : m_directories) {
		dir.second->file_counter = 0;
		size_t current_file_size = 0;
		c_array curr_res_arr;

		printf("Saving Directory %s | %d \n", dir.second->directory_name.c_str(), dir.second->m_resources.size());

		std::string directory_name = std::to_string(hash::compute_hash_crc32(dir.second->directory_name));
		for (auto resource : dir.second->m_resources)
		{
			// Generate the file name for the resource
			auto file_name = "resources\\" + directory_name + "_" + std::to_string(hash::compute_hash_crc32(std::to_string(dir.second->file_counter))) + ".res";

			printf("Saving Resource %s \n", file_name.c_str());

			// Write the resource data to the current resource array
			curr_res_arr.write(resource.second->resource_data, resource.second->resource_size);

			current_file_size += resource.second->resource_size;

			// If the current file size exceeds the maximum file size, create a new resource file
			if (current_file_size > max_file_size) {

				auto compressed = crypt::compress_packet(curr_res_arr);
				util::create_if_not_exists(file_name);
				util::write_to_file(file_name, (char*)compressed.get_buffer(), compressed.get_buffer_length());

				current_file_size = 0;
				++dir.second->file_counter;
				curr_res_arr.reset();
			}
		}

		// If there is remaining data in the current resource array, create the last resource file
		if (curr_res_arr.get_buffer_length()) {
			auto file_name = "resources\\" + directory_name + "_" + std::to_string(hash::compute_hash_crc32(std::to_string(dir.second->file_counter))) + ".res";
			auto compressed = crypt::compress_packet(curr_res_arr);

			util::create_if_not_exists(file_name);
			util::write_to_file(file_name, (char*)compressed.get_buffer(), compressed.get_buffer_length());
		}
	}
}

bool resource::c_resourcesystem::load_resources_from_main_file()
{
	bool result = false;
	bool read_result = false;
	std::streampos file_size = 0;
	char* buffer = util::open_file("resource.ld", file_size, read_result);

	c_array compressed((std::int8_t*)buffer, file_size);
	// Check if the main file was successfully opened and read
	if (read_result)
	{
		auto decompressed = crypt::decompress_packet(compressed);
		printf("Decompressed Size %d  Normal Size %d \n", decompressed.get_buffer_length(), compressed.get_buffer_length());
		c_array arr;
		arr.init((int8_t*)decompressed.get_buffer(), decompressed.get_buffer_length());
		uint32_t directory_size = 0;
		printf("Loading Directories ..\n");

		// Read the number of directories from the array
		if (arr.read<uint32_t>(directory_size))
		{
			for (int i = 0; i < directory_size; i++)
			{
				// Create a new resource directory object
				c_resourcedirectory* resource_directory = new c_resourcedirectory();

				// Read the directory data from the array
				if (!resource_directory->read(arr))
				{
					printf("Failed to read directory \n");
					return false;
				}

				// Add the resource directory to the m_directories map
				m_directories[resource_directory->directory_name] = resource_directory;
			}
		}

		result = true;
	}

	return result;
}
