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
};

struct scene_save_command {
    // May optionally provide a new save path, or the loaded file will be overwritten
    std::optional<std::string> save_path;
};

struct new_scene_command {
    bool poo = false;
};

class scene_manager {
public:
    scene_manager(std::initializer_list<std::pair<unsigned int, std::string>> scene_filenames);

public:
    void update();
    void load_scene(unsigned int scene_index);
    void load_scene(std::string scene_filename);

    int get_current_scene_index() const;
    scene_serializer& get_serializer() { return this->serializer; }
    flecs::world& get_world() { return main_world; }

private:
    void unload_scene();

    scene_serializer serializer;

    std::map<unsigned int, std::string> scenes;
    int loaded_scene = -1;
    flecs::world main_world;
};

