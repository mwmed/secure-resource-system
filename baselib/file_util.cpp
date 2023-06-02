#include "file_util.h"

bool baselib::util::write_to_file(std::string file_name, char* buffer, size_t buffer_size)
{
    // Open the output file in binary write mode
    FILE* output_file = fopen(file_name.c_str(), "wb");
    if (!output_file)
        return false;

    // Write the buffer content to the file
    fwrite(buffer, 1, buffer_size, output_file);

    // Close the file
    fclose(output_file);

    return true;
}

void baselib::util::create_if_not_exists(std::string file_name)
{
    // Check if the file already exists
    if (!std::filesystem::exists(file_name))
    {
        // Create an empty file if it doesn't exist
        FILE* f = fopen(file_name.c_str(), "w");
        fclose(f);
    }
}

std::vector<std::pair<std::string, uint64_t>> baselib::util::get_files_in_directory(std::string m_path)
{
    std::vector<std::pair<std::string, uint64_t>> resources;

    // Iterate over the files in the directory
    for (const auto& entry : std::filesystem::directory_iterator(m_path))
    {
        if (std::filesystem::is_regular_file(entry.path()))
        {
            // Add the file path and size to the resources vector
            resources.push_back({ entry.path().string(), entry.file_size() });
        }
    }

    return resources;
}

char* baselib::util::open_file(std::string file_name, std::streampos& file_size, bool& result)
{
    // Open the file for reading
    HANDLE file_handle = CreateFileA(file_name.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (file_handle != INVALID_HANDLE_VALUE)
    {
        // Get the file size
        file_size = GetFileSize(file_handle, nullptr);

        // Allocate memory for the file content
        char* buffer = new char[file_size];

        DWORD bytes_read;
        BOOL success = ReadFile(file_handle, buffer, file_size, &bytes_read, nullptr);
        CloseHandle(file_handle);

        if (success && bytes_read == file_size)
        {
            // File reading successful


            result = true;
            return buffer;
        }

        delete[] buffer;

    }
    return nullptr;
}