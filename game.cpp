#include "game.h"

#include <iostream>
#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include "node.h"
#include "shader.h"
#include "sprite.h"

game::game(int width, int height, const std::string& title)
{
	init_glfw_window(width, height, title);
	input_ = std::make_unique<input_handler>(input_handler());
	
}

game::~game()
{
	glfwTerminate();
}

void game::framebuffer_size_callback(GLFWwindow* window, const int width, const int height) {
	glViewport(0, 0, width, height);
}

void game::loop()
{
	const node* root_node = new node();

	const auto vertex_shader_source = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"void main()\n"
		"{\n"
		"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}\0";

	const auto fragment_shader_source = "#version 330 core\n"
		"out vec4 fragColor;\n"
		"void main()\n"
		"{\n"
		"    fragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
		"}\0";

	const shader* program = new shader(vertex_shader_source, fragment_shader_source);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), static_cast<void*>(nullptr));
	glEnableVertexAttribArray(0);

	while (!glfwWindowShouldClose(window_)) {
		input_->process_input(window_);

		glClearColor(0.2f, 0.3f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		program->use();
		glBindVertexArray(root_node->sprite_->VAO_);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glfwSwapBuffers(window_);
		glfwPollEvents();
	}

	delete(root_node);
	delete(program);
}

void game::init_glfw_window(const int width, const int height, const std::string& title)
{
	window_ = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
	if (window_ == nullptr) {
		std::cout << "Failed to create GLFW window" << std::endl;
		exit();
	}
	glfwMakeContextCurrent(window_);

	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		exit();
	}

	glViewport(0, 0, 800, 600);
	glfwSetFramebufferSizeCallback(window_, framebuffer_size_callback);
}

void game::exit()
{
	glfwSetWindowShouldClose(window_, true);
}
