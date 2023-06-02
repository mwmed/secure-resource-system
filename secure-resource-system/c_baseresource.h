#pragma once
#include "c_resource.h"






namespace resource {
	class c_baseresource : public c_resource
	{
	public:


		// Inherited via c_resource
		virtual c_array write_data() override;

		virtual c_array write_main_data() override;

	};
}