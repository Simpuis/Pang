#include "editor.h"
#include "src/flecs_modules/transformation/transformation.h"

#include <iostream>
#include <functional>

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

    if(ImGui::Begin("Inspector", nullptr, ImGuiWindowFlags_MenuBar)) {
        static flecs::entity selected;
        {
            ImGui::BeginChild("left pane", ImVec2(150, 0), ImGuiChildFlags_Border | ImGuiChildFlags_ResizeX);
            registry.each<position>([&](flecs::entity e, position &pos) {
                if (e.name().size() > 0) {
                    if (ImGui::Selectable(e.name(), e == selected)) {
                        selected = e;
                    }
                }
            });
            ImGui::EndChild();
        }
        ImGui::SameLine();

        std::function<std::string(std::string)> process_json_desc = [](std::string s) {
            size_t pos = s.find(',');
            while(pos != std::string::npos) {
                s.insert(pos + 1, "\n");
                pos = s.find(',', pos + 1);
            }
            return s;
        };
        if(selected) {
            ImGui::BeginGroup();
            ImGui::BeginChild("item view", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()));
            ImGui::Text("%s", selected.name().c_str());
            ImGui::Separator();
            if(ImGui::BeginTabBar("#Tabs", ImGuiTabBarFlags_None)) {
                if(ImGui::BeginTabItem("Inspect")) {
                    flecs::entity_to_json_desc_t desc;
                    desc.serialize_values = true;
                    ImGui::Text("%s", process_json_desc(std::string(selected.to_json(&desc).c_str())).c_str());
                    ImGui::EndTabItem();
                }
                ImGui::EndTabBar();
            }
            ImGui::EndChild();
            ImGui::EndGroup();
        }
        ImGui::End();
    }
}
