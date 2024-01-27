#pragma once

#include <glm/glm.hpp>

/**
 * @brief A camera class that can be used to render a scene.
 *
 */
class camera {
public:
    /**
     * @brief Construct a new camera object
     * Sets its position to the origin
     */
    camera() : transform_matrix(glm::mat4x4(1)) {}
    /**
     * @brief Construct a new camera object
     *
     * @param transform_matrix The position and rotation of the camera
     */
    explicit camera(glm::mat4x4 transform_matrix) : transform_matrix(transform_matrix) {}

    /**
     * @brief Get the view matrix of the camera
     *
     * @return glm::mat4x4 The view matrix of the camera
     */
    [[nodiscard]] virtual glm::mat4x4 get_view_matrix() const {
        return glm::inverse(transform_matrix);
    }
    glm::mat4x4 transform_matrix;
};
