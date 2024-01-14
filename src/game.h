#pragma once

#include <string>
#include <memory>
#include <entt/entity/registry.hpp>

#include "editor/editor.h"
#include "input_handler.h"
#include "render/renderer.h"
#include "render/camera.h"
#include "render/freefly_camera.h"

/**
 * @brief The game class is the main class of the engine.
 * It handles setting up the glfw window and contains the main loop.
 * It also owns and manages the ECS registry.
 */
class game
{
public:
    /**
     * @brief Construct a new game object
     *
     * @param width The width of the window
     * @param height The height of the window
     * @param title The title of the window
     */
	game(int width, int height, const std::string& title);
	~game();

public:
    /**
     * @brief The main loop of the game
     *
     */
	void loop();

    freefly_camera main_camera;
    GLFWwindow* window_;

private:
	void init_glfw_window(int width, int height, const std::string& title);
	void exit();

	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

	std::unique_ptr<input_handler> input_;

	entt::registry registry_;
	renderer renderer_;

	editor editor_;
};
