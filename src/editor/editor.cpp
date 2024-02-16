#include "editor.h"
#include "src/flecs_modules/transformation/transformation.h"

#include <iostream>
#include <functional>
#include <refl.hpp>

editor::editor() {
    editor_elements.push_back(std::make_unique<file_menu>());
    editor_elements.push_back(std::make_unique<scene_hierarchy>());
    editor_elements.push_back(std::make_unique<transformation_gizmo>());
}

void editor::update(ImGuiIO& imgui_io, flecs::world& registry)
{
    ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);

    for(auto& element : editor_elements) {
        element->tick(registry, shared_state);
    }
}

