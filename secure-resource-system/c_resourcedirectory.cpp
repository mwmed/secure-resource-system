#include "c_resourcedirectory.h"
#include "c_baseresource.h"
#include <crc32.h>
#include <nzip.h>
bool resource::c_resourcedirectory::read_main_data(c_array& obj)
{
    bool result = false;
    uint32_t resource_size = 0;

    // Read directory name and resource size from obj
    if (obj.read_string(directory_name) && obj.read(resource_size))
    {
        printf("[%s] | Resource Size %d \n", directory_name.c_str(), resource_size);

        // Read resource paths and sizes and store them in m_resources
        for (int i = 0; i < resource_size; i++)
        {
            c_baseresource* m_res = new c_baseresource();

            // Read resource path and size for each resource
            if (!obj.read_string(m_res->resource_path) || !obj.read<uint32_t>(m_res->resource_size))
            {
                return false;
            }

            m_resources[m_res->resource_path] = m_res;
        }

        result = true;
    }
    else
    {
        printf("Failed to read directory name or resource size.\n");
    }

    return result;
}

bool resource::c_resourcedirectory::read_data()
{
    uint64_t folder_count = 0, total_size = 0;
    std::string name = std::to_string(hash::compute_hash_crc32(directory_name));

    // Get total file size and folder count
    for (auto file : util::get_files_in_directory("resources"))
    {
        if (file.first.find(name + "_") != std::string::npos) { // Check if file name contains the desired prefix
            total_size += file.second;
            ++folder_count;
        }
    }
    printf("%lu - %lu\n", folder_count, total_size);

    // Read files and store the bytes in total_data
    for (uint32_t i = 0; i < folder_count; i++)
    {
        auto counter_enc = std::to_string(hash::compute_hash_crc32(std::to_string(i)));
        auto folder_name = "resources/" + name + "_" + counter_enc + ".res"; // Construct folder name

        std::streampos file_size = 0;

        printf("%s\n", folder_name.c_str());

        bool result = false;
        char* buffer = util::open_file(folder_name, file_size, result); // Open file and get buffer
        if (!result) {
            delete[] buffer; // Clean up buffer if open_file fails
            return false;
        }

        baselib::c_array compressed((std::int8_t*)buffer, file_size);
        auto decompressed = crypt::decompress_packet(compressed);
        total_data.write(reinterpret_cast<uint8_t*>(decompressed.get_buffer()), decompressed.get_buffer_length()); // Write buffer data to total_data
        delete[] buffer; // Clean up buffer
    }

    // Read resource data from total_data into resource objects
    for (auto& m_res : m_resources)
    {
        m_res.second->resource_data = new uint8_t[m_res.second->resource_size]; // Allocate memory for resource data

        if (!total_data.read(m_res.second->resource_data, m_res.second->resource_size)) // Read data from total_data
        {
            printf("Failed to read %s\n", m_res.second->resource_path.c_str());
            return false;
        }

        printf("Read %s\n", m_res.second->resource_path.c_str());
    }

    return true;
}
bool resource::c_resourcedirectory::read(c_array& obj)
{
    return (read_main_data(obj) && read_data());
}

c_array resource::c_resourcedirectory::write_data()
{
    c_array arr;

    for (auto res : m_resources) {
        arr.write(res.second->resource_data, res.second->resource_size);
    }

    return arr;
}

c_array resource::c_resourcedirectory::write_main_data()
{
    c_array arr;
    arr.write<uint32_t>(m_resources.size());
    for (auto res : m_resources) 
    {
        arr.write_string(res.second->resource_path);
        arr.write<uint32_t>(res.second->resource_size);
    }
    return arr;
}
