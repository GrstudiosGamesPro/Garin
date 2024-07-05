#include "../includes/DynamicLibLoader.h"
#include "ECS.h"
#include <GarinComponents.h>
#include <GarinBehaviours.h>

void DynamicLibLoader::load_components()
{
    try
    {
        if (loader)
        {
            loader.release();
            loader = nullptr;
        }

        loader = std::make_unique<Loader>();
        auto copy_file = [](const std::filesystem::path &from, const std::filesystem::path &to)
        {
            std::ifstream src(from, std::ios::binary);
            std::ofstream dst(to, std::ios::binary);

            dst.clear();

            dst << src.rdbuf();
        };

        loader->free();

        auto from_dll_path = FileManager::get_execute_path() + "packages/GarinGameCore.dll";
        auto dll_path = FileManager::get_execute_path() + "GarinGameCore.dll";

        if (!std::filesystem::exists(from_dll_path))
        {
            return;
        }

        copy_file(from_dll_path, dll_path);

        loader->load(dll_path.c_str());

        typedef void (*FuncType)(GameBehaviourFactory *);

        auto func = (FuncType)loader->get_function<FuncType>("REGISTER_COMPONENTS");

        if (!func)
        {
            std::cout << "Failed to load components" << std::endl;
            return;
        }

        GameBehaviourFactory *factoryPtr = &GameBehaviourFactory::instance();

        func(factoryPtr);

        loader_dll_stamp = std::filesystem::last_write_time(from_dll_path).time_since_epoch().count();

        std::cout << "Components reloaded" << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
}

void DynamicLibLoader::check_components_reload()
{
    const auto from_dll_path = FileManager::get_execute_path() + "packages/GarinGameCore.dll";

    auto stamp = std::filesystem::last_write_time(from_dll_path).time_since_epoch().count();

    if (stamp != loader_dll_stamp)
    {
        std::cout << "Reloading components..." << std::endl;
        load_components();
    }
}

// void DynamicLibLoader::register_component(const std::string &name, std::function<std::shared_ptr<Component>()> factory)
// {
//     factories[name] = factory;
// }

std::shared_ptr<Component> DynamicLibLoader::create_component(const std::string &name) const
{
    auto it = factories.find(name);
    if (it != factories.end())
    {
        return it->second();
    }

    return nullptr;
}

void DynamicLibLoader::reset_component_registry()
{
    // component_registry = new_unique<ComponentRegistry>();

    // component_registry->register_component<MissingComponent>("MissingComponent");
    // component_registry->register_component<Transform>("Transform");
}