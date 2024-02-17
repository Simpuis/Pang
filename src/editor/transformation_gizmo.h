#pragma once

#include <flecs.h>
#include <imgui.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#include "ImGuizmo.h"
#include "editor_element.h"
#include "src/flecs_modules/freefly_camera/freefly.h"
#include "src/flecs_modules/transformation/transformation.h"
#include "src/serialization/scene_serializer.h"

class transformation_gizmo : public editor_element {
public:
    void tick(flecs::world& world, shared_editor_state& shared_state) override {
        if(!shared_state.selected_entity) return;
        if(!shared_state.selected_entity->is_valid()) return;
        if(!shared_state.selected_entity->has<transform_matrix, world_space>()) return;
        if(world.get<scene_root>()->root_entity == shared_state.selected_entity) return;

        static ImGuizmo::OPERATION current_operation(ImGuizmo::TRANSLATE);
        static ImGuizmo::MODE current_mode(ImGuizmo::LOCAL);
        if(ImGui::IsKeyPressed(ImGuiKey_W)) current_operation = ImGuizmo::TRANSLATE;
        if(ImGui::IsKeyPressed(ImGuiKey_E)) current_operation = ImGuizmo::ROTATE;
        if(ImGui::IsKeyPressed(ImGuiKey_R)) current_operation = ImGuizmo::SCALE;
        if(ImGui::IsKeyPressed(ImGuiKey_T)) current_mode = ImGuizmo::WORLD;
        if(ImGui::IsKeyPressed(ImGuiKey_Y)) current_mode = ImGuizmo::LOCAL;

        auto* world_transform = shared_state.selected_entity->get_mut<transform_matrix, world_space>();
        auto parent_transform = (shared_state.selected_entity->parent().has<transform_matrix, local_space>()) ?
                shared_state.selected_entity->parent().get_mut<transform_matrix, local_space>()->transform :
                glm::mat4x4(1.0f);

        glm::mat4x4 view;
        camera cam;
        if(const auto* debug_camera = world.get<render_debug_camera>(); debug_camera->debug_camera) {
            const auto* debug_camera_trans = debug_camera->debug_camera->get<transform_matrix, world_space>();
            const auto* debug_camera_freefly = debug_camera->debug_camera->get<freefly_controller>();
            view = glm::inverse(debug_camera_trans->transform * debug_camera_freefly->local_trans);
            cam = *debug_camera->debug_camera->get<camera>();

            ImGuiIO& io = ImGui::GetIO();
            ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
            ImGuizmo::Manipulate(glm::value_ptr(view),
                                 glm::value_ptr(glm::perspective(glm::radians(cam.field_of_view_degrees),
                                                                 2560.0f / 1440.0f, cam.near_plane_z, cam.far_plane_z)),
                                 current_operation,
                                 current_mode,
                                 glm::value_ptr(world_transform->transform),
                                 nullptr, nullptr);

            glm::mat4 new_local = glm::inverse(parent_transform) * world_transform->transform;
            glm::quat new_rotation;

            glm::vec3 skew;
            glm::vec4 perspective;
            glm::decompose(new_local, shared_state.selected_entity->get_mut<scale>()->vec, new_rotation,
                           shared_state.selected_entity->get_mut<position>()->pos, skew, perspective);

            shared_state.selected_entity->get_mut<rotation>()->rot = new_rotation;
        }
    }
};