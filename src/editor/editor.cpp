#include "editor.h"
#include "src/flecs_modules/transformation/transformation.h"

#include <iostream>
#include <functional>
#include <refl.hpp>

editor::editor() {
    hierarchy = std::make_unique<scene_hierarchy>();
}

void editor::update(ImGuiIO& imgui_io, flecs::world& registry)
{
    ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);

    if(ImGui::BeginMainMenuBar())
    {
	    if(ImGui::BeginMenu("File"))
	    {
		    
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
}

