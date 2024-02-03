#pragma once

#include <flecs.h>
#include <refl.hpp>
#include <iostream>

#include "editor_element.h"
#include "src/flecs_modules/transformation/transformation.h"
#include "src/flecs_modules/rendering/rendering.h"
#include "src/serialization/serializable.h"

template<typename... Serializable_Ts>
class inspector : public editor_element {
    void tick(flecs::world& world) override {
        if(ImGui::Begin("Inspector", nullptr, ImGuiWindowFlags_MenuBar)) {
            static flecs::entity selected;
            {
                ImGui::BeginChild("left pane", ImVec2(150, 0), ImGuiChildFlags_Border | ImGuiChildFlags_ResizeX);
                world.each<position>([&](flecs::entity e, position &pos) {
                    if (e.name().size() > 0) {
                        if (ImGui::Selectable(e.name(), e == selected)) {
                            selected = e;
                        }
                    }
                });
                ImGui::EndChild();
            }
            ImGui::SameLine();

            static bool has_changed_entity = false;
            if(selected) {
                ImGui::BeginGroup();
                ImGui::BeginChild("item view", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()));
                ImGui::Text("%s", selected.name().c_str());
                ImGui::Separator();
                if(ImGui::BeginTabBar("#Tabs", ImGuiTabBarFlags_None)) {
                    if(ImGui::BeginTabItem("Inspect")) {
                        draw_serializables<Serializable_Ts...>(selected);
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
    void draw_serializables(flecs::entity selected_entity) {
        if(Head_T* component = selected_entity.get_mut<Head_T>(); component) {
            for_each(refl::reflect(*component).members, [&](auto member) {
                if constexpr (is_readable(member) && refl::descriptor::has_attribute<serializable>(member)) {
                    ImGui::Text("%s", std::to_string(member(*component)).c_str());
                    ImGui::Separator();
                }
            });
            ImGui::Separator();
        }
        if constexpr (sizeof...(Tail_Ts) > 0) {
            draw_serializables<Tail_Ts...>(selected_entity);
        }
    }
};
