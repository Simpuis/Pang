#pragma once

#include <flecs.h>
#include <refl.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <magic_enum.hpp>

#include "editor_element.h"
#include "src/glm_reflection.h"
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

template<typename T, typename U>
concept component_holder = requires (T a, U b) {
    a.template set<U>(b);
    a.template add<U>();
    a.template has<U>();
    a.template get<U>();
    a.template get_mut<U>();
};

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

    void component_input(flecs::world& world, float& value) {
        ImGui::InputFloat("##value", &value, 0.1f);
    }

    void component_input(flecs::world& world, int& value) {
        ImGui::InputInt("##value", &value, 1);
    }

    void component_input(flecs::world& world, unsigned int& value) {
        int changed_value = (int)value;
        ImGui::InputInt("##value", &changed_value, 1);
        changed_value = std::max(0, changed_value);
        if(changed_value != value) {
            value = changed_value;
        }
    }

    void component_input(flecs::world& world, glm::vec3& value) {
        ImGui::InputFloat3("##value", glm::value_ptr(value));
    }

    void component_input(flecs::world& world, glm::quat& value) {
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
    void component_input(flecs::world& world, std::vector<T>& value) {
        for(auto& t : value) {
            component_input(world, t);
        }
    }

    void component_input(flecs::world& world, std::shared_ptr<mesh>& value) {
        ImGui::Text("Test");
    }

    template<is_enum T>
    void component_input(flecs::world& world, T& value) {
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

    void component_input(flecs::world& world, flecs::entity& value) {
        if(ImGui::BeginCombo("", (value.is_valid()) ? value.name() : "Select entity")) {
            flecs::entity root = world.get<scene_root>()->root_entity;
            std::vector<flecs::entity> scene_entities;
            find_all_children(root, scene_entities);

            for(auto entity : scene_entities) {
                if(ImGui::Selectable(entity.name())) {
                    value = entity;
                }
            }
            ImGui::EndCombo();
        }
    }

    void find_all_children(flecs::entity parent, std::vector<flecs::entity>& entities) {
        entities.push_back(parent);
        parent.children([&](flecs::entity child) {
            find_all_children(child, entities);
        });
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
                            draw_serializables<flecs::world, Serializable_Ts...>(world, world);
                        }
                        else {
                            draw_serializables<flecs::entity, Serializable_Ts...>(world, shared_state.selected_entity);
                        }
                        if(ImGui::Button("Add Component##AddButton"))
                            ImGui::OpenPopup("add_component_popup");
                        if(ImGui::BeginPopup("add_component_popup")) {
                            if(shared_state.selected_entity == world.get<scene_root>()->root_entity) {
                                draw_add_component_options_singleton<Serializable_Ts...>(shared_state.selected_entity);
                            }
                            else {
                                draw_add_component_options<Serializable_Ts...>(shared_state.selected_entity);
                            }
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

    template<typename Component_Holder_T, typename Head_T, typename... Tail_Ts>
        requires component_holder<Component_Holder_T, Head_T>
    void draw_serializables(flecs::world& world, Component_Holder_T& selected, int i = 0) {
        draw_serializable<Head_T>(world, selected, i);
        if constexpr (sizeof...(Tail_Ts) > 0) {
            draw_serializables<Component_Holder_T, Tail_Ts...>(world, selected, i + 1);
        }
    }

    template<typename Head_T, typename... Tail_Ts>
    void draw_add_component_options(flecs::entity selected_entity, int i = 0) {
        const Head_T* component = selected_entity.get<Head_T>();
        if(component) ImGui::BeginDisabled(true);

        constexpr auto type = refl::reflect<Head_T>();
        if constexpr(!refl::descriptor::has_attribute<singleton_component>(refl::reflect<Head_T>())) {
            if (ImGui::Selectable(type.name.c_str())) {
                selected_entity.add<Head_T>();
            }
        }

        if(component) ImGui::EndDisabled();

        if constexpr(sizeof...(Tail_Ts) > 0) {
            draw_add_component_options<Tail_Ts...>(selected_entity, i + 1);
        }
    }

    template<typename Head_T, typename... Tail_Ts>
    void draw_add_component_options_singleton(flecs::entity selected_entity, int i = 0) {
        constexpr auto type = refl::reflect<Head_T>();
        if constexpr(refl::descriptor::has_attribute<singleton_component>(refl::reflect<Head_T>())) {
            const Head_T *component = selected_entity.get<Head_T>();
            if (component) ImGui::BeginDisabled(true);

            if (ImGui::Selectable(type.name.c_str())) {
                selected_entity.add<Head_T>();
            }

            if (component) ImGui::EndDisabled();
        }
        if constexpr(sizeof...(Tail_Ts) > 0) {
            draw_add_component_options_singleton<Tail_Ts...>(selected_entity, i + 1);
        }
    }

    template<typename T, typename Component_Holder_T>
        requires (!simple_pair<T>) &&
        component_holder<Component_Holder_T, T>
    void draw_serializable(flecs::world& world, Component_Holder_T& selected, int i) {
        if(selected.template has<T>()) {
            T* component = selected.template get_mut<T>();
            constexpr auto type = refl::reflect<T>();
            ImGui::SeparatorText(type.name.c_str());
            if(ImGui::BeginPopupContextItem(("Remove Component Popup " + std::to_string(i)).c_str())) {
                if(ImGui::Selectable("Remove Component")) {
                    selected.template remove<T>();
                    ImGui::EndPopup();
                    return;
                }
                ImGui::EndPopup();
            }
            for_each(refl::reflect(*component).members, [&](auto member) {
                if constexpr (is_readable(member) && refl::descriptor::has_attribute<serializable>(member)) {
                    ImGui::Text("%s", get_display_name(member));
                    ImGui::PushID(i);
                    component_input(world, member(*component));
                    ImGui::PopID();
                    ImGui::Separator();
                }
            });
            ImGui::Separator();
        }
    }

    template<typename T>
        requires simple_pair<T>
    void draw_serializable(flecs::world& world, flecs::entity selected_entity, int i) {
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
                    component_input(world, member(*component));
                    ImGui::PopID();
                    ImGui::Separator();
                }
            });
            ImGui::Separator();
        }
    }
};
