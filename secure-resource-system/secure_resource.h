#pragma once
#include "baselib_includes.h"
#include "c_resource.h"




namespace resource {
	void initialize();

	void create_directory(std::string directory_name);
	void create_resource_in_directory(std::string directory_name, std::string resource_path, std::string resource_name);

	void save_resources();
	void extract_resources();

	bool load_resources_main();

	c_resource* get_resource(std::string directory_name, std::string resource_name);
}