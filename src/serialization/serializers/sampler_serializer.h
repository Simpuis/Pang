#pragma once

#include <vector>
#include <memory>
#include <tiny_gltf.h>

#include "src/serialization/scene_serializer.h"

struct sampler_table {
    std::vector<tinygltf::Sampler> table;
};

class sampler_serializer {
public:
    static void save(serializer_save_data& save_data) {
        if(!save_data.world.has<sampler_table>()) return;
        auto* samplers = save_data.world.get<sampler_table>();

        save_data.model.samplers = samplers->table;
    }

    static void load(serializer_load_data& load_data) {
        load_data.world.set<sampler_table>(sampler_table());
        auto* samplers = load_data.world.get_mut<sampler_table>();

        samplers->table = load_data.model.samplers;
    }
};
