#include "game.h"

#include <iostream>
#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <entt/entity/registry.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "material.h"
#include "shader.h"
#include "shader_loader.h"
#include "sprite.h"
#include "texture_loader.h"
#include "transform.h"

game::game(int width, int height, const std::string& title)
{
	init_glfw_window(width, height, title);
	input_ = std::make_unique<input_handler>(input_handler());

	registry_ = entt::registry();
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
	auto entitty = registry_.create();

	auto& material_comp = registry_.emplace<material>(entitty);
	material_comp.material_shader = std::unique_ptr<shader>(shader_loader::load_shader("vertex_shader.glsl", "fragment_shader.glsl"));
	material_comp.set_texture("tex", texture_loader::load_texture("container.jpg", GL_RGB), 0);
	material_comp.set_texture("tex2", texture_loader::load_texture("awesomeface.png", GL_RGB), 1);

	registry_.emplace<sprite>(entitty);

	auto& trans = registry_.emplace<transform>(entitty, glm::mat4(1.0f));
	trans.transform_matrix = glm::rotate(trans.transform_matrix, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	trans.transform_matrix = glm::scale(trans.transform_matrix, glm::vec3(0.5f, 0.5f, 0.5f));

	double lastFrameTime = glfwGetTime();
	while (!glfwWindowShouldClose(window_)) {
		const double currentFrameTime = glfwGetTime();
		const double delta = currentFrameTime - lastFrameTime;
		lastFrameTime = currentFrameTime;

		input_->process_input(window_);

		trans.transform_matrix[3] = glm::vec4(sin((float)glfwGetTime()), 0.0f, 0.0f, 1.0f);

		glClearColor(0.2f, 0.3f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		renderer_.render_scene(registry_, window_);

		glfwPollEvents();
	}
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

	glViewport(0, 0, width, height);
	glfwSetFramebufferSizeCallback(window_, framebuffer_size_callback);
}

void game::exit()
{
	glfwSetWindowShouldClose(window_, true);
}
