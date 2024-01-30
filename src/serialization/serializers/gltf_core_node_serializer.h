#pragma once

#include <map>
#include <tiny_gltf.h>
#include <flecs.h>

class gltf_core_node_serializer {
public:
    static void save(const flecs::world& world, tinygltf::Model& model, std::map<const flecs::entity, tinygltf::Node*>& entity_node_map);
    static void load(flecs::world& world, const tinygltf::Model& model, std::map<const tinygltf::Node*, flecs::entity>& node_entity_map);
};
