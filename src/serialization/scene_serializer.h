#pragma once

#include <entt/entt.hpp>
#include <utility>
#include <tiny_gltf.h>
#include "src/render/material.h"

class scene_serializer {
public:
    explicit scene_serializer(std::string&& filename) : active_node(nullptr), file(std::move(filename)) {}
    ~scene_serializer();

public:
    void operator()(entt::entity e);
    void operator()(std::underlying_type_t<entt::entity> size);

    template<typename T>
    void operator()(const T & comp);

    std::uint32_t get_material_index(const material* mat);

private:
    std::map<const material*, std::uint32_t> material_indices;

    tinygltf::Model model;
    tinygltf::Node* active_node;

    const std::string file;
};

template<typename T>
void scene_serializer::operator()(const T & comp) {
    comp.serialize(*this, model, active_node);
}
