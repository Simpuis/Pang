#include "freefly_camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/detail/type_quat.hpp>
#include <glm/gtc/quaternion.hpp>

#include "src/game.h"

void freefly_camera::tick(game& context, double delta) {
    double cursor_x;
    double cursor_y;
    glfwGetCursorPos(context.window_, &cursor_x, &cursor_y);
    double delta_x = cam_x - cursor_x;
    double delta_y = cam_y - cursor_y;

    if(abs(delta_x) > 0.00001 || abs(delta_y) > 0.00001) {
        constexpr float rot_speed = 0.1f;
        glm::quat rot1 = glm::angleAxis((float)(delta_x * rot_speed * delta), glm::vec3(0.0, 1.0, 0.0));
        transform_matrix *= glm::mat4_cast(rot1);
        glm::quat rot2 = glm::angleAxis((float)(delta_y * rot_speed * delta), glm::vec3(1.0, 0.0, 0.0));
        local_trans *= glm::mat4_cast(rot2);
    }

    cam_x = cursor_x;
    cam_y = cursor_y;

    constexpr glm::vec3 forward = glm::vec3(0.0f, 0.0f, -1.0f);
    constexpr glm::vec3 right = glm::vec3(-1.0f, 0.0f, 0.0f);
    constexpr glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

    constexpr float camera_speed = 7.5f;
    if(glfwGetKey(context.window_, GLFW_KEY_W) == GLFW_PRESS) {
        transform_matrix = glm::translate(transform_matrix, camera_speed * (float)delta * forward);
    }
    if(glfwGetKey(context.window_, GLFW_KEY_S) == GLFW_PRESS) {
        transform_matrix = glm::translate(transform_matrix, camera_speed * (float)delta * -forward);
    }
    if(glfwGetKey(context.window_, GLFW_KEY_A) == GLFW_PRESS) {
        transform_matrix = glm::translate(transform_matrix, camera_speed * (float)delta * right);
    }
    if(glfwGetKey(context.window_, GLFW_KEY_D) == GLFW_PRESS) {
        transform_matrix = glm::translate(transform_matrix, camera_speed * (float)delta * -right);
    }
    if(glfwGetKey(context.window_, GLFW_KEY_SPACE) == GLFW_PRESS) {
        transform_matrix = glm::translate(transform_matrix, camera_speed * (float)delta * +up);
    }
    if(glfwGetKey(context.window_, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        transform_matrix = glm::translate(transform_matrix, camera_speed * (float)delta * -up);
    }
}

void freefly_camera::init(game& context) {
    glm::mat4x4 cam_matrix = glm::translate(glm::mat4x4(1.0f), glm::vec3(0.0f, 0.0f, 5.0f));
    transform_matrix = cam_matrix;

    glfwGetCursorPos(context.window_, &cam_x, &cam_y);
    glfwSetInputMode(context.window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

glm::mat4x4 freefly_camera::get_view_matrix() const {
    return glm::inverse(transform_matrix * local_trans);
}
