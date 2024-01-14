#pragma once

#include <glm/glm.hpp>
#include <entt/entt.hpp>

namespace tinygltf {
    class Model;
    class Node;
}

class scene_serializer;
struct deserialization_data;

/**
 * @brief A transform component.
 *
 * This component is used to represent the position, rotation, and scale of an entity.
 *
 */

struct transform
{
    /**
     * @brief Default constructor.
     * The transform is set to the identity matrix.
     */
	transform() = default;
    /**
     * @brief Construct the transform with a given matrix.
     * @param trans The matrix to use for the transform.
     */
	explicit transform(const glm::mat4& trans) : transform_matrix(trans) {}

	glm::mat4 transform_matrix = glm::mat4(1.0f);

    void serialize(scene_serializer& serializer, tinygltf::Model& model, tinygltf::Node* node) const;
    static void deserialize(deserialization_data& data);
};
