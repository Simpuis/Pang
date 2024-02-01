#pragma once

#include <map>
#include <gsl/gsl>
#include <memory>
#include <tiny_gltf.h>
#include <flecs.h>
#include "src/serialization/scene_serializer.h"

class mesh;

struct mesh_table {
    std::vector<std::shared_ptr<mesh>> table;
};

class mesh_serializer {
public:
    static void save(serializer_save_data& save_data);
    static void load(serializer_load_data& load_data);
};
