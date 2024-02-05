#include "texture_serializer.h"

#include "src/render/texture.h"
#include "src/flecs_modules/rendering/rendering.h"

void texture_serializer::save(serializer_save_data& save_data) {
    if(!save_data.world.has<texture_table>()) return;
    auto* textures = save_data.world.get<texture_table>();

    for(const auto& unserialized_texture : textures->table) {
        save_data.model.textures.push_back(unserialized_texture->serialize());
    }
}

void texture_serializer::load(serializer_load_data& load_data) {
    load_data.world.set<texture_table>(texture_table());
    auto* textures = load_data.world.get_mut<texture_table>();

    for(const auto& tex : load_data.model.textures) {
        textures->table.push_back(texture::deserialize(load_data.model, tex));
    }
}
