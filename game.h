#pragma once

#include <string>
#include <memory>

#include "input_handler.h"

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
};

