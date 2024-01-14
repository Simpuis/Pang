#pragma once

#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>

class camera {
public:
    camera() : transform_matrix(glm::mat4x4(1)) {}
    explicit camera(glm::mat4x4 transform_matrix) : transform_matrix(transform_matrix) {}

    [[nodiscard]] virtual glm::mat4x4 get_view_matrix() const;
    glm::mat4x4 transform_matrix;

    glm::vec3 forward() const;
};
