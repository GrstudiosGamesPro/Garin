#include <GCamera.h>
#include <RenderPipeline.h>
#include <GarinMaths.h>

void GCamera::init()
{
    a_camera = new Camera();
    a_camera->target_render = RenderPipeline::add_render_texture();
}

void GCamera::update()
{
    if (a_camera != nullptr)
    {
        a_camera->cameraPosition = entity->get_transform()->Position;
        glm::vec3 eulerAngles = entity->get_transform()->get_euler_angles();

        float pitch = eulerAngles.x;
        float yaw = eulerAngles.y;
        float roll = eulerAngles.z;

        glm::quat qPitch = glm::angleAxis(pitch, glm::vec3(1, 0, 0));
        glm::quat qYaw = glm::angleAxis(yaw, glm::vec3(0, 1, 0));
        glm::quat qRoll = glm::angleAxis(roll, glm::vec3(0, 0, 1));

        glm::quat qRotation = qRoll * qYaw * qPitch;

        glm::vec3 newOrientation = glm::normalize(glm::mat3_cast(qRotation) * a_camera->Orientation);
        a_camera->cameraRotation = get_transform()->rotation;
    }
}

void GCamera::clean()
{
    auto &targets = RenderPipeline::camera_targets;
    targets.erase(std::remove(targets.begin(), targets.end(), a_camera), targets.end());
}