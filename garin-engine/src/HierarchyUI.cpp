#include "HierarchyUI.h"

void HierarchyUI::draw(Entity *select_obj)
{
    ImGui::Begin("Hierarchy");

    if (ImGui::IsMouseClicked(1))
    {
        right_click_held = true;
    }

    if (ImGui::IsMouseDown(1) && right_click_held)
    {
        ImGui::OpenPopup("MenuHierarchy");
        right_click_held = false;
    }

    if (ImGui::BeginPopup("MenuHierarchy"))
    {
        if (ImGui::MenuItem("Create New Object"))
        {
            SceneManager::GetOpenScene()->make_entity();
        }

        ImGui::EndPopup();
    }

    for (Entity *ent : SceneManager::GetOpenScene()->objects_worlds)
    {
        if (ent->get_transform()->parent == nullptr)
        {
            drawEntityNode(ent);
        }
    }

    ImGui::End();
}

void HierarchyUI::drawEntityNode(Entity *entity)
{
    ImGui::PushID(entity->objectID);

    bool node_open = false;
    bool selected = (entity == game->select_obj);

    std::string name_tree = entity->ObjectName + " (" + std::to_string(entity->objectID) + ")";

    if (!entity->get_transform()->childrens.empty())
    {
        node_open = ImGui::TreeNodeEx(name_tree.c_str(), ImGuiTreeNodeFlags_OpenOnArrow | (selected ? ImGuiTreeNodeFlags_Selected : 0));

        if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
        {
            if (entity != game->select_obj)
            {
                game->set_object_select(entity);
            }
            else
            {
                if (game->select_obj != nullptr)
                {
                    game->set_object_select(nullptr);
                }
            }
        }

        if (node_open)
        {
            for (TransformComponent *child : entity->get_transform()->childrens)
            {
                drawEntityNode(child->entity);
            }
            ImGui::TreePop();
        }
    }
    else
    {
        if (ImGui::Selectable(name_tree.c_str(), selected))
        {
            if (entity != game->select_obj)
            {
                game->set_object_select(entity);
            }
            else
            {
                if (game->select_obj != nullptr)
                {
                    game->set_object_select(nullptr);
                }
            }
        }
    }

    if (ImGui::BeginDragDropSource())
    {
        ImGui::SetDragDropPayload("ENTITY_CELL", &entity, sizeof(Entity *));
        ImGui::Text("Dragging %s", entity->ObjectName.c_str());
        ImGui::EndDragDropSource();
    }

    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("ENTITY_CELL"))
        {
            IM_ASSERT(payload->DataSize == sizeof(Entity *));
            Entity *droppedEntity = *(Entity **)payload->Data;

            if (droppedEntity->get_transform()->parent)
            {
                auto &siblings = droppedEntity->get_transform()->parent->childrens;
                siblings.erase(std::remove(siblings.begin(), siblings.end(), droppedEntity->get_transform()), siblings.end());
            }

            droppedEntity->get_transform()->parent = entity->get_transform();
            entity->get_transform()->childrens.push_back(droppedEntity->get_transform());
        }
        ImGui::EndDragDropTarget();
    }

    ImGui::PopID();
}
