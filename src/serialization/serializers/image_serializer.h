#pragma once

#include <vector>
#include <memory>
#include <tiny_gltf.h>

#include "src/serialization/scene_serializer.h"

struct image_table {
    std::vector<tinygltf::Image> table;
};

class image_serializer {
public:
    static void save(serializer_save_data& save_data) {
        if(!save_data.world.has<image_table>()) return;
        auto* images = save_data.world.get<image_table>();

        save_data.model.images = images->table;
    }

    static void load(serializer_load_data& load_data) {
        load_data.world.set<image_table>(image_table());
        auto* images = load_data.world.get_mut<image_table>();

        images->table = load_data.model.images;
    }
};
