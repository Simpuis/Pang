#pragma once

#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>
#include <GLFW/glfw3.h>

#include "camera.h"

class freefly_camera : public camera {
public:
    void init(GLFWwindow* window);
    void tick(GLFWwindow* window, double delta);

    [[nodiscard]] inline glm::mat4x4 get_view_matrix() const override {
        return glm::inverse(transform_matrix * local_trans);
    }

private:
    glm::mat4x4 local_trans = glm::mat4x4(1.0f);

    double cam_x = 0.0;
    double cam_y = 0.0;
};
