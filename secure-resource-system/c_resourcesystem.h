#pragma once
#include "c_resourcedirectory.h"





namespace resource {
	class c_resourcesystem
	{
	public:
		inline c_resourcesystem(){}

	public:
		void create_directory(std::string directory_name);
		bool add_resource_to_directory(std::string directory_name, std::string resource_path, std::string resource_name);

	public:
		void save_main_resource();
		void save_resources();
		bool load_resources_from_main_file();

	public:
		std::map<std::string, c_resourcedirectory*> m_directories;

		std::map<std::string, c_resource*> m_resources;
	};
}