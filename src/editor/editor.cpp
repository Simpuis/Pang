#include "editor.h"
#include "src/flecs_modules/transformation/transformation.h"

#include <iostream>
#include <functional>
#include <refl.hpp>

void editor::update(ImGuiIO& imgui_io, flecs::world& registry)
{
    if(ImGui::BeginMainMenuBar())
    {
	    if(ImGui::BeginMenu("File"))
	    {
		    
			ImGui::EndMenu();
	    }

        if(inspector_element) {
            inspector_element->tick(registry);
        }
		ImGui::EndMainMenuBar();
    }

}