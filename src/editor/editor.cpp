#include "editor.h"

void editor::update(ImGuiIO& imgui_io, flecs::world& registry)
{
    if(ImGui::BeginMainMenuBar())
    {
	    if(ImGui::BeginMenu("File"))
	    {
		    
			ImGui::EndMenu();
	    }

		ImGui::EndMainMenuBar();
    }
}
