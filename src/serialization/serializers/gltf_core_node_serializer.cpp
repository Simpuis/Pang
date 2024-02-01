#include "gltf_core_node_serializer.h"

#include "src/flecs_modules/transformation/transformation.h"

void gltf_core_node_serializer::save(serializer_save_data& save_data) {
    for(auto pair : save_data.entity_node_map) {
        auto* node = &save_data.model.nodes[pair.second];
        if(auto* pos = pair.first.get<position>(); pos) {
            node->translation.push_back(pos->pos.x);
            node->translation.push_back(pos->pos.y);
            node->translation.push_back(pos->pos.z);
        }

        if(auto* rot = pair.first.get<rotation>(); rot) {
            node->rotation.push_back(rot->rot.x);
            node->rotation.push_back(rot->rot.y);
            node->rotation.push_back(rot->rot.z);
            node->rotation.push_back(rot->rot.w);
        }

        if(auto* local_scale = pair.first.get<scale>(); local_scale) {
            node->scale.push_back(local_scale->local_scale.x);
            node->scale.push_back(local_scale->local_scale.y);
            node->scale.push_back(local_scale->local_scale.z);
        }
    }
}

void gltf_core_node_serializer::load(serializer_load_data& load_data) {
    for(auto pair : load_data.node_entity_map) {
        if(!load_data.model.nodes[pair.first].translation.empty()) {
            pair.second.set<position>({static_cast<float>(load_data.model.nodes[pair.first].translation[0]),
                                        static_cast<float>(load_data.model.nodes[pair.first].translation[1]),
                                        static_cast<float>(load_data.model.nodes[pair.first].translation[2])});
        }
        else {
            pair.second.set<position>(position());
        }

        if(!load_data.model.nodes[pair.first].rotation.empty()) {
            pair.second.set<rotation>({static_cast<float>(load_data.model.nodes[pair.first].rotation[0]),
                                        static_cast<float>(load_data.model.nodes[pair.first].rotation[1]),
                                        static_cast<float>(load_data.model.nodes[pair.first].rotation[2]),
                                        static_cast<float>(load_data.model.nodes[pair.first].rotation[3])});
        }
        else {
            pair.second.set<rotation>(rotation());
        }

        if(!load_data.model.nodes[pair.first].scale.empty()) {
            pair.second.set<scale>({static_cast<float>(load_data.model.nodes[pair.first].scale[0]),
                                        static_cast<float>(load_data.model.nodes[pair.first].scale[1]),
                                        static_cast<float>(load_data.model.nodes[pair.first].scale[2])});
        }
        else {
            pair.second.set<scale>(scale());
        }

        if(load_data.model.nodes[pair.first].mesh >= 0) {
            pair.second.set<mesh_component>(mesh_component(load_data.model.nodes[pair.first].mesh));
        }
    }
}
