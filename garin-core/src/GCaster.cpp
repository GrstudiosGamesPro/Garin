#include "../includes/GCaster.h"
#include <GarinGraphics.h>

bool GCaster::RayCast(glm::vec3 RayOrigin, glm::vec3 RayDirection, float Length, GCastHit *hitRay, PxHitFlags hitFlags, PxU32 raycastLayerMask)
{
    try
    {
        if (!hitRay)
        {
            std::cerr << "Error: hitRay is null." << std::endl;
            return false;
        }

        PxVec3 origin(RayOrigin.x, RayOrigin.y, RayOrigin.z);
        PxVec3 dir = PxVec3(glm::normalize(RayDirection).x, glm::normalize(RayDirection).y, glm::normalize(RayDirection).z);

        PxRaycastBuffer hitBuffer;

        auto scene = SceneManager::GetOpenScene();
        if (!scene)
        {
            std::cerr << "Error: current scene is null." << std::endl;
            return false;
        }

        auto physicsWorld = scene->physic_world;
        if (!physicsWorld || !physicsWorld->mScene)
        {
            std::cerr << "Error: physics world or mScene is null." << std::endl;
            return false;
        }

        PxQueryFilterData filterData;
        filterData.data.word0 = raycastLayerMask;

        // std::cout << "Raycast Layer Mask: " << raycastLayerMask << std::endl;

        // std::cout << "Starting raycast..." << std::endl;
        // std::cout << "Ray Origin: (" << RayOrigin.x << ", " << RayOrigin.y << ", " << RayOrigin.z << ")" << std::endl;
        // std::cout << "Ray Direction: (" << RayDirection.x << ", " << RayDirection.y << ", " << RayDirection.z << ")" << std::endl;
        // std::cout << "Ray Length: " << Length << std::endl;

        bool hitCasting = physicsWorld->mScene->raycast(origin, dir, Length, hitBuffer, hitFlags, filterData);

        if (hitCasting)
        {
            std::cout << "Raycast hit something with filter." << std::endl;
            for (PxU32 i = 0; i < hitBuffer.getNbAnyHits(); i++)
            {
                const PxRaycastHit *hit = &hitBuffer.getAnyHit(i);
                if (!hit)
                {
                    std::cerr << "Error: hit is null." << std::endl;
                    continue;
                }

                PxShape *shape = hit->shape;
                if (!shape)
                {
                    std::cerr << "Error: shape is null." << std::endl;
                    continue;
                }

                hitRay->point = glm::vec3(hit->position.x, hit->position.y, hit->position.z);
                hitRay->normal = glm::vec3(hit->normal.x, hit->normal.y, hit->normal.z);

                const char *shapeName = shape->getName();
                if (!shapeName)
                {
                    std::cerr << "Error: shape name is null." << std::endl;
                    continue;
                }

                std::string nombreDelShape(shapeName);
                std::cout << "Object name: " << shapeName << std::endl;

                if (std::all_of(nombreDelShape.begin(), nombreDelShape.end(), ::isdigit))
                {
                    int objectID = std::stoi(nombreDelShape);
                    hitRay->entity = SceneManager::GetOpenScene()->get_entity_by_id_string(nombreDelShape);
                    if (!hitRay->entity)
                    {
                        std::cerr << "Error: entity not found for ID: " << objectID << std::endl;
                    }
                    else
                    {
                        std::cout << "Entity ID: " << objectID << " hit at position (" << hitRay->point.x << ", " << hitRay->point.y << ", " << hitRay->point.z << ")" << std::endl;
                        return true;
                    }
                }
                else
                {
                    std::cerr << "Error: El nombre del shape no es un número válido: " << nombreDelShape << std::endl;
                }
            }
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }

    return false;
}
