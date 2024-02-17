#pragma once

#include <glm/glm.hpp>
#include <flecs.h>

struct freefly_controller {
    glm::mat4x4 local_trans = glm::mat4x4(1.0f);
    double cam_x = 0.0;
    double cam_y = 0.0;
};

struct freefly {
    explicit freefly(flecs::world& world);
};