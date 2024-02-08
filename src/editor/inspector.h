#pragma once

#include <flecs.h>
#include <refl.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <magic_enum.hpp>

#include "editor_element.h"
#include "src/flecs_modules/transformation/transformation.h"
#include "src/flecs_modules/rendering/rendering.h"
#include "src/serialization/serializable.h"
#include "src/serialization/scene_serializer.h"

template<typename T>
concept simple_pair = requires (T t) { T::first_type(); T::second_type(); };

template<typename T>
concept is_enum = requires (T t) { std::is_enum_v<T>; };

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

    template<typename T>
    void component_input(std::vector<T>& value) {
        for(auto& t : value) {
            component_input(t);
        }
    }

    void component_input(std::shared_ptr<mesh>& value) {
        ImGui::Text("Test");
    }

    template<is_enum T>
    void component_input(T& value) {
        constexpr auto enum_values = magic_enum::enum_values<T>();
        if(ImGui::Button(std::string(magic_enum::enum_name(value)).c_str())) {
            ImGui::OpenPopup("select_enum");
        }
        if(ImGui::BeginPopup("select_enum")) {
            for(int i = 0; i < magic_enum::enum_count<T>(); i++) {
                if(ImGui::Selectable(std::string(magic_enum::enum_name(enum_values[i])).c_str())) {
                    value = enum_values[i];
                }
            }
            ImGui::EndPopup();
        }
    }

    void tick(flecs::world& world, shared_editor_state& shared_state) override {
        if(ImGui::Begin("Inspector", nullptr, ImGuiWindowFlags_MenuBar)) {
            static bool has_changed_entity = false;
            if(world.is_valid(shared_state.selected_entity)) {
                ImGui::BeginGroup();
                ImGui::BeginChild("item view", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()));
                ImGui::Text("%s", shared_state.selected_entity.name().c_str());
                ImGui::Separator();
                if(ImGui::BeginTabBar("#Tabs", ImGuiTabBarFlags_None)) {
                    if(ImGui::BeginTabItem("Inspect")) {
                        if(shared_state.selected_entity == world.get<scene_root>()->root_entity) {
                            draw_singleton_serializables<Serializable_Ts...>(world);
                        }
                        else {
                            draw_serializables<Serializable_Ts...>(shared_state.selected_entity);
                        }
                        if(ImGui::Button("Add Component##AddButton"))
                            ImGui::OpenPopup("add_component_popup");
                        if(ImGui::BeginPopup("add_component_popup")) {
                            draw_add_component_options<Serializable_Ts...>(shared_state.selected_entity);
                            ImGui::EndPopup();
                        }
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
        draw_serializable<Head_T>(selected_entity, i);
        if constexpr (sizeof...(Tail_Ts) > 0) {
            draw_serializables<Tail_Ts...>(selected_entity, i + 1);
        }
    }

    template<typename Head_T, typename... Tail_Ts>
    void draw_singleton_serializables(flecs::world& world, int i = 0) {
        if(world.has<Head_T>()) {
            Head_T* component = world.get_mut<Head_T>();
            constexpr auto type = refl::reflect<Head_T>();
            ImGui::Text("%s", type.name);
            if (ImGui::BeginPopupContextItem(("Remove Component Popup " + std::to_string(i)).c_str())) {
                if (ImGui::Selectable("Remove Component")) {
                    world.remove<Head_T>();
                    ImGui::EndPopup();
                    return;
                }
                ImGui::EndPopup();
            }
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
        if constexpr(sizeof...(Tail_Ts) > 0) {
            draw_singleton_serializables<Tail_Ts...>(world, i + 1);
        }
    }

    template<typename Head_T, typename... Tail_Ts>
    void draw_add_component_options(flecs::entity selected_entity, int i = 0) {
        const Head_T* component = selected_entity.get<Head_T>();
        if(component) ImGui::BeginDisabled(true);

        constexpr auto type = refl::reflect<Head_T>();
        if(ImGui::Selectable(type.name.c_str())) {
            selected_entity.add<Head_T>();
        }

        if(component) ImGui::EndDisabled();

        if constexpr(sizeof...(Tail_Ts) > 0) {
            draw_add_component_options<Tail_Ts...>(selected_entity, i + 1);
        }
    }

    template<typename T>
        requires (!simple_pair<T>)
    void draw_serializable(flecs::entity selected_entity, int i) {
        if(selected_entity.has<T>()) {
            T* component = selected_entity.get_mut<T>();
            constexpr auto type = refl::reflect<T>();
            ImGui::SeparatorText(type.name.c_str());
            if(ImGui::BeginPopupContextItem(("Remove Component Popup " + std::to_string(i)).c_str())) {
                if(ImGui::Selectable("Remove Component")) {
                    selected_entity.remove<T>();
                    ImGui::EndPopup();
                    return;
                }
                ImGui::EndPopup();
            }
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
    }

    template<typename T>
        requires simple_pair<T>
    void draw_serializable(flecs::entity selected_entity, int i) {
        if(selected_entity.has<T::first_type, T::second_type>()) {
            auto* component = selected_entity.get_mut<T::first_type, T::second_type>();
            constexpr auto type1 = refl::reflect<T::first_type>();
            constexpr auto type2 = refl::reflect<T::second_type>();
            ImGui::Text("%s, %s", type1.name, type2.name);
            if(ImGui::BeginPopupContextItem(("Remove Component Popup " + std::to_string(i)).c_str())) {
                if(ImGui::Selectable("Remove Component")) {
                    selected_entity.remove<T::first_type, T::second_type>();
                    ImGui::EndPopup();
                    return;
                }
                ImGui::EndPopup();
            }
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
    }
};
