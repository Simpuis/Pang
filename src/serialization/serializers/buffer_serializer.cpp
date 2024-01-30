#include "buffer_serializer.h"

void buffer_serializer::save(const flecs::world &world, tinygltf::Model &model,
                             std::map<const flecs::entity, gsl::index> &entity_node_map) {
    if(!world.has<scene_buffer_table>()) return;
    auto buffer_table = world.get<scene_buffer_table>();

    for(unsigned int table_index = 0; table_index < buffer_table->table.size(); table_index++) {
        const auto data = buffer_table->table.at(table_index);

        model.accessors.insert(model.accessors.end(), data->accessors.begin(), data->accessors.end());
        model.bufferViews.insert(model.bufferViews.end(), data->buffer_views.begin(), data->buffer_views.end());
        model.buffers.insert(model.buffers.end(), data->buffers.begin(), data->buffers.end());
    }
}


void buffer_serializer::load(flecs::world& world, const tinygltf::Model& model,
                             std::map<gsl::index, flecs::entity>& node_entity_map) {
    if(!world.has<scene_buffer_table>()) {
        auto buffer_table = scene_buffer_table();
        world.set<scene_buffer_table>(buffer_table);
    }
    auto* buffer_table = world.get_mut<scene_buffer_table>();

    auto data = std::make_shared<scene_buffer_data>();
    data->accessors = model.accessors;
    data->buffer_views = model.bufferViews;
    data->buffers = model.buffers;

    buffer_table->table.insert({(unsigned int)buffer_table->table.size(), data});
}
