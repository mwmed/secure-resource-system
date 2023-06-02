#pragma once
#include "c_resource.h"






namespace resource {
	class c_resourcedirectory : public c_resource
	{
	public:
		c_resourcedirectory(std::string p_directory_name = "") : directory_name(p_directory_name) {}

	public: // properties
		std::string directory_name = "";
	public: 
		uint64_t file_counter = 0;

	public: //data
		std::map<std::string, c_resource*> m_resources;
		c_array total_data;

	public:
		bool read_main_data(c_array& obj);
		bool read_data();
		bool read(c_array& obj);

	public:
		// Inherited via c_resource
		virtual c_array write_data() override;

		virtual c_array write_main_data() override;

	};
}