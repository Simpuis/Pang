#include "material_serializer.h"

#include <src/render/material.h>

#include "src/flecs_modules/rendering/rendering.h"

void material_serializer::save(serializer_save_data& save_data) {
    if(!save_data.world.has<material_table>()) return;
    auto* materials = save_data.world.get<material_table>();

    for(const auto& unserialized_material : materials->table) {
        save_data.model.materials.push_back(unserialized_material->serialize());
    }
}

void material_serializer::load(serializer_load_data& load_data) {
    load_data.world.set<material_table>(material_table());
    auto* materials = load_data.world.get_mut<material_table>();

    for(const auto& mat : load_data.model.materials) {
        materials->table.push_back(material::deserialize(load_data.model, mat));
    }
}
