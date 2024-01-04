#pragma once

#include <string>
#include <utility>
#include <entt/entt.hpp>

namespace tinygltf{
    class Model;
    class Node;
}

struct name {
    explicit name(std::string s) : name_string(std::move(s)) {}

    std::string name_string;

    void serialize(tinygltf::Model& model, tinygltf::Node* node) const;
    static void deserialize(const tinygltf::Node& node, entt::registry& registry, const entt::entity& entity);
};
