#pragma once

#include <entt/entt.hpp>
#include <utility>
#include <tiny_gltf.h>

class scene_serializer {
public:
    explicit scene_serializer(std::string filename) : file(std::move(filename)), active_node(nullptr) {}
    ~scene_serializer();

public:
    void operator()(entt::entity e);
    void operator()(std::underlying_type_t<entt::entity> size);

    template<typename T>
    void operator()(const T & comp);

private:
    tinygltf::Model model;
    tinygltf::Node* active_node;

    const std::string file;
};

template<typename T>
void scene_serializer::operator()(const T & comp) {
    comp.serialize(model, active_node);
}
