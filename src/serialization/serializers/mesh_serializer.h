#pragma once

#include <map>
#include <gsl/gsl>
#include <memory>
#include <tiny_gltf.h>
#include <flecs.h>

class mesh;

struct mesh_table {
    std::map<unsigned int, std::shared_ptr<mesh>> table;
};

class mesh_serializer {
public:
    static void save(const flecs::world& world, tinygltf::Model& model, std::map<const flecs::entity, gsl::index>& entity_node_map);
    static void load(flecs::world& world, const tinygltf::Model& model, std::map<gsl::index, flecs::entity>& node_entity_map);
};
