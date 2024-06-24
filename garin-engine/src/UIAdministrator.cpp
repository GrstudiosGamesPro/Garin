#include "../includes/UIAdministrator.h"
#include "../includes/ComponentsDrawer.h"
#include <GarinComponents.h>

void UIAdministrator::draw_ui(Entity *owner)
{
    ComponentsDrawer::updateUI<ModelComponent>(owner, 0, new CModel());
}