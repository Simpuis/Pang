#pragma once

#include <nfd.h>

#include "editor_element.h"
#include "src/serialization/scene_manager.h"

class file_menu : public editor_element {
public:
    void tick(flecs::world& world, shared_editor_state& shared_state) override {
        if(ImGui::BeginMainMenuBar())
        {
            if(ImGui::BeginMenu("File"))
            {
                if(ImGui::MenuItem("New Scene")) {
                    world.set<new_scene_command>(new_scene_command());
                }
                if(ImGui::MenuItem("Open Scene")) {
                    auto result = get_load_path();
                    if(result.has_value()) {
                        world.set<scene_swap_command>({result.value(), true});
                    }
                    shared_state.selected_entity.reset();
                }
                if(ImGui::MenuItem("Save Scene As...")) {
                    auto result = get_save_path();
                    if(result.has_value()) {
                        world.set<scene_save_command>({result.value()});
                    }
                }
                ImGui::EndMenu();
            }

            ImGui::Separator();
            if(ImGui::Button("Play")) {
                world.set<switch_play_command>({false});
            }
            if(ImGui::Button("Stop")) {
                world.set<switch_play_command>({true});
            }

            ImGui::EndMainMenuBar();
        }
    }

    static std::optional<std::string> get_save_path() {
        nfdchar_t *save_path = nullptr;
        nfdresult_t result = NFD_SaveDialog("gltf", nullptr, &save_path);
        if(result == NFD_OKAY) {
            return save_path;
        }
        else {
            return {};
        }
    }

    static std::optional<std::string> get_load_path() {
        nfdchar_t *out_path = nullptr;
        nfdresult_t result = NFD_OpenDialog("gltf", nullptr, &out_path);

        if(result == NFD_OKAY) {
            return out_path;
        }
        else {
            return {};
        }
    }
};