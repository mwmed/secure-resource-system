#include "secure_resource.h"
#include "c_resourcesystem.h"





namespace resource 
{
	c_resourcesystem* resourcesystem = nullptr;

	void initialize()
	{
		resourcesystem = new c_resourcesystem();

	}
	void create_directory(std::string directory_name)
	{
		resourcesystem->create_directory(directory_name);
	}
	bool load_resources_main()
	{
		return resourcesystem->load_resources_from_main_file();
	}
	void create_resource_in_directory(std::string directory_name, std::string resource_path, std::string resource_name)
	{
		resourcesystem->add_resource_to_directory(directory_name, resource_path, resource_name);
	}
	void save_resources()
	{
		if (!std::filesystem::exists("resources"))
			std::filesystem::create_directory("resources");
		resourcesystem->save_main_resource();
		resourcesystem->save_resources();
	}
	void extract_resources()
	{
		// Create a directory to store extracted resources
		std::string extraction_path = "resources_extracted";
		if (!std::filesystem::exists(extraction_path))
			std::filesystem::create_directory(extraction_path);

		// Extract resources from each directory
		for (auto m_directory : resourcesystem->m_directories)
		{
			printf("Extracting Folder %s \n", m_directory.second->directory_name.c_str());

			// Create a directory for the current resource folder
			std::string current_folder = extraction_path + "\\" + m_directory.second->directory_name;
			if (!std::filesystem::exists(current_folder))
				std::filesystem::create_directory(current_folder);

			// Extract resources within the current folder
			for (auto m_res : m_directory.second->m_resources)
			{
				std::string resource_path = current_folder + "\\" + m_res.second->resource_path;

				printf("Extracting resource %s \n", resource_path.c_str());

				// Create the resource file and write the resource data
				util::create_if_not_exists(resource_path);
				util::write_to_file(resource_path, (char*)m_res.second->resource_data, m_res.second->resource_size);
			}
		}
	}
}