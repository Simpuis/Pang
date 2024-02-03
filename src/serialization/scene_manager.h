#pragma once

#include <string>
#include <map>
#include <memory>
#include <flecs.h>

#include "scene_serializer.h"

class scene_serializer;

class scene_manager {
public:
    scene_manager(std::initializer_list<std::pair<unsigned int, std::string>> scene_filenames);

public:
    template<typename... Serializer_Ts>
    void load_scene(flecs::world& world, unsigned int scene_index);

    int get_current_scene_index() const;
    scene_serializer& get_serializer() { return this->serializer; }

private:
    void unload_scene(flecs::world& world);

    scene_serializer serializer;

    std::map<unsigned int, std::string> scenes;
    int loaded_scene = -1;
};

template<typename... Serializer_Ts>
void scene_manager::load_scene(flecs::world& world, unsigned int scene_index) {
    unload_scene(world);

    serializer.load_scene_from_file(world, scenes[scene_index]);
}
