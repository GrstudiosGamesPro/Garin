#include "../includes/gamescene.h"

#include <iostream>
#include <cstdarg>
#include <thread>
#include <SceneData.h>
#include <RenderPipeline.h>

void gamescene::on_start()
{
    old_stdout = std::cout.rdbuf(buffer_stdout.rdbuf());
    old_stderr = std::cerr.rdbuf(buffer_stderr.rdbuf());

    configs = new EditorConfigs();
    camera = new Camera();
    main_camera = camera;

    GarinUI::make_ui_context(Gfx::get_game_window());
    std::cout << "CARPETA DE EJECUCION: " << FileManager::get_execute_path() << std::endl;

    uieditor = new UIEditorManager();
    gizmos = new GizmosDrawer();

    uieditor->configs = configs;
    uieditor->game = this;

    uieditor->setup();

    gizmos->config_line();

    IconsManager::init();

    string info = "OPEN FILE: " + configs->current_scene;

    UINotification::AddNotification(info, 3.0f);
}

void gamescene::on_edition_mode(float delta_time)
{
    if (ImGui::IsMouseDown(ImGuiMouseButton_Right) && configs->project_select)
    {
        camera->move_forward(delta_time, InputSystem::get_axis(GLFW_KEY_W, GLFW_KEY_S) * 0.1f);
        camera->move_left(delta_time, InputSystem::get_axis(GLFW_KEY_A, GLFW_KEY_D) * 0.1f);

        if (glfwGetKey(Gfx::get_game_window(), GLFW_KEY_Q) == GLFW_PRESS)
        {
            camera->cameraPosition.y -= 0.1f;
        }

        if (glfwGetKey(Gfx::get_game_window(), GLFW_KEY_E) == GLFW_PRESS)
        {
            camera->cameraPosition.y += 0.1f;
        }

        glm::vec3 newOrientation = glm::rotate(camera->Orientation, glm::radians(InputSystem::get_mouse_y()), glm::normalize(glm::cross(camera->Orientation, camera->GetUp())));

        if (abs(glm::angle(newOrientation, camera->GetUp()) - glm::radians(90.0f)) <= glm::radians(85.0f))
        {
            camera->Orientation = newOrientation;
        }

        camera->Orientation = glm::rotate(camera->Orientation, glm::radians(-InputSystem::get_mouse_x()), camera->GetUp());
    }

    std::string window_name = "Garin Editor - " + SceneManager::GetOpenScene()->scene_name;

    for (Entity *entity : objects_worlds)
    {
        entity->transform_component->update();
        for (Entity *transform : entity->childrens)
        {
            transform->transform_component->update();
        }
    }

    glfwSetWindowTitle(Gfx::get_game_window(), window_name.c_str());
}

void gamescene::on_update(float delta_time)
{
    uieditor->select_obj = select_obj;
    // GCastHit *hit = new GCastHit();

    // if (GCaster::LineCast(SceneManager::GetOpenScene()->get_entity_by_index(1)->get_transform()->Position, glm::vec3(0.0f, -1.0f, 0.0f), 100, hit, LAYER_1))
    // {
    //     std::cout << "Casting: " << hit->entity->ObjectName << std::endl;
    // }
}

void gamescene::on_draw()
{
    gizmos->draw_line(glm::vec3(0.0f), glm::vec3(20.0f, 2.0f, 0.0f), glm::vec3(0.0f), glm::vec3(5.0f));
}

void gamescene::draw_ui()
{
    GarinUI::get_ui_manager()->render_new_frame_ui_context(true);

    uieditor->draw();

    if (configs->project_select == true)
    {
        ReadBuffer(buffer_stdout, stdout_buffer);
        ReadBuffer(buffer_stderr, stderr_buffer);

        // notify->RenderNotifications();
        UINotification::instance.RenderNotifications();

        ImGui::Begin("Console");
        std::lock_guard<std::mutex> guard(mutex);

        // if (ImGui::Button("Clear Console"))
        // {
        //     Clear();
        // }

        ImGui::BeginChild("ScrollingRegion", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
        ImGui::TextUnformatted(stdout_buffer.c_str());
        ImGui::TextUnformatted(stderr_buffer.c_str());

        if (shouldScroll)
        {
            ImGui::SetScrollHereY(1.0f);
            shouldScroll = false;
        }
        ImGui::EndChild();

        if (ImGui::IsKeyDown(ImGuiKey_LeftCtrl))
        {
            if (ImGui::IsKeyReleased(ImGuiKey_S))
            {
                if (!AppSettings::is_playing)
                {
                    SceneData::save_scene();

                    string info = "Scene Saved: " + configs->current_scene;

                    UINotification::AddNotification(info, 3.0f);
                }
                else
                {
                    string info = "You cannot save scene in playmode";
                    UINotification::AddNotification(info, 3.0f);
                }
            }
        }

        ImGui::End();
    }

    GarinUI::get_ui_manager()->render_ui_context();
}

void gamescene::on_destroy()
{
    configs->save_config();
}

void gamescene::set_object_select(Entity *obj)
{
    select_obj = obj;
}