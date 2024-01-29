#include "freefly_camera.h"

#include "src/flecs_modules/transformation/transformation.h"

void freefly_camera::tick(GLFWwindow* window, double delta) {
    double cursor_x;
    double cursor_y;
    glfwGetCursorPos(window, &cursor_x, &cursor_y);
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

    constexpr float camera_speed = 7.5f;
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        transform_matrix = glm::translate(transform_matrix, camera_speed * (float)delta * transformation::global_forward());
    }
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        transform_matrix = glm::translate(transform_matrix, camera_speed * (float)delta * -transformation::global_forward());
    }
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        transform_matrix = glm::translate(transform_matrix, camera_speed * (float)delta * transformation::global_right());
    }
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        transform_matrix = glm::translate(transform_matrix, camera_speed * (float)delta * -transformation::global_right());
    }
    if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        transform_matrix = glm::translate(transform_matrix, camera_speed * (float)delta * transformation::global_up());
    }
    if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        transform_matrix = glm::translate(transform_matrix, camera_speed * (float)delta * -transformation::global_up());
    }
}

void freefly_camera::init(GLFWwindow* window) {
    glm::mat4x4 cam_matrix = glm::translate(glm::mat4x4(1.0f), glm::vec3(0.0f, 0.0f, 5.0f));
    transform_matrix = cam_matrix;

    glfwGetCursorPos(window, &cam_x, &cam_y);
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}