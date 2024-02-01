#include "buffer_serializer.h"

void buffer_serializer::save(serializer_save_data& save_data) {
    if(!save_data.world.has<scene_buffer_data>()) return;
    auto buffer_table = save_data.world.get<scene_buffer_data>();

    save_data.model.accessors = buffer_table->accessors;
    save_data.model.bufferViews = buffer_table->buffer_views;
    save_data.model.buffers = buffer_table->buffers;
}


void buffer_serializer::load(serializer_load_data& load_data) {
    if(!load_data.world.has<scene_buffer_data>()) {
        auto buffer_table = scene_buffer_data();
        load_data.world.set<scene_buffer_data>(buffer_table);
    }
    auto* buffer_table = load_data.world.get_mut<scene_buffer_data>();

    buffer_table->accessors.insert(buffer_table->accessors.end(),
                                      load_data.model.accessors.begin(), load_data.model.accessors.end());
    buffer_table->buffer_views.insert(buffer_table->buffer_views.end(),
                                      load_data.model.bufferViews.begin(), load_data.model.bufferViews.end());
    buffer_table->buffers.insert(buffer_table->buffers.end(),
                                      load_data.model.buffers.begin(), load_data.model.buffers.end());
}
