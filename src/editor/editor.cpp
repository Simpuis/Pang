#include "editor.h"

void editor::update(ImGuiIO& imgui_io, entt::registry& registry)
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
