#pragma once

#include <glm/glm.hpp>
#include <entt/entt.hpp>

namespace tinygltf {
    class Model;
    class Node;
}

struct transform
{
	transform() : transform_matrix(glm::mat4(0.0f)) {}
	explicit transform(const glm::mat4& trans) : transform_matrix(trans) {}

	glm::mat4 transform_matrix;

    void serialize(tinygltf::Model& model, tinygltf::Node* node) const;
    static void deserialize(const tinygltf::Node& node, entt::registry& registry, const entt::entity& entity);
};
