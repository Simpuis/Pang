#pragma once

#include <string>
#include <map>
#include <memory>
#include <variant>
#include <flecs.h>

#include "scene_serializer.h"

class scene_serializer;

struct scene_swap_command {
    // May load by scene index or filepath
    std::variant<unsigned int, std::string> scene;
    bool load_as_editor = false;
};

struct scene_save_command {
    // May optionally provide a new save path, or the loaded file will be overwritten
    std::optional<std::string> save_path;
};

struct new_scene_command {
    bool poo = false;
};

struct switch_play_command {
    bool editor;
};

class scene_manager {
public:
    scene_manager(std::initializer_list<std::pair<unsigned int, std::string>> scene_filenames);

public:
    void update();
    void load_scene(unsigned int scene_index, bool editor);
    void load_scene(std::string scene_filename, bool editor);

    int get_current_scene_index() const;
    scene_serializer& get_serializer() { return this->serializer; }
    flecs::world& get_world() { return main_world; }

    static inline flecs::entity editor_only_onstart;
    static inline flecs::entity editor_only_preupdate;
    static inline flecs::entity editor_only_onupdate;
    static inline flecs::entity editor_only_onstore;
    static inline flecs::entity play_and_editor_onstart;
    static inline flecs::entity play_and_editor_preupdate;
    static inline flecs::entity play_and_editor_onupdate;
    static inline flecs::entity play_and_editor_onstore;

private:
    flecs::entity editor_pipeline;
    flecs::entity play_pipeline;
    void unload_scene();

    scene_serializer serializer;

    std::map<unsigned int, std::string> scenes;
    int loaded_scene = -1;
    flecs::world main_world;

    void set_editor();

    void set_play();
};
