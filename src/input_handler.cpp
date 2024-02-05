#include "input_handler.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

void input_handler::process_input(flecs::world& world, GLFWwindow* window) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    auto* input_table = world.get_mut<frame_input_table>();

    glfwGetCursorPos(window, &input_table->mouse_x, &input_table->mouse_y);
    input_table->mouse_right_click = glfwGetMouseButton(window, 1);

    input_table->key_w = glfwGetKey(window, GLFW_KEY_W);
    input_table->key_s = glfwGetKey(window, GLFW_KEY_S);
    input_table->key_a = glfwGetKey(window, GLFW_KEY_A);
    input_table->key_d = glfwGetKey(window, GLFW_KEY_D);
    input_table->key_space_bar = glfwGetKey(window, GLFW_KEY_SPACE);
    input_table->key_left_shift = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT);
}
