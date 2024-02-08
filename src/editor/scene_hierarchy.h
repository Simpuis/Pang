#pragma once

#include <stack>
#include <imgui.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>

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
            auto root = world.get_mut<scene_root>()->root_entity;
            traverse_hierarchy(world, root, root, shared_state);
            ImGui::EndChild();
            ImGui::SameLine();
            ImGui::End();
        }

    }

    void traverse_hierarchy(flecs::world& world, flecs::entity root, flecs::entity entity, shared_editor_state& shared_state) {
        ImGuiTreeNodeFlags tree_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth
                                        | ImGuiTreeNodeFlags_DefaultOpen;
        if(entity == shared_state.selected_entity) {
            tree_flags |= ImGuiTreeNodeFlags_Selected;
        }

        bool entity_has_children = has_children(entity);
        if(!entity_has_children) tree_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

        bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)entity, tree_flags, entity.name(), entity);
        if(ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) {
            shared_state.selected_entity = entity;
        }
        if(ImGui::BeginDragDropSource()) {
            ImGui::SetDragDropPayload("REPARENT", &entity, sizeof(flecs::entity));
            ImGui::Text("%s", entity.name().c_str());
            ImGui::EndDragDropSource();
        }
        if(ImGui::BeginDragDropTarget()) {
            if(const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("REPARENT")) {
                flecs::entity payload_entity = *(const flecs::entity*)payload->Data;
                if(payload_entity != root && !is_recursive_child_of(entity, payload_entity)) {
                    reparent(entity, payload_entity);
                }
            }
        }
        if(ImGui::BeginPopupContextItem()) {
            if(ImGui::Selectable("New Entity as child")) {
                auto new_entity = world.entity();
                new_entity.set_name((std::string("new_entity_") + std::to_string(new_entity.id())).c_str());
                new_entity.child_of(entity);
                ImGui::CloseCurrentPopup();
            }
            if(entity != root) {
                if (ImGui::Selectable("Remove Entity and Children")) {
                    traverse_delete(world, entity);
                    ImGui::CloseCurrentPopup();
                }
                if (ImGui::Selectable("Remove Entity and Reparent")) {
                    std::stack<flecs::entity> children;
                    entity.children([&](flecs::entity entity) {
                        children.push(entity);
                    });
                    while (!children.empty()) {
                        reparent(entity.parent(), children.top());

                        children.pop();
                    }
                    entity.destruct();
                    ImGui::CloseCurrentPopup();
                }
            }
            ImGui::EndPopup();
        }
        if(node_open && entity_has_children) {
            std::vector<flecs::entity> children;
            entity.children([&](flecs::entity child) {
                children.push_back(child);
            });
            for(auto child : children) {
                traverse_hierarchy(world, root, child, shared_state);
            }
            ImGui::TreePop();
        }
    }

    void reparent(const flecs::entity new_parent, flecs::entity child) const {
        glm::mat4 transform = (new_parent.has<transform_matrix, world_space>()) ?
              new_parent.get<transform_matrix, world_space>()->transform :
              glm::mat4x4(1.0f);

        glm::mat4 child_transform = (child.has<transform_matrix, world_space>()) ?
                              child.get<transform_matrix, world_space>()->transform :
                              glm::mat4x4(1.0f);

        auto transform_diff = glm::inverse(transform) * child_transform;

        glm::quat new_rotation;

        glm::vec3 skew;
        glm::vec4 perspective;
        glm::decompose(transform_diff, child.get_mut<scale>()->vec, new_rotation,
                       child.get_mut<position>()->pos, skew, perspective);

        child.get_mut<rotation>()->rot = new_rotation;

        child.remove(flecs::ChildOf);
        child.child_of(new_parent);
    }

    void traverse_delete(flecs::world& world, flecs::entity entity) {
        std::stack<flecs::entity> delete_stack;
        delete_stack.push(entity);
        while(!delete_stack.empty()) {
            auto top = delete_stack.top();
            delete_stack.pop();
            top.children([&](flecs::entity child) {
                delete_stack.push(child);
            });
            top.destruct();
        }
    }

    static bool has_children(flecs::entity entity) {
        bool has_children = false;
        entity.children([&](flecs::entity child) { has_children = true; return; });

        return has_children;
    }

    static bool is_recursive_child_of(flecs::entity potential_child, flecs::entity parent) {
        if(potential_child.parent() == parent) {
            return true;
        }
        else if(potential_child.parent().is_valid()) {
            return is_recursive_child_of(potential_child.parent(), parent);
        }
        else {
            return false;
        }
    }
};