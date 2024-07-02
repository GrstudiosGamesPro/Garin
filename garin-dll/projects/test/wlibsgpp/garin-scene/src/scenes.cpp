#include "../includes/scenes.h"
#include "VarVerify.h"

using namespace nlohmann;

void scenes::init()
{
    physic_world = new PhysicsEngine();
    physic_world->start_world_physics();

    // VIRTUAL START
    on_start();
}

void scenes::update(float delta_time)
{
    physic_world->update_world_physics();
    for (Entity *ent : objects_worlds)
    {
        ent->update();
    }

    // VIRTUAL UPDATE
    on_update(delta_time);
}

Entity *scenes::make_entity()
{
    Entity *entity_maked = new Entity();
    std::cout << "New entity created" << std::endl;
    entity_maked->objectID = IDGenerator::generate_id();

    objects_worlds.push_back(entity_maked);

    return entity_maked;
}

Entity *scenes::get_entity_by_index(int index)
{
    Entity *object_get = objects_worlds[index];

    if (objects_worlds[index] == nullptr)
    {
        std::cout << "Object index not found return nullptr" << std::endl;
        return nullptr;
    }

    return object_get;
}

Entity *scenes::get_entity_by_id(int id)
{
    for (Entity *ent : objects_worlds)
    {
        if (ent->objectID == id)
        {
            return ent;
        }
    }

    return nullptr;
}

Entity *scenes::get_entity_by_name(std::string name)
{
    for (Entity *ent : objects_worlds)
    {
        if (ent->ObjectName == name)
        {
            return ent;
        }
    }

    return nullptr;
}

void scenes::load_scene(std::string scene_name_new)
{
    for (Entity *ent : objects_worlds)
    {
        ent->ClearAllComponentes();
    }

    objects_worlds.clear();
    scene_name = scene_name_new;

    std::string file_path = FileManager::get_execute_path() + "projects/test/assets/" + scene_name_new + ".scene";
    std::ifstream file(file_path);
    if (!file.is_open())
    {
        std::cerr << "Error opening file: " << file_path << std::endl;
        return;
    }

    json data_loaded;
    file >> data_loaded;

    // Verificar que data_loaded sea un array
    if (!data_loaded.is_array())
    {
        std::cerr << "Error: data_loaded is not an array" << std::endl;
        return;
    }

    for (size_t i = 0; i < data_loaded.size(); i++)
    {
        Entity *new_object = make_entity();
        VarVerify::set_value_if_exists(data_loaded[i], "name", new_object->ObjectName);

        auto transform = new_object->get_transform();
        VarVerify::set_value_if_exists(data_loaded[i], "px", transform->Position.x);
        VarVerify::set_value_if_exists(data_loaded[i], "py", transform->Position.y);
        VarVerify::set_value_if_exists(data_loaded[i], "pz", transform->Position.z);
        VarVerify::set_value_if_exists(data_loaded[i], "rx", transform->rotation.x);
        VarVerify::set_value_if_exists(data_loaded[i], "ry", transform->rotation.y);
        VarVerify::set_value_if_exists(data_loaded[i], "rz", transform->rotation.z);
        VarVerify::set_value_if_exists(data_loaded[i], "sx", transform->Scale.x);
        VarVerify::set_value_if_exists(data_loaded[i], "sy", transform->Scale.y);
        VarVerify::set_value_if_exists(data_loaded[i], "sz", transform->Scale.z);
        VarVerify::set_value_if_exists(data_loaded[i], "object_id", new_object->objectID);

        if (data_loaded[i].contains("components") && data_loaded[i]["components"].is_array())
        {
            json cmps = data_loaded[i]["components"];
            for (size_t e = 0; e < cmps.size(); e++)
            {
                if (cmps[e].contains("name"))
                {
                    std::string component_name = cmps[e]["name"];
                    ComponentFactory::add_component(new_object, component_name);
                }
            }
        }
        else
        {
            std::cerr << "Warning: 'components' is missing or not an array in entity " << i << std::endl;
        }
    }

    // ATTACH WITH PARENTS
    for (size_t i = 0; i < data_loaded.size(); i++)
    {
        int objectID = -1;
        int parentID = -1;

        VarVerify::set_value_if_exists(data_loaded[i], "object_id", objectID);
        VarVerify::set_value_if_exists(data_loaded[i], "parent_id", parentID);

        if (objectID != 1 && parentID != -1)
        {
            Entity *new_object_child = get_entity_by_id(objectID);
            Entity *new_object_parent = get_entity_by_id(parentID);
            new_object_child->get_transform()->parent = new_object_parent->get_transform();

            new_object_parent->get_transform()->childrens.push_back(new_object_child->get_transform());
        }
    }

    std::cout << "Loaded Scene: " << scene_name_new << std::endl;
}

void scenes::save_scene()
{
    try
    {
        json object;

        for (size_t i = 0; i < objects_worlds.size(); i++)
        {
            Entity *ent = objects_worlds[i];

            json dataOBJ;
            dataOBJ["name"] = ent->ObjectName;
            dataOBJ["px"] = ent->get_transform()->Position.x;
            dataOBJ["py"] = ent->get_transform()->Position.y;
            dataOBJ["pz"] = ent->get_transform()->Position.z;

            dataOBJ["rx"] = ent->get_transform()->rotation.x;
            dataOBJ["ry"] = ent->get_transform()->rotation.y;
            dataOBJ["rz"] = ent->get_transform()->rotation.z;

            dataOBJ["sx"] = ent->get_transform()->Scale.x;
            dataOBJ["sy"] = ent->get_transform()->Scale.y;
            dataOBJ["sz"] = ent->get_transform()->Scale.z;

            dataOBJ["object_id"] = ent->objectID;

            if (ent->get_transform()->parent != nullptr)
                dataOBJ["parent_id"] = ent->get_transform()->parent->entity->objectID;

            json components = json::array();

            std::vector<Component *> cmpms = ent->getComponents<Component>();

            for (Component *component : cmpms)
            {
                json comp;
                std::string className = demangle(typeid(*component).name());

                comp["name"] = className;
                // comp["data"] = component->serialize();
                for (const auto &[key, value] : component->variableMap)
                {
                    if (value.type() == typeid(std::string))
                    {
                        std::string val = std::any_cast<std::string>(value);
                        comp[key] = val;
                    }
                    else if (value.type() == typeid(int))
                    {
                        int val = std::any_cast<int>(value);
                        comp[key] = val;
                    }
                    else if (value.type() == typeid(float))
                    {
                        float val = std::any_cast<float>(value);
                        comp[key] = val;
                    }
                    else if (value.type() == typeid(bool))
                    {
                        bool val = std::any_cast<bool>(value);
                        comp[key] = val;
                    }
                }

                components.push_back(comp);
            }

            dataOBJ["components"] = components;

            object.push_back(dataOBJ);
        }

        std::string file_path = FileManager::get_execute_path() + "projects/test/assets/" + scene_name + ".scene";
        FileManager::write_file(file_path, object.dump(4));
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
}