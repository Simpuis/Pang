#pragma once

#include <imgui.h>

#include "editor_element.h"
#include "src/flecs_modules/transformation/transformation.h"

class scene_hierarchy : public editor_element {
    void tick(flecs::world& world, shared_editor_state& shared_state) override {
        if(ImGui::Begin("Hierarchy", nullptr)) {
            {
                ImGui::BeginChild("left pane", ImVec2(0, 0), ImGuiChildFlags_Border);
                if(ImGui::BeginPopupContextItem()) {
                    ImGui::EndPopup();
                }
                world.each<position>([&](flecs::entity e, position &pos) {
                    if (e.name().size() > 0) {
                        if (ImGui::Selectable(e.name(), world.is_valid(shared_state.selected_entity) &&
                                                        e == shared_state.selected_entity)) {
                            shared_state.selected_entity = e;
                        }
                    }
                });
                ImGui::EndChild();
            }
            ImGui::SameLine();
            ImGui::End();
        }

    }
};