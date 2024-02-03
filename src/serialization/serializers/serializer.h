#pragma once

#include <tiny_gltf.h>
#include <flecs.h>
#include <gsl/gsl>

struct serializer_save_data {
    serializer_save_data(tinygltf::Model& model,
                         std::map<const flecs::entity, gsl::index>& entity_node_map,
                         const flecs::world& world)
            : model(model), entity_node_map(entity_node_map), world(world) {}

    tinygltf::Model& model;
    std::map<const flecs::entity, gsl::index>& entity_node_map;
    const flecs::world& world;
};

struct serializer_load_data {
    serializer_load_data(const tinygltf::Model& model,
                         std::map<gsl::index, flecs::entity>& node_entity_map,
                         flecs::world& world)
            : model(model), node_entity_map(node_entity_map), world(world) {}

    const tinygltf::Model& model;
    std::map<gsl::index, flecs::entity>& node_entity_map;
    flecs::world& world;
};

class serializer {
public:
    virtual void save(serializer_save_data& save_data) = 0;
    virtual void load(serializer_load_data& load_data) = 0;
};
