#include "input_handler.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

void input_handler::process_input(GLFWwindow* window)
{
	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}
