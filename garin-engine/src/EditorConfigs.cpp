#include "../includes/EditorConfigs.h"
#include <GarinGraphics.h>

#include <VarVerify.h>
#include <GarinIO.h>

using namespace nlohmann;

void EditorConfigs::save_config()
{
    json settings;
    settings["current_scene"] = SceneManager::GetOpenScene()->scene_name;
    settings["start_scene"] = start_scene;
    settings["camera_state"] = SceneManager::GetOpenScene()->main_camera->use_projection;

    FileManager::write_file(current_proyect + "/EngineSettings.data", settings.dump(4));
}

void EditorConfigs::load_config()
{
    json settings = json::parse(FileManager::read_file(current_proyect + "/EngineSettings.data"));
    std::cout << "JSON CONFIG DATA: " << settings.dump(4) << std::endl;
    current_scene = (std::string)settings["current_scene"];
    start_scene = (std::string)settings["start_scene"];

    if (SceneManager::GetOpenScene()->main_camera == nullptr)
    {
        std::cout << "Main camera its null" << std::endl;
        return;
    }
    else
    {
        std::cout << "Loading camera config" << std::endl;
    }

    VarVerify::set_value_if_exists(settings, "camera_state", SceneManager::GetOpenScene()->main_camera->use_projection);
}