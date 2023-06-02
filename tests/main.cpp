#include <secure_resource.h>

#pragma comment(lib, "baselib")
#pragma comment(lib, "secure-resource-system.lib")

void save_resource_tests()
{
    resource::create_directory("image");

    resource::create_resource_in_directory("image", "base_resource\\image\\loading.gif", "loading.gif");
    resource::create_resource_in_directory("image", "base_resource\\image\\tt.gif", "tt.gif");
    resource::create_resource_in_directory("image", "base_resource\\image\\logo.png", "logo.png");
    resource::create_resource_in_directory("image", "base_resource\\image\\text.png", "text.png");
    resource::save_resources();

    resource::extract_resources();
}

void load_resource_tests()
{
    resource::load_resources_main();

    resource::extract_resources();
}

int main()
{
    resource::initialize();

    load_resource_tests();

    //save_resource_tests();

    system("pause");
}

