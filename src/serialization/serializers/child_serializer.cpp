#include "child_serializer.h"
#include "src/serialization/scene_serializer.h"

void child_serializer::save(serializer_save_data &save_data) {
    for(auto pair : save_data.entity_node_map) {
        pair.first.children([&](flecs::entity child) {
            save_data.model.nodes[pair.second].children.push_back(save_data.entity_node_map[child]);
        });
    }
}

void child_serializer::load(serializer_load_data &load_data) {
    auto root = load_data.world.get<scene_root>()->root_entity;

    for(auto pair : load_data.node_entity_map) {
        for(auto child : load_data.model.nodes[pair.first].children) {
            load_data.node_entity_map[child].child_of(pair.second);
        }
    }

    for(auto pair : load_data.node_entity_map) {
        if(!pair.second.parent().is_valid()) {
            pair.second.child_of(root);
        }
    }
}
