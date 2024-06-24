#pragma once
#include <iostream>
#include <ECS.h>
#include <filesystem>
#include <string>
#include <vector>
#include <optional>

#include "InspectorUI.h"
#include "MainBarUI.h"
#include "SceneUI.h"
#include "FilesUI.h"
#include "AssetsUI.h"
#include "GameSettingsUI.h"
#include "HierarchyUI.h"
#include "MenuUI.h"
#include "UINotification.h"

namespace fs = std::filesystem;

class UIAdministrator
{
public:
    static void draw_ui(Entity *owner);
};
