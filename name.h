#pragma once

#include <string>
#include <utility>
#include <entt/entt.hpp>

namespace tinygltf{
    class Model;
    class Node;
}

class scene_serializer;
struct deserialization_data;

struct name {
    explicit name(std::string s) : name_string(std::move(s)) {}

    std::string name_string;

    void serialize(scene_serializer& serializer, tinygltf::Model& model, tinygltf::Node* node) const;
    static void deserialize(deserialization_data& data);
};
