#pragma once

#include <flecs.h>
#include <refl.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "editor_element.h"
#include "src/flecs_modules/transformation/transformation.h"
#include "src/flecs_modules/rendering/rendering.h"
#include "src/serialization/serializable.h"

template<typename... Serializable_Ts>
class inspector;

template<typename T>
concept is_stringable = requires(T a) { std::to_string(a); };

template<typename... Serializable_Ts>
class inspector : public editor_element {
    template<is_stringable T>
    std::string to_string(T value) {
        return std::to_string(value);
    }

    std::string to_string(glm::vec3 value) {
        return std::to_string(value.x) + ", " + std::to_string(value.y) + ", " + std::to_string(value.z);
    }

    std::string to_string(glm::quat value) {
        return std::to_string(value.x) + ", " + std::to_string(value.y) + ", " + std::to_string(value.z) + ", " + std::to_string(value.w);
    }

    void component_input(float& value) {
        ImGui::InputFloat("##value", &value, 0.1f);
    }

    void component_input(int& value) {
        ImGui::InputInt("##value", &value, 1);
    }

    void component_input(unsigned int& value) {
        int changed_value = (int)value;
        ImGui::InputInt("##value", &changed_value, 1);
        changed_value = std::max(0, changed_value);
        if(changed_value != value) {
            value = changed_value;
        }
    }

    void component_input(glm::vec3& value) {
        ImGui::InputFloat3("##value", glm::value_ptr(value));
    }

    void component_input(glm::quat& value) {
        glm::vec3 previous_euler_angles = glm::degrees(eulerAngles(value));
        glm::vec3 euler_angles = previous_euler_angles;
        ImGui::InputFloat3("##value", glm::value_ptr(euler_angles));
        if(previous_euler_angles != euler_angles) {
            if(abs(euler_angles.x) > 180.0f) {
                euler_angles.x = fmod(euler_angles.x, 180.0f);
            }
            if(abs(euler_angles.y) > 180.0f) {
                euler_angles.y = fmod(euler_angles.y, 180.0f);
            }
            if(abs(euler_angles.z) > 180.0f) {
                euler_angles.z = fmod(euler_angles.z, 180.0f);
            }
            value = glm::quat(glm::radians(euler_angles));
        }
    }

    void tick(flecs::world& world, shared_editor_state& shared_state) override {
        if(ImGui::Begin("Inspector", nullptr, ImGuiWindowFlags_MenuBar)) {
            static bool has_changed_entity = false;
            if(shared_state.selected_entity) {
                ImGui::BeginGroup();
                ImGui::BeginChild("item view", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()));
                ImGui::Text("%s", shared_state.selected_entity.name().c_str());
                ImGui::Separator();
                if(ImGui::BeginTabBar("#Tabs", ImGuiTabBarFlags_None)) {
                    if(ImGui::BeginTabItem("Inspect")) {
                        draw_serializables<Serializable_Ts...>(shared_state.selected_entity);
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

    template<typename Head_T, typename... Tail_Ts>
    void draw_serializables(flecs::entity selected_entity, int i = 0) {
        if(Head_T* component = selected_entity.get_mut<Head_T>(); component) {
            constexpr auto type = refl::reflect<Head_T>();
            ImGui::Text("%s", type.name);
            for_each(refl::reflect(*component).members, [&](auto member) {
                if constexpr (is_readable(member) && refl::descriptor::has_attribute<serializable>(member)) {
                    ImGui::Text("%s", get_display_name(member));
                    ImGui::PushID(i);
                    component_input(member(*component));
                    ImGui::PopID();
                    ImGui::Separator();
                }
            });
            ImGui::Separator();
        }
        if constexpr (sizeof...(Tail_Ts) > 0) {
            draw_serializables<Tail_Ts...>(selected_entity, i + 1);
        }
    }
};
