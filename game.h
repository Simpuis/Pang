#pragma once

#include <string>
#include <memory>
#include <entt/entity/registry.hpp>

#include "editor.h"
#include "input_handler.h"
#include "renderer.h"

class game
{
public:
	game(int width, int height, const std::string& title);
	~game();

public:
	void loop();

private:
	void init_glfw_window(int width, int height, const std::string& title);
	void exit();

	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

	std::unique_ptr<input_handler> input_;
	GLFWwindow* window_;

	entt::registry registry_;
	renderer renderer_;

	editor editor_;
};
