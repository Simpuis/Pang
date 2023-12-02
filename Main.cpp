#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "game.h"

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	auto game_context = std::make_unique<game>(1920, 1080, "Pong");
	game_context->loop();

	return 0;
}
