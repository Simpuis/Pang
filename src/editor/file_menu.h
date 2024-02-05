#pragma once

#include <nfd.h>

#include "editor_element.h"
#include "src/serialization/scene_manager.h"

class file_menu : public editor_element {
public:
    void tick(flecs::world& world, shared_editor_state& shared_state) override {
        if(ImGui::MenuItem("New Scene")) {
            world.set<new_scene_command>(new_scene_command());
        }
        if(ImGui::MenuItem("Open Scene")) {
            auto result = get_file_path();
            if(result.has_value()) {
                world.set<scene_swap_command>({result.value()});
            }
        }
        if(ImGui::MenuItem("Save Scene")) {
            world.set<scene_save_command>({"blahaj.gltf"});
        }
    }

    static std::optional<std::string> get_file_path() {
        nfdchar_t *out_path = nullptr;
        nfdresult_t result = NFD_OpenDialog(nullptr, nullptr, &out_path);

        if(result == NFD_OKAY) {
            return out_path;
        }
        else {
            return {};
        }
    }
};