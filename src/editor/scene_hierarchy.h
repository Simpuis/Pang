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
            traverse_hierarchy(root, shared_state);

            ImGui::EndChild();
            ImGui::SameLine();
            ImGui::End();
        }

    }

    void traverse_hierarchy(flecs::entity parent, shared_editor_state& shared_state) {
        ImGuiTreeNodeFlags tree_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
        if(parent == shared_state.selected_entity) {
            tree_flags |= ImGuiTreeNodeFlags_Selected;
        }
        bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)parent, tree_flags, parent.name(), parent);
        if(ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) {
            shared_state.selected_entity = parent;
        }
        if(node_open) {
            parent.children([&](flecs::entity child) {
                traverse_hierarchy(child, shared_state);
            });
            ImGui::TreePop();
        }
    }
};