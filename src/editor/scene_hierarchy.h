#pragma once

#include <imgui.h>

#include "editor_element.h"
#include "src/flecs_modules/transformation/transformation.h"
#include "src/serialization/scene_serializer.h"

class scene_hierarchy : public editor_element {
    void tick(flecs::world& world, shared_editor_state& shared_state) override {
        if(ImGui::Begin("Hierarchy", nullptr)) {
            ImGui::BeginChild("left pane", ImVec2(0, 0), ImGuiChildFlags_Border);
            if (ImGui::BeginPopupContextItem()) {
                ImGui::EndPopup();
            }
            auto root = world.get<scene_root>()->root_entity;
            traverse_hierarchy(root);

            ImGui::EndChild();
            ImGui::SameLine();
            ImGui::End();
        }

    }

    void traverse_hierarchy(flecs::entity parent) {
        if(ImGui::TreeNode(parent.name())) {
            parent.children([&](flecs::entity child) {
                traverse_hierarchy(child);
            });
            ImGui::TreePop();
        }
    }
};