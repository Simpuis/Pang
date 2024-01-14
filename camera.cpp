#include "camera.h"

#include <glm/glm.hpp>

glm::mat4x4 camera::get_view_matrix() const {
    return glm::inverse(transform_matrix);
}

glm::vec3 camera::forward() const {
    return glm::normalize(glm::vec3(transform_matrix[0][2], transform_matrix[1][2], transform_matrix[2][2]));
}
