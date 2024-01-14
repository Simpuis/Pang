#pragma once

#include <glm/glm.hpp>
#include <entt/entt.hpp>

namespace tinygltf {
    class Model;
    class Node;
}

class scene_serializer;
struct deserialization_data;

struct transform
{
	transform() = default;
	explicit transform(const glm::mat4& trans) : transform_matrix(trans) {}

	glm::mat4 transform_matrix = glm::mat4(0.0f);

    void serialize(scene_serializer& serializer, tinygltf::Model& model, tinygltf::Node* node) const;
    static void deserialize(deserialization_data& data);
};
