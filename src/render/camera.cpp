#include "camera.h"

#include <glm/glm.hpp>

glm::mat4x4 camera::get_view_matrix() const {
    return glm::inverse(transform_matrix);
}
