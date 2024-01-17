#pragma once

#include <glm/glm.hpp>
#include <glm/detail/type_quat.hpp>
#include <glm/gtc/quaternion.hpp>

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
     * The position is set to origin, the rotation to identity and the scale to 1.
     */
	transform() = default;
    /**
     * @brief Constructs the transform from a given position, rotation and scale.
     * @param position
     * @param rotation
     * @param local_scale
     */
	explicit transform(const glm::vec3 position, const glm::quat rotation, const glm::vec3 local_scale)
                       : position(position), rotation(rotation), local_scale(local_scale) {}

    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    glm::vec3 local_scale = glm::vec3(1.0f, 1.0f, 1.0f);

    /**
     * @brief Returns the transform matrix constructed from position, rotation and local_scale.
     */
    [[nodiscard]] glm::mat4x4 get_matrix() const {
        glm::mat4x4 matrix = glm::mat4x4(1.0f);
        matrix = glm::translate(matrix, position);
        matrix *= glm::mat4_cast(rotation);
        matrix = glm::scale(matrix, local_scale);
        return matrix;
    }

    void serialize(scene_serializer& serializer, tinygltf::Model& model, tinygltf::Node* node) const;
    static void deserialize(deserialization_data& data);
};
