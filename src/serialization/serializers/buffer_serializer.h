#pragma once

#include <map>
#include <gsl/gsl>
#include <vector>
#include <memory>
#include <tiny_gltf.h>
#include <flecs.h>

struct scene_buffer_data {
    std::vector<tinygltf::Accessor> accessors;
    std::vector<tinygltf::BufferView> buffer_views;
    std::vector<tinygltf::Buffer> buffers;
};

struct scene_buffer_table {
    std::map<unsigned int, std::shared_ptr<scene_buffer_data>> table;
};

class buffer_serializer {
public:
    static void save(const flecs::world& world, tinygltf::Model& model, std::map<const flecs::entity, gsl::index>& entity_node_map);
    static void load(flecs::world& world, const tinygltf::Model& model, std::map<gsl::index, flecs::entity>& node_entity_map);

};
