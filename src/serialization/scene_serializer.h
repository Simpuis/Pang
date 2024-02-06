#pragma once

#include <tiny_gltf.h>
#include <flecs.h>
#include <memory>
#include <iostream>
#include <map>
#include <gsl/gsl>

#include "src/flecs_modules/transformation/transformation.h"
#include "src/render/mesh.h"
#include "src/flecs_modules/rendering/rendering.h"
#include "serializers/serializer.h"

struct scene_root {
    flecs::entity root_entity;
};

class scene_serializer {
public:
    enum gltf_file_type {
        ascii, binary
    };

public:
    scene_serializer();

public:
    void save_scene_to_file(flecs::world &world, const std::string &filename);
    void load_scene_from_file(flecs::world &world, const std::string &filename);
    void register_serializer(std::unique_ptr<serializer>&& serializer_instance);

private:
    static bool load_scene_file(tinygltf::Model &model, const std::string &filename);

    std::vector<std::unique_ptr<serializer>> serializers;
    bool write_mode = false;
};
