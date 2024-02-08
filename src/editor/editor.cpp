#include "editor.h"
#include "src/flecs_modules/transformation/transformation.h"

#include <iostream>
#include <functional>
#include <refl.hpp>

editor::editor() {
    file_menu_element = std::make_unique<file_menu>();
    hierarchy = std::make_unique<scene_hierarchy>();
    gizmo = std::make_unique<transformation_gizmo>();
}

void editor::update(ImGuiIO& imgui_io, flecs::world& registry)
{
    ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);

    if(ImGui::BeginMainMenuBar())
    {
	    if(ImGui::BeginMenu("File"))
	    {
            file_menu_element->tick(registry, shared_state);
			ImGui::EndMenu();
	    }

		ImGui::EndMainMenuBar();
    }

    if(inspector_element) {
        inspector_element->tick(registry, shared_state);
    }

    if(hierarchy) {
        hierarchy->tick(registry, shared_state);
    }

    if(gizmo) {
        gizmo->tick(registry, shared_state);
    }
}

