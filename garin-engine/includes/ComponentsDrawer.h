#pragma once
#include "AComponent.h"
#include "CModel.h"
#include "CRigidbody.h"
#include "CCollider.h"
#include "CGScript.h"
#include "CMaterial.h"
#include "CCharacter.h"
#include <ECS.h>
#include <iostream>
#include <imgui.h>
#include <memory>
#include <sstream>

class ComponentsDrawer
{
public:
    template <typename T>
    static void updateUI(Entity *owner, int id, AComponent *cmp)
    {
        if (owner->hasComponent<T>())
        {
            auto modelComponents = owner->getComponents<T>();
            for (auto *cmap : modelComponents)
            {
                EditorGUI::Draw_Component<T>(owner, cmap, demangle(typeid(cmap).name()), cmap->component_id, cmap, [cmap]() {});
            }
            // cmp->draw(owner);
        }
    }

    static std::string demangle(const char *mangled_name)
    {
        std::string demangled_name = mangled_name;

        // Remover prefijo de clase
        const std::string class_prefix = "class ";
        if (demangled_name.find(class_prefix) == 0)
        {
            demangled_name = demangled_name.substr(class_prefix.size());
        }

        // Remover prefijo de struct
        const std::string struct_prefix = "struct ";
        if (demangled_name.find(struct_prefix) == 0)
        {
            demangled_name = demangled_name.substr(struct_prefix.size());
        }

        // Remover sufijo de puntero __ptr64
        const std::string ptr64_suffix = " *__ptr64";
        size_t pos = demangled_name.find(ptr64_suffix);
        if (pos != std::string::npos)
        {
            demangled_name = demangled_name.substr(0, pos);
        }

        // Remover sufijo de puntero
        const std::string ptr_suffix = " *";
        pos = demangled_name.find(ptr_suffix);
        if (pos != std::string::npos)
        {
            demangled_name = demangled_name.substr(0, pos);
        }

        return demangled_name;
    }
};
